# ThinCloud C Embedded SDK

A header-only helper library for embedded devices wanting to connect to ThinCloud.

Documentation can be found [here](https://yonomi.github.io/thincloud-embedded-c-sdk/docs/html/index.html)

## Dependencies

- aws-iot-device-sdk-embedded-C
- json-c

## Usage

To use, include `thincloud.h` in your project and link all dependencies.

See `Makefile` for an example.

## Example

```c
#include "thincloud.h"

static void subscribe_callback_handler(
    AWS_IoT_Client *client,
    char *topicName,
    uint16_t topicNameLen,
    IoT_Publish_Message_Params *params,
    void *data
    )
{
    /* Handle responses */
    exit(0);
}

int main()
{
    AWS_IoT_Client client;

    IoT_Error_t rc = tc_init(&client, "xxxxxxxxxxxxxx.iot.us-east-1.amazonaws.com", "rootCA.crt", "cert.pem", "privkey.pem", NULL, NULL);

    if (rc != SUCCESS)
    {
        return rc;
    }

    rc = tc_connect(&client, "c-sdk-client-id", false);

    if (rc != SUCCESS)
    {
        return rc;
    }

    rc = subscribe_to_commissioning_response(&client, "1235", "lock", "85599", subscribe_callback_handler, NULL);

    while((NETWORK_ATTEMPTING_RECONNECT == rc || NETWORK_RECONNECTED == rc || SUCCESS == rc)
            && (publishCount > 0 || infinitePublishFlag))
    {
        rc = aws_iot_mqtt_yield(&client, 100);
        if(NETWORK_ATTEMPTING_RECONNECT == rc)
        {
            continue;
        }

        rc = send_commissioning_request(&client, "1235", "lock", "85599");
    }

    return 0;
}
```

## Build docs

```bash
$ doxygen Doxyfile
```

## Testing

### Build

```bash
$ git clone https://github.com/aws/aws-iot-device-sdk-embedded-C
$ git clone https://github.com/ARMmbed/mbedtls aws-iot-device-sdk-embedded-C/external_libs/mbedtls
$ cd thincloud-embedded-c-sdk/tests
$ make
```

### Run

```bash
$ ./tests
```
