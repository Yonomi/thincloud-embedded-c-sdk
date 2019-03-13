# ThinCloud C Embedded SDK

A header-only helper library for embedded devices wanting to connect to ThinCloud.

Documentation can be found [here](https://yonomi.github.io/thincloud-embedded-c-sdk/html/thincloud_8h.html)

## Dependencies

- [aws-iot-device-sdk-embedded-C](https://github.com/aws/aws-iot-device-sdk-embedded-C) (3.0.0)
- [json-c](https://github.com/json-c/json-c) (0.13 or greater)

## Usage

To use, include `thincloud.h` in your project and link all dependencies.

See `Makefile` for an example.

## Example

```c
#include "thincloud.h"

static bool isCommissioned = false;
static bool requestedCommission = false;
static char deviceId[TC_ID_LENGTH];

static void command_callback_handler(
    AWS_IoT_Client *client,
    char *topicName,
    uint16_t topicNameLen,
    IoT_Publish_Message_Params *params,
    void *data)
{
    char commandId[TC_ID_LENGTH];
    char method[16];

    IoT_Error_t rc = command_request(commandId, method, NULL, params->payload, params->payloadLen);
    if (rc != SUCCESS)
    {
        IOT_ERROR("Failed to proccess command request: rc = %d", rc);
        return;
    }

    if (strcmp(method, "ping") == 0)
    {
        json_object *body = json_object_new_object();
        json_object *data = json_object_new_object();
        json_object *pong = json_object_new_string("pong");
        json_object_object_add(data, "echo", pong);
        json_object_object_add(body, "data", data);
        rc = send_command_response(client, deviceId, commandId, 200, false, NULL, body);
        if (rc != SUCCESS)
        {
            IOT_ERROR("Failed to publish command response: rc = %d", rc);
            return;
        }
    }
}

static void commissioning_callback_handler(
    AWS_IoT_Client *client,
    char *topicName,
    uint16_t topicNameLen,
    IoT_Publish_Message_Params *params,
    void *data)
{
    isCommissioned = true;

    static uint16_t statusCode = 0;

    IoT_Error_t rc = commissioning_response(deviceId, &statusCode, NULL, params->payload, params->payloadLen);
    if (rc != SUCCESS)
    {
        IOT_ERROR("Failed to process commissioning response: rc = %d", rc);
        return;
    }

    if (statusCode != 200)
    {
        IOT_ERROR("Failed to commission device: Status Code = %d", statusCode);
        abort();
    }

    rc = subscribe_to_command_request(client, deviceId, command_callback_handler, NULL);
    if (rc != SUCCESS)
    {
        IOT_ERROR("Failed to subscribe to command request topic: rc = %d", rc);
        return;
    }
}

int main()
{
    AWS_IoT_Client client;

    IoT_Error_t rc = tc_init(&client, "xxxxxxxxxxxxxxxxxx.us-east-1.amazonaws.com", "rootCA.pem", "client.crt", "client.key", NULL, NULL);

    if (rc != SUCCESS)
    {
        return rc;
    }

    rc = tc_connect(&client, "my-client-id", true);

    if (rc != SUCCESS)
    {
        return rc;
    }

    const char *commissionRequestId = "12345";
    const char *deviceType = "lock";
    const char *physicalId = "56789";

    rc = subscribe_to_commissioning_response(&client, commissionRequestId, deviceType, physicalId, commissioning_callback_handler, NULL);

    while ((NETWORK_ATTEMPTING_RECONNECT == rc || NETWORK_RECONNECTED == rc || SUCCESS == rc))
    {
        rc = aws_iot_mqtt_yield(&client, 100);
        if (NETWORK_ATTEMPTING_RECONNECT == rc)
        {
            continue;
        }

        if (!isCommissioned && !requestedCommission)
        {
            rc = send_commissioning_request(&client, commissionRequestId, deviceType, physicalId, NULL, 0);
            requestedCommission = true;
        }
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
