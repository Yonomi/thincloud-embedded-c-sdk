/*
 * Copyright 2019 Yonomi, Inc. or its affiliates. All Rights Reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef THINCLOUD_EMBEDDED_C_SDK_
#define THINCLOUD_EMBEDDED_C_SDK_

/* 1.0.1 */
#define TC_EMBEDDED_C_SDK_VERSION_MAJOR 1
#define TC_EMBEDDED_C_SDK_VERSION_MINOR 0
#define TC_EMBEDDED_C_SDK_VERSION_PATCH 3

/*
 * Thincloud C Embedded SDK
 * 
 * Contains utilities to construct ThinCloud standard topics
 * and marshal and unmarshal request and responses.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <json-c/json.h>

#include "aws_iot_log.h"
#include "aws_iot_error.h"
#include "aws_iot_mqtt_client_interface.h"

/**
 * UUID standard length plus null character
 */
#define TC_ID_LENGTH 37

/**
 * AWS IoT Max Topic Length plus null character
 */
#define MAX_TOPIC_LENGTH 257

#define REQUEST_METHOD_GET "GET"
#define REQUEST_METHOD_PUT "PUT"
#define REQUEST_METHOD_POST "POST"
#define REQUEST_METHOD_DELETE "DELETE"

char COMMISSIONING_RESPONSE_TOPIC_BUFFER[MAX_TOPIC_LENGTH];
char COMMAND_TOPIC_BUFFER[MAX_TOPIC_LENGTH];
char SERVICE_RESPONSE_TOPIC_BUFFER[MAX_TOPIC_LENGTH];

/**
 * @brief Build a commission request topic
 * 
 * Constructs a commission request topic of the format
 *       "thincloud/registration/{deviceType}_{physicalId}/requests"
 *
 * @param[out]  buffer      Pointer to a string buffer to write to.
 * @param[in]   deviceType  Devices's device type.
 * @param[in]   physicalId  Device's physical ID.
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t commission_request_topic(char *buffer, const char *deviceType, const char *physicalId)
{
    if (buffer == NULL)
    {
        return 0;
    }

    if (deviceType == NULL || physicalId == NULL)
    {
        FUNC_EXIT_RC(NULL_VALUE_ERROR);
    }

    sprintf(buffer, "thincloud/registration/%s_%s/requests", deviceType, physicalId);

    FUNC_EXIT_RC(SUCCESS);
}

/**
 * @brief Build a commission response topic
 * 
 * Constructs a commission response topic of the format
 *       "thincloud/registration/{deviceType}_{physicalId}/requests/{requestId}/response"
 * 
 * @param[out]  buffer      Pointer to a string buffer to write to.
 * @param[in]   deviceType  Devices's device type.
 * @param[in]   physicalId  Device's physical ID.
 * @param[in]   requestId   Unique ID for the request.
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t commission_response_topic(char *buffer, const char *deviceType, const char *physicalId, const char *requestId)
{
    if (buffer == NULL)
    {
        return 0;
    }

    if (deviceType == NULL || physicalId == NULL || requestId == NULL)
    {
        FUNC_EXIT_RC(NULL_VALUE_ERROR);
    }

    sprintf(buffer, "thincloud/registration/%s_%s/requests/%s/response", deviceType, physicalId, requestId);

    FUNC_EXIT_RC(SUCCESS);
}

/**
 * @brief Build a command request topic
 * 
 * Constructs a command request topic of the format
 *       "thincloud/devices/{deviceId}/command"
 * 
 * @param[out]  buffer    Pointer to a string buffer to write to.
 * @param[in]   deviceId  Device's ID.
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t command_request_topic(char *buffer, const char *deviceId)
{
    if (buffer == NULL)
    {
        return 0;
    }

    if (deviceId == NULL)
    {
        FUNC_EXIT_RC(NULL_VALUE_ERROR);
    }

    sprintf(buffer, "thincloud/devices/%s/command", deviceId);

    FUNC_EXIT_RC(SUCCESS);
}

/**
 * @brief Build a command response topic
 * 
 * Constructs a command response topic of the format
 *       "thincloud/devices/{deviceId}/command/{commandId}/response"
 * 
 * @param[out]  buffer    Pointer to a string buffer to write to.
 * @param[in]   deviceId  Devices's ID.
 * @param[in]   commandId  Command request's ID.
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t command_response_topic(char *buffer, const char *deviceId, const char *commandId)
{
    if (buffer == NULL)
    {
        return 0;
    }

    if (deviceId == NULL || commandId == NULL)
    {
        FUNC_EXIT_RC(NULL_VALUE_ERROR);
    }

    sprintf(buffer, "thincloud/devices/%s/command/%s/response", deviceId, commandId);

    FUNC_EXIT_RC(SUCCESS);
}

/**
 * @brief Build a service request topic
 * 
 * Constructs a service request topic of the format
 *       "thincloud/devices/{deviceId}/requests"
 * 
 * @param[out]  buffer    Pointer to a string buffer to write to.
 * @param[in]   deviceId  Devices's ID.
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t service_request_topic(char *buffer, const char *deviceId)
{
    if (buffer == NULL)
    {
        return 0;
    }

    if (deviceId == NULL)
    {
        FUNC_EXIT_RC(NULL_VALUE_ERROR);
    }

    sprintf(buffer, "thincloud/devices/%s/requests", deviceId);

    FUNC_EXIT_RC(SUCCESS);
}

/**
 * @brief Build a service response topic
 * 
 * Constructs a service response topic of the format
 *       "thincloud/devices/{deviceId}/requests/%s/response"
 * 
 * @param[out] buffer     Pointer to a string buffer to write to.
 * @param[in]  deviceId   Devices's ID.
 * @param[in]  requestId  Request's ID.
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t service_response_topic(char *buffer, const char *deviceId, const char *requestId)
{
    if (buffer == NULL)
    {
        return 0;
    }

    if (deviceId == NULL || requestId == NULL)
    {
        FUNC_EXIT_RC(NULL_VALUE_ERROR);
    }

    sprintf(buffer, "thincloud/devices/%s/requests/%s/response", deviceId, requestId);

    FUNC_EXIT_RC(SUCCESS);
}

/**
 * @brief Build a commissioning request
 * 
 * Construct a commissioning request.
 * 
 * @param[out] buffer      Pointer to a string buffer to write to 
 * @param[in]  requestId   Unique ID for the request
 * @param[in]  deviceType  Requesting device's type
 * @param[in]  physicalId  Device's physical ID
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t commissioning_request(char *buffer, const char *requestId, const char *deviceType, const char *physicalId)
{
    if (deviceType == NULL || physicalId == NULL)
    {
        FUNC_EXIT_RC(NULL_VALUE_ERROR);
    }

    json_object *obj = json_object_new_object();
    if (requestId != NULL)
    {
        json_object *value = json_object_new_string(requestId);
        json_object_object_add(obj, "id", value);
    }

    json_object *method = json_object_new_string("commission");
    json_object_object_add(obj, "method", method);

    json_object *params = json_object_new_array();
    json_object *data = json_object_new_object();
    json_object *dataObj = json_object_new_object();

    json_object *deviceTypeValue = json_object_new_string(deviceType);
    json_object *physicalIdValue = json_object_new_string(physicalId);
    json_object_object_add(data, "deviceType", deviceTypeValue);
    json_object_object_add(data, "physicalId", physicalIdValue);

    json_object_object_add(dataObj, "data", data);
    json_object_array_add(params, dataObj);

    json_object_object_add(obj, "params", params);

    const char *str = json_object_to_json_string_ext(obj, JSON_C_TO_STRING_PLAIN);

    strcpy(buffer, str);

    json_object_put(obj);

    FUNC_EXIT_RC(SUCCESS);
}

/**
 * @brief Unmarshall a commissioning response 
 * 
 * Unmarshall a commissioning response from a string stream.
 * 
 * @param[out]  deviceId    Assigned device ID.
 * @param[out]  statusCode  Commissioning status.
 * @param[out]  requestId   ID of the original request.
 * @param[in]   payload     Response payload.
 * @param[in]   payloadLen  Reponse payload's length.
 * 
 * @return Zero on success, a negative value otherwise 
 */
IoT_Error_t commissioning_response(char *deviceId, uint16_t *statusCode, char *requestId, char *payload, uint16_t payloadLen)
{
    if (payload == NULL || payloadLen == 0)
    {
        FUNC_EXIT_RC(SUCCESS);
    }

    json_tokener *tok = json_tokener_new();

    json_object *obj = json_tokener_parse_ex(tok, payload, payloadLen);
    if (obj == NULL)
    {
        FUNC_EXIT_RC(JSON_PARSE_ERROR);
    }

    if (requestId != NULL)
    {
        const char *requestIdValue = json_object_get_string(json_object_object_get(obj, "id"));
        if (requestIdValue != NULL)
        {
            strcpy(requestId, requestIdValue);
        }
    }

    json_object *result = json_object_object_get(obj, "result");

    if (result != NULL)
    {
        if (statusCode != NULL)
        {
            *statusCode = json_object_get_int(json_object_object_get(result, "statusCode"));
        }

        if (deviceId != NULL)
        {
            const char *deviceIdValue = json_object_get_string(json_object_object_get(result, "deviceId"));
            if (deviceIdValue != NULL)
            {
                strcpy(deviceId, deviceIdValue);
            }
        }
    }

    json_tokener_free(tok);
    json_object_put(obj);

    FUNC_EXIT_RC(SUCCESS);
}

/**
 * @brief Marshal a command response
 * 
 * Marshal a command response into a JSON string.
 * 
 * @param[out]  buffer           Pointer to a string buffer to write to.
 * @param[in]   requestId        Command request's ID.
 * @param[in]   statusCode       Command's response status.
 * @param[in]   isErrorResponse  Signals if a command responds with an error.
 * @param[in]   errorMessage     Response error message. Only used if isErrorResponse is set to true.
 * @param[in]   body             Command response body.
 * 
 * @return Zero on success, a negative value otherwise 
 */
IoT_Error_t command_response(char *buffer, const char *requestId, uint16_t statusCode, bool isErrorResponse, char *errorMessage, json_object *body)
{
    json_object *obj = json_object_new_object();
    if (requestId != NULL)
    {
        json_object *value = json_object_new_string(requestId);
        json_object_object_add(obj, "id", value);
    }

    json_object *statusCodeValue = json_object_new_int(statusCode);

    if (isErrorResponse)
    {
        json_object *error = json_object_new_object();
        json_object_object_add(error, "statusCode", statusCodeValue);
        if (errorMessage != NULL)
        {
            json_object *errorMessageValue = json_object_new_string(errorMessage);
            json_object_object_add(error, "message", errorMessageValue);
        }

        json_object_object_add(obj, "error", error);
    }
    else
    {
        json_object *result = json_object_new_object();
        json_object_object_add(result, "statusCode", statusCodeValue);

        if (body != NULL)
        {
            json_object_object_add(result, "body", body);
        }

        json_object_object_add(obj, "result", result);
    }

    const char *str = json_object_to_json_string_ext(obj, JSON_C_TO_STRING_PLAIN);

    strcpy(buffer, str);

    json_object_put(obj);

    FUNC_EXIT_RC(SUCCESS);
}

/**
 * @brief Unmarshall a command request payload 
 * 
 * Unmarshall a command request from a string payload. 
 * 
 * @param[out]  requestId   ID of the original request.
 * @param[out]  method      Command method.
 * @param[out]  params      Command request parameters.
 * @param[in]   payload     Response payload.
 * @param[in]   payloadLen  Reponse payload's length.
 * 
 * @return Zero on success, a negative value otherwise 
 */
IoT_Error_t command_request(char *requestId, char *method, json_object **params, const char *payload, const unsigned int payloadLen)
{
    if (payload == NULL || payloadLen == 0)
    {
        FUNC_EXIT_RC(SUCCESS);
    }

    json_tokener *tok = json_tokener_new();

    json_object *obj = json_tokener_parse_ex(tok, payload, payloadLen);
    if (obj == NULL)
    {
        FUNC_EXIT_RC(JSON_PARSE_ERROR);
    }

    if (requestId != NULL)
    {
        const char *requestIdValue = json_object_get_string(json_object_object_get(obj, "id"));
        if (requestIdValue != NULL)
        {
            strcpy(requestId, requestIdValue);
        }
    }

    if (method != NULL)
    {
        const char *methodValue = json_object_get_string(json_object_object_get(obj, "method"));
        if (methodValue != NULL)
        {
            strcpy(method, methodValue);
        }
    }

    json_object *pParams = json_object_object_get(obj, "params");

    if (pParams != NULL && params != NULL)
    {
        const int rc = json_object_deep_copy(pParams, params, NULL);
        if (rc < 0)
        {
            FUNC_EXIT_RC(JSON_PARSE_ERROR);
        }
    }

    json_tokener_free(tok);
    json_object_put(obj);

    FUNC_EXIT_RC(SUCCESS);
}

/**
 * @brief Marshal a service request
 * 
 * Marshal a service request to a JSON string.
 * 
 * @param[out]  buffer     Pointer to a string buffer to write to.
 * @param[in]   requestId  Unique ID for the request.
 * @param[in]   method     Service method to request.
 * @param[in]   params     Service request parameters.
 * 
 * @return Zero on success, a negative value otherwise 
 */
IoT_Error_t service_request(char *buffer, const char *requestId, const char *method, json_object *params)
{
    json_object *obj = json_object_new_object();
    if (requestId != NULL)
    {
        json_object *value = json_object_new_string(requestId);
        json_object_object_add(obj, "id", value);
    }

    json_object *methodValue = json_object_new_string(method);
    json_object_object_add(obj, "method", methodValue);

    if (params != NULL)
    {
        json_object_object_add(obj, "params", params);
    }

    const char *str = json_object_to_json_string_ext(obj, JSON_C_TO_STRING_PLAIN);

    strcpy(buffer, str);

    json_object_put(obj);

    FUNC_EXIT_RC(SUCCESS);
}

/**
 * @brief Unmarshall a service response payload 
 * 
 * Unmarshall a service response from a string payload. 
 * 
 * @param[out]  requestId   Request ID of the original request.
 * @param[out]  statusCode  Request's status.
 * @param[out]  data        Response's data.
 * @param[in]   payload     Response payload.
 * @param[in]   payloadLen  Reponse payload's length.
 * 
 * @return Zero on success, a negative value otherwise 
 */
IoT_Error_t service_response(char *requestId, uint16_t *statusCode, json_object **data, const char *payload, const unsigned int payloadLen)
{
    if (payload == NULL || payloadLen == 0)
    {
        FUNC_EXIT_RC(SUCCESS);
    }

    json_tokener *tok = json_tokener_new();

    json_object *obj = json_tokener_parse_ex(tok, payload, payloadLen);
    if (obj == NULL)
    {
        FUNC_EXIT_RC(JSON_PARSE_ERROR);
    }

    if (requestId != NULL)
    {
        const char *requestIdValue = json_object_get_string(json_object_object_get(obj, "id"));
        if (requestIdValue != NULL)
        {

            strcpy(requestId, requestIdValue);
        }
    }

    json_object *result = json_object_object_get(obj, "result");

    if (result != NULL)
    {
        if (statusCode != NULL)
        {
            *statusCode = json_object_get_int(json_object_object_get(result, "statusCode"));
        }

        json_object *body = json_object_object_get(result, "body");
        if (body != NULL && data != NULL)
        {
            const int rc = json_object_deep_copy(body, data, NULL);
            if (rc < 0)
            {
                FUNC_EXIT_RC(JSON_PARSE_ERROR);
            }
        }
    }

    json_tokener_free(tok);
    json_object_put(obj);

    FUNC_EXIT_RC(SUCCESS);
}

/**
 * @brief Send a command response.
 * 
 * Publish a command response to MQTT.
 * 
 * @param[in]  client           AWS IoT MQTT Client instance.
 * @param[in]  deviceId         Device's ID.
 * @param[in]  commandId        ID of the requested command.
 * @param[in]  statusCode       Command's status code.
 * @param[in]  isErrorResponse  Signals if a command responds with an error.
 * @param[in]  errorMessage     Response error message. Only used if isErrorResponse is set to true.
 * @param[in]  body             Command response body.
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t send_command_response(AWS_IoT_Client *client, const char *deviceId, const char *commandId, uint16_t statusCode, bool isErrorResponse, char *errorMessage, json_object *body)
{
    char topic[MAX_TOPIC_LENGTH];

    IoT_Error_t rc = command_response_topic(topic, deviceId, commandId);
    if (rc != SUCCESS)
    {
        FUNC_EXIT_RC(rc);
    }

    char payload[MAX_JSON_TOKEN_EXPECTED];

    rc = command_response(payload, commandId, statusCode, isErrorResponse, errorMessage, body);

    if (rc != SUCCESS)
    {
        FUNC_EXIT_RC(rc);
    }

    IoT_Publish_Message_Params params;
    params.qos = QOS0;
    params.isRetained = false;
    params.payload = (void *)payload;
    params.payloadLen = strlen(payload);

    return aws_iot_mqtt_publish(client, topic, strlen(topic), &params);
}

/**
 * @brief Send commissioning request
 * 
 * Publish a commissioning request to MQTT.
 * 
 * @param[in]  client      AWS IoT MQTT Client instance.
 * @param[in]  requestId   Unique ID for the request.
 * @param[in]  deviceType  Devices's device type.
 * @param[in]  physicalId  Device's physical ID.
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t send_commissioning_request(AWS_IoT_Client *client, const char *requestId, const char *deviceType, const char *physicalId)
{
    char topic[MAX_TOPIC_LENGTH];

    IoT_Error_t rc = commission_request_topic(topic, deviceType, physicalId);
    if (rc != SUCCESS)
    {
        FUNC_EXIT_RC(rc);
    }

    char payload[MAX_JSON_TOKEN_EXPECTED];

    rc = commissioning_request(payload, requestId, deviceType, physicalId);

    if (rc != SUCCESS)
    {
        FUNC_EXIT_RC(rc);
    }

    IoT_Publish_Message_Params params;
    params.qos = QOS0;
    params.isRetained = false;
    params.payload = (void *)payload;
    params.payloadLen = strlen(payload);

    return aws_iot_mqtt_publish(client, topic, strlen(topic), &params);
}

/**
 * @brief Send service request
 * 
 * Publish a service request to MQTT.
 * 
 * @param[in]  client     AWS IoT MQTT Client instance.
 * @param[in]  requestId  Unique ID for the request.
 * @param[in]  deviceId   Devices's ID.
 * @param[in]  method     Service method to request.
 * @param[in]  params     Service request parameters.
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t send_service_request(AWS_IoT_Client *client, const char *requestId, const char *deviceId, const char *method, json_object *reqParams)
{
    char topic[MAX_TOPIC_LENGTH];

    IoT_Error_t rc = service_request_topic(topic, deviceId);

    if (rc != SUCCESS)
    {
        FUNC_EXIT_RC(rc);
    }

    char payload[MAX_JSON_TOKEN_EXPECTED];

    rc = service_request(payload, requestId, method, reqParams);

    if (rc != SUCCESS)
    {
        FUNC_EXIT_RC(rc);
    }

    IoT_Publish_Message_Params params;
    params.qos = QOS0;
    params.isRetained = false;
    params.payload = (void *)payload;
    params.payloadLen = strlen(payload);

    return aws_iot_mqtt_publish(client, topic, strlen(topic), &params);
}

/**
 * @brief Subscribe to commissioning respones
 * 
 * @param[in]  client          AWS IoT MQTT Client instance.
 * @param[in]  requestId       Unique ID of a request.
 * @param[in]  deviceType      Devices's device type.
 * @param[in]  physicalId      Device's physical ID.
 * @param[in]  handler         Subscription response handler.
 * @param[in]  subscriberData  Data blob to be passed to the subscription handler on invoke.
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t subscribe_to_commissioning_response(AWS_IoT_Client *client, const char *requestId, const char *deviceType, const char *physicalId, pApplicationHandler_t handler, void *subscribeData)
{
    IoT_Error_t rc = commission_response_topic(COMMISSIONING_RESPONSE_TOPIC_BUFFER, deviceType, physicalId, requestId);

    if (rc != SUCCESS)
    {
        FUNC_EXIT_RC(rc);
    }

    return aws_iot_mqtt_subscribe(client, COMMISSIONING_RESPONSE_TOPIC_BUFFER, strlen(COMMISSIONING_RESPONSE_TOPIC_BUFFER), QOS0, handler, subscribeData);
}

/**
 * @brief Subscribe to command requests
 * 
 * @param[in]  client          AWS IoT MQTT Client instance.
 * @param[in]  deviceId        Device's ID.
 * @param[in]  handler         Subscription response handler.
 * @param[in]  subscriberData  Data blob to be passed to the subscription handler on invoke.
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t subscribe_to_command_request(AWS_IoT_Client *client, const char *deviceId, pApplicationHandler_t handler, void *subscribeData)
{
    IoT_Error_t rc = command_request_topic(COMMAND_TOPIC_BUFFER, deviceId);

    if (rc != SUCCESS)
    {
        FUNC_EXIT_RC(rc);
    }

    return aws_iot_mqtt_subscribe(client, COMMAND_TOPIC_BUFFER, strlen(COMMAND_TOPIC_BUFFER), QOS0, handler, subscribeData);
}

/**
 * @brief Subscribe to service responses
 * 
 * @param[in]  client          AWS IoT MQTT Client instance.
 * @param[in]  deviceId        Device's ID.
 * @param[in]  requestId       Unique ID of a request.
 * @param[in]  handler         Subscription response handler.
 * @param[in]  subscriberData  Data blob to be passed to the subscription handler on invoke.
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t subscribe_to_service_response(AWS_IoT_Client *client, const char *deviceId, const char *requestId, pApplicationHandler_t handler, void *subscribeData)
{
    IoT_Error_t rc = service_response_topic(SERVICE_RESPONSE_TOPIC_BUFFER, deviceId, requestId);

    if (rc != SUCCESS)
    {
        FUNC_EXIT_RC(rc);
    }

    return aws_iot_mqtt_subscribe(client, SERVICE_RESPONSE_TOPIC_BUFFER, strlen(SERVICE_RESPONSE_TOPIC_BUFFER), QOS0, handler, subscribeData);
}

/**
 * @brief Initialize an AWS IoT Client 
 * 
 * Initialize an AWS IoT Client instance with ThinCloud specific parameters and defaults.
 * 
 * @param[in]  client          AWS IoT MQTT Client instance.
 * @param[in]  hostAddr        ThinCloud host address. 
 * @param[in]  rootCAPath      Path to a root CA.
 * @param[in]  clientCRTPath   Path to a client CRT.
 * @param[in]  clientKeyPath   Path to a client private key.
 * @param[in]  handler         Disconnect handler.
 * @param[in]  disconnectData  Data blob to be passed to the disconnect handler on invoke.
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t tc_init(AWS_IoT_Client *client, char *hostAddr, char *rootCAPath, char *clientCRTPath, char *clientKeyPath, iot_disconnect_handler handler, void *disconnectData)
{
    IoT_Client_Init_Params params = iotClientInitParamsDefault;

    params.enableAutoReconnect = false; // We enable this on connect
    params.pHostURL = hostAddr;
    params.port = 443; // Default MQTT port
    params.pRootCALocation = rootCAPath;
    params.pDeviceCertLocation = clientCRTPath;
    params.pDevicePrivateKeyLocation = clientKeyPath;
    params.mqttCommandTimeout_ms = 20000;
    params.tlsHandshakeTimeout_ms = 5000;
    params.isSSLHostnameVerify = true;
    params.disconnectHandler = handler;
    params.disconnectHandlerData = disconnectData;

    return aws_iot_mqtt_init(client, &params);
}

/**
 * @brief Start MQTT connection to ThinCloud host
 * 
 * Initialize an MQTT conenction to a thincloud host.
 * 
 * @param[in]  client         AWS IoT MQTT Client instance.
 * @param[in]  clientId       An unique ID for the client instance.
 * @param[in]  autoReconnect  Signals if the MQTT client should attempt to auto-reconnect
 *                            after connection failures.
 * 
 * @return Zero on success, negative value otherwise 
 */
IoT_Error_t tc_connect(AWS_IoT_Client *client, char *clientId, bool autoReconnect)
{
    IoT_Client_Connect_Params params = iotClientConnectParamsDefault;

    params.keepAliveIntervalInSec = 600;
    params.isCleanSession = true;
    params.MQTTVersion = MQTT_3_1_1;
    params.pClientID = clientId;
    params.clientIDLen = (uint16_t)strlen(clientId);
    params.isWillMsgPresent = false;

    IoT_Error_t rc = aws_iot_mqtt_connect(client, &params);
    if (rc != SUCCESS)
    {
        FUNC_EXIT_RC(rc);
    }

    if (autoReconnect)
    {
        rc = aws_iot_mqtt_autoreconnect_set_status(client, true);
        if (rc != SUCCESS)
        {
            FUNC_EXIT_RC(rc);
        }
    }

    FUNC_EXIT_RC(SUCCESS);
}

#endif /* THINCLOUD_EMBEDDED_C_SDK_ */
