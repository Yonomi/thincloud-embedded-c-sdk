#include <json-c/json.h>

#include "thincloud.h"
#include "greatest.h"

SUITE(thincloud);

TEST should_build_commission_topic(void)
{
    char topic[MAX_TOPIC_LENGTH];

    const int rc = commission_request_topic(topic, "lock", "123456");

    const char *expectedStr = "thincloud/registration/lock_123456/requests";
    const int expectedStrLen = strlen(expectedStr);

    ASSERT_STR_EQ(expectedStr, topic);
    ASSERT_EQ(expectedStrLen, rc);

    PASS();
}

TEST should_fail_commission_topic_on_null_inputs(void)
{
    char topic[MAX_TOPIC_LENGTH];

    const int rc = commission_request_topic(topic, NULL, NULL);

    ASSERT_EQ(NULL_VALUE_ERROR, rc);

    PASS();
}

TEST should_build_commission_response_topic(void)
{
    char topic[MAX_TOPIC_LENGTH];
    const int rc = commission_response_topic(topic, "lock", "123456", "123");

    const char *expectedStr = "thincloud/registration/lock_123456/requests/123/response";
    const int expectedStrLen = strlen(expectedStr);

    ASSERT_STR_EQ(expectedStr, topic);
    ASSERT_EQ(expectedStrLen, rc);

    PASS();
}

TEST should_build_command_request_topic(void)
{
    char topic[MAX_TOPIC_LENGTH];
    const int rc = command_request_topic(topic, "123456");

    const char *expectedStr = "thincloud/devices/123456/command";
    const int expectedStrLen = strlen(expectedStr);

    ASSERT_STR_EQ(expectedStr, topic);
    ASSERT_EQ(expectedStrLen, rc);

    PASS();
}

TEST should_build_command_response_topic(void)
{
    char topic[MAX_TOPIC_LENGTH];
    const int rc = command_response_topic(topic, "123456", "7890");

    const char *expectedStr = "thincloud/devices/123456/command/7890/response";
    const int expectedStrLen = strlen(expectedStr);

    ASSERT_STR_EQ(expectedStr, topic);
    ASSERT_EQ(expectedStrLen, rc);

    PASS();
}

TEST should_build_service_request_topic(void)
{
    char topic[MAX_TOPIC_LENGTH];
    const int rc = service_request_topic(topic, "123456");

    const char *expectedStr = "thincloud/devices/123456/requests";
    const int expectedStrLen = strlen(expectedStr);

    ASSERT_STR_EQ(expectedStr, topic);
    ASSERT_EQ(expectedStrLen, rc);

    PASS();
}

TEST should_build_service_response_topic(void)
{
    char topic[MAX_TOPIC_LENGTH];
    const int rc = service_response_topic(topic, "123456", "7890");

    const char *expectedStr = "thincloud/devices/123456/requests/7890/response";
    const int expectedStrLen = strlen(expectedStr);

    ASSERT_STR_EQ(expectedStr, topic);
    ASSERT_EQ(expectedStrLen, rc);

    PASS();
}

TEST should_build_commission_request(void)
{
    char buffer[256];

    const int rc = commissioning_request(buffer, "1234", "lock", "5678");

    const char *expectedStr = "{\"id\":\"1234\",\"method\":\"commission\",\"params\":{\"data\":{\"deviceType\":\"lock\",\"physicalId\":\"5678\"}}}";
    const int expectedStrLen = strlen(expectedStr);

    ASSERT_STR_EQ(expectedStr, buffer);
    ASSERT_EQ(expectedStrLen, rc);

    PASS();
}

TEST should_build_command_response(void)
{
    char buffer[256];

    json_object *body = json_object_new_object();
    json_object *bar = json_object_new_string("bar");
    json_object_object_add(body, "foo", bar);

    const int rc = command_response(buffer, "1234", 200, false, NULL, body);

    const char *expectedStr = "{\"id\":\"1234\",\"result\":{\"statusCode\":200,\"body\":{\"foo\":\"bar\"}}}";
    const int expectedStrLen = strlen(expectedStr);

    ASSERT_STR_EQ(expectedStr, buffer);
    ASSERT_EQ(expectedStrLen, rc);

    PASS();
}

TEST should_build_service_request(void)
{
    char buffer[256];

    json_object *params = json_object_new_object();
    json_object *bar = json_object_new_string("bar");
    json_object_object_add(params, "foo", bar);

    const int rc = service_request(buffer, "1234", REQUEST_METHOD_GET, params);

    const char *expectedStr = "{\"id\":\"1234\",\"method\":\"GET\",\"params\":{\"foo\":\"bar\"}}";
    const int expectedStrLen = strlen(expectedStr);

    ASSERT_STR_EQ(expectedStr, buffer);
    ASSERT_EQ(expectedStrLen, rc);

    PASS();
}

TEST should_process_commissioning_response(void)
{
    char deviceId[TC_ID_LENGTH];
    char requestId[TC_ID_LENGTH];
    uint16_t statusCode = 0;

    char *response = "{\"id\":\"1234\",\"result\":{\"statusCode\":200,\"deviceId\":\"5678\"}}";
    const int respLen = strlen(response);

    const IoT_Error_t rc = commissioning_response(deviceId, &statusCode, requestId, response, respLen);

    ASSERT_EQ_FMT(rc, SUCCESS, "%d");

    ASSERT_STR_EQ("5678", deviceId);
    ASSERT_EQ(statusCode, 200);
    ASSERT_STR_EQ("1234", requestId);

    PASS();
}

TEST should_process_command_request(void)
{
    char requestId[TC_ID_LENGTH];
    char method[64];

    json_object *params = NULL;

    const char *request = "{\"id\":\"1234\",\"method\":\"startRoutine\",\"params\":[{\"data\":{\"foo\":\"bar\"}}]}";

    const IoT_Error_t rc = command_request(requestId, method, &params, request, strlen(request));

    ASSERT_EQ_FMT(rc, SUCCESS, "%d");

    ASSERT_STR_EQ("1234", requestId);
    ASSERT_STR_EQ("startRoutine", method);

    ASSERT(params != NULL);

    json_object *foo = json_object_object_get(
        json_object_object_get(
            json_object_array_get_idx(params, 0),
            "data"),
        "foo");

    ASSERT(foo != NULL);
    ASSERT_STR_EQ("bar", json_object_get_string(foo));

    json_object_put(foo);

    PASS();
}

TEST should_process_service_response(void)
{
    char requestId[TC_ID_LENGTH];
    uint16_t statusCode = 0;

    json_object *data = NULL;

    const char *response = "{\"id\":\"1234\",\"result\":{\"statusCode\":200,\"body\":{\"foo\":\"bar\"}}}";

    const IoT_Error_t rc = service_response(requestId, &statusCode, &data, response, strlen(response));

    ASSERT_EQ_FMT(rc, SUCCESS, "%d");

    ASSERT_STR_EQ("1234", requestId);
    ASSERT_EQ(200, statusCode);

    ASSERT(data != NULL);

    json_object *foo = json_object_object_get(data, "foo");
    ASSERT(foo != NULL);
    ASSERT_STR_EQ("bar", json_object_get_string(foo));

    json_object_put(foo);

    PASS();
}

SUITE(tc_topics)
{
    RUN_TEST(should_build_commission_topic);
    RUN_TEST(should_fail_commission_topic_on_null_inputs);
    RUN_TEST(should_build_commission_response_topic);
    RUN_TEST(should_build_command_request_topic);
    RUN_TEST(should_build_command_response_topic);
    RUN_TEST(should_build_service_request_topic);
    RUN_TEST(should_build_service_response_topic);
}

SUITE(tc_unmarshal)
{
    RUN_TEST(should_build_commission_request);
    RUN_TEST(should_build_command_response);
    RUN_TEST(should_build_service_request);
}

SUITE(tc_marshal)
{
    RUN_TEST(should_process_commissioning_response);
    RUN_TEST(should_process_command_request);
    RUN_TEST(should_process_service_response);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(tc_topics);
    RUN_SUITE(tc_unmarshal);
    RUN_SUITE(tc_marshal);

    GREATEST_MAIN_END();
}
