#include "../include/argp_parse.h"
#include "../include/daemon.h"
#include "../include/ubus_communication.h"
#include "../include/tuya_parse_command.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <syslog.h>

#include "mqtt_client_interface.h"
#include "tuyalink_core.h"
#include "cJSON.h"

#include "../include/tuya_functions.h"

#define OPRT_RETRYING -3

const static char tuya_cacert1[] = {\
"-----BEGIN CERTIFICATE-----\n"\
"MIIDxTCCAq2gAwIBAgIBADANBgkqhkiG9w0BAQsFADCBgzELMAkGA1UEBhMCVVMx\n"\
"EDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNjb3R0c2RhbGUxGjAYBgNVBAoT\n"\
"EUdvRGFkZHkuY29tLCBJbmMuMTEwLwYDVQQDEyhHbyBEYWRkeSBSb290IENlcnRp\n"\
"ZmljYXRlIEF1dGhvcml0eSAtIEcyMB4XDTA5MDkwMTAwMDAwMFoXDTM3MTIzMTIz\n"\
"NTk1OVowgYMxCzAJBgNVBAYTAlVTMRAwDgYDVQQIEwdBcml6b25hMRMwEQYDVQQH\n"\
"EwpTY290dHNkYWxlMRowGAYDVQQKExFHb0RhZGR5LmNvbSwgSW5jLjExMC8GA1UE\n"\
"AxMoR28gRGFkZHkgUm9vdCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkgLSBHMjCCASIw\n"\
"DQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL9xYgjx+lk09xvJGKP3gElY6SKD\n"\
"E6bFIEMBO4Tx5oVJnyfq9oQbTqC023CYxzIBsQU+B07u9PpPL1kwIuerGVZr4oAH\n"\
"/PMWdYA5UXvl+TW2dE6pjYIT5LY/qQOD+qK+ihVqf94Lw7YZFAXK6sOoBJQ7Rnwy\n"\
"DfMAZiLIjWltNowRGLfTshxgtDj6AozO091GB94KPutdfMh8+7ArU6SSYmlRJQVh\n"\
"GkSBjCypQ5Yj36w6gZoOKcUcqeldHraenjAKOc7xiID7S13MMuyFYkMlNAJWJwGR\n"\
"tDtwKj9useiciAF9n9T521NtYJ2/LOdYq7hfRvzOxBsDPAnrSTFcaUaz4EcCAwEA\n"\
"AaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYwHQYDVR0OBBYE\n"\
"FDqahQcQZyi27/a9BUFuIMGU2g/eMA0GCSqGSIb3DQEBCwUAA4IBAQCZ21151fmX\n"\
"WWcDYfF+OwYxdS2hII5PZYe096acvNjpL9DbWu7PdIxztDhC2gV7+AJ1uP2lsdeu\n"\
"9tfeE8tTEH6KRtGX+rcuKxGrkLAngPnon1rpN5+r5N9ss4UXnT3ZJE95kTXWXwTr\n"\
"gIOrmgIttRD02JDHBHNA7XIloKmf7J6raBKZV8aPEjoJpL1E/QYVN8Gb5DKj7Tjo\n"\
"2GTzLH4U/ALqn83/B2gX2yKQOC16jdFU8WnjXzPKej17CuPKf1855eJ1usV2GDPO\n"\
"LPAvTK33sefOT6jEm0pUBsV/fdUID+Ic/n4XuKxe9tQWskMJDE32p2u0mYRlynqI\n"\
"4uJEvlz36hz1\n"\
"-----END CERTIFICATE-----\n"};

static const int max_attempts = 10;
static int attempt_cnt = 0;
int get_action();
int handle_action(char *actionCode, cJSON *action_data);

tuya_mqtt_context_t client_instance;


void on_connected(tuya_mqtt_context_t* context, void* user_data)
{
    syslog(LOG_USER | LOG_INFO, "Connected!");
    tuyalink_thing_data_model_get(context, NULL);
}

void on_disconnect(tuya_mqtt_context_t* context, void* user_data)
{
    syslog(LOG_USER | LOG_INFO, "Disconnected!");
}

void on_messages(tuya_mqtt_context_t* context, void* user_data, const tuyalink_message_t* msg)
{
    switch (msg->type) {

        case THING_TYPE_ACTION_EXECUTE:
            ;
            int err = 0;
            char *actionCode = NULL;
            cJSON *action_data = NULL;

            err = get_action(msg, &actionCode, &action_data);
            if (err != 0) {
                syslog(LOG_USER | LOG_ERR, "Action code not valid.\n");
                return;
            }

            err = handle_action(actionCode, action_data);
            if (err != 0) {
                syslog(LOG_USER | LOG_ERR, "Action handler failed.\n");
                return;
            }

            break;

        default:
            break;
    }
    printf("\r\n");
}

int connect_to_tuya_cloud(char *deviceId, char *deviceSecret){
    int ret = OPRT_OK;

    ret = tuya_mqtt_init(&client_instance, &(const tuya_mqtt_config_t) {
        .host = "m1.tuyacn.com",
        .port = 8883,
        .cacert = tuya_cacert1,
        .cacert_len = sizeof(tuya_cacert1),
        .device_id = deviceId,
        .device_secret = deviceSecret,
        .keepalive = 100,
        .timeout_ms = 5000,
        .on_connected = on_connected,
        .on_disconnect = on_disconnect,
        .on_messages = on_messages
    });

    if (ret != OPRT_OK) {
        syslog(LOG_USER | LOG_ERR, "Failed to initialize.\n");
        return ret;
    }
    ret = tuya_mqtt_connect(&client_instance);
    if (ret != OPRT_OK) {
        syslog(LOG_USER | LOG_ERR, "Failed to connect to cloud.");
        tuya_mqtt_deinit(&client_instance);
        return ret;
    }

    return OPRT_OK;
}

int get_action(const tuyalink_message_t* msg, char **actionCode, cJSON **action_data)
{
    if (msg->data_string == NULL)
        return -1;

    *action_data = cJSON_Parse(msg->data_string);
    if (*action_data == NULL)
        return -2;

    *actionCode = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(*action_data, "actionCode"));
    if (*actionCode == NULL) {
        cJSON_Delete(*action_data);
        return -3;
    }

    syslog(LOG_USER | LOG_INFO, "action code: %s", *actionCode);
    return 0;
}

int handle_action(char *actionCode, cJSON *action_data)
{
    char message_for_tuya[512] = {0};
    char port[51] = {0};
    int pin = 0;
    int err = 0;

    if (strcmp(actionCode, "devices") == 0) {

        err = ubus_get_devices(message_for_tuya);

    } else if (strcmp(actionCode, "on") == 0 || strcmp(actionCode, "off") == 0 || strcmp(actionCode, "get") == 0) {
        
        cJSON *inputParamsObj = cJSON_GetObjectItemCaseSensitive(action_data, "inputParams");
        err = parse_pin_and_port(inputParamsObj, port, sizeof(port), &pin);
        if (err != 0) {
            syslog(LOG_USER | LOG_ERR, "Error parsing pin and port. Error code: %d", err);
            cJSON_Delete(action_data);
            return err;
        }

        if (strcmp(actionCode, "get") == 0) {
            char sensor[51] = {0};
            char model[51] = {0};
            err = parse_sensor_info(inputParamsObj, sensor, model, sizeof(sensor), sizeof(model));
            if (err != 0) {
                syslog(LOG_USER | LOG_ERR, "Error parsing pin and port. Error code: %d", err);
                cJSON_Delete(action_data);
                return err;
            }

            err = ubus_get_pin_info(port, pin, sensor, model, message_for_tuya);
        } else {
            err = ubus_toggle_pin(port, pin, (char *)actionCode, message_for_tuya);
        }

    }

    if (err != 0) {
        syslog(LOG_USER | LOG_ERR, "Failed to invoke ubus with actionCode: %s, Error: %d", actionCode, err);
        cJSON_Delete(action_data);
        err = tuyalink_thing_property_report(&client_instance, NULL, message_for_tuya);
        return err;
    } else {
        syslog(LOG_INFO, "Ubus invocation succeeded. Sending to Tuya Cloud: %s", message_for_tuya);
        err = tuyalink_thing_property_report(&client_instance, NULL, message_for_tuya);
        syslog(LOG_INFO, "Reporting code: %d", err);
    }


    cJSON_Delete(action_data);
    return err;

}

int send_data_to_cloud(){
    int ret = OPRT_OK;
    if (!tuya_mqtt_connected(&client_instance)){
        attempt_cnt++;
        tuya_mqtt_loop(&client_instance);
        if (attempt_cnt > max_attempts) {
            syslog(LOG_USER | LOG_ERR, "Max retry attempts reached, time out.");
            return OPRT_NETWORK_ERROR;
        }
        return OPRT_RETRYING;
    }
    ret = tuya_mqtt_loop(&client_instance);
     if (ret != OPRT_OK) {
        syslog(LOG_USER | LOG_ERR, "Error in MQTT loop: %d", ret);
        return ret;
    }

    attempt_cnt = 0;
    
    return ret;
}

int disconnect(){
    if (tuya_mqtt_disconnect(&client_instance) != OPRT_OK) {
        syslog(LOG_USER | LOG_ERR, "Error disconnecting from Tuya cloud.");
    }
    if (tuya_mqtt_deinit(&client_instance) != OPRT_OK) {
        syslog(LOG_USER | LOG_ERR, "Error deinitializing MQTT client.");
        return 1;
    }
    return 0;
}