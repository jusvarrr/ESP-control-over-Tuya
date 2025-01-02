#include "../include/tuya_parse_command.h"

int parse_pin_and_port(cJSON *inputParamsObj, char *port, size_t port_size, int *pin) {
    if (inputParamsObj == NULL) {
        syslog(LOG_USER | LOG_ERR, "Input parameters object is NULL");
        return -1;
    }

    cJSON *param_port = cJSON_GetObjectItemCaseSensitive(inputParamsObj, "port");
    cJSON *param_pin = cJSON_GetObjectItem(inputParamsObj, "pin");

    if (param_port == NULL || param_pin == NULL) {
        syslog(LOG_USER | LOG_ERR, "Missing 'port' or 'pin' in input parameters");
        return -2;
    }

    const char *port_value = cJSON_GetStringValue(param_port);
    if (port_value == NULL) {
        syslog(LOG_USER | LOG_ERR, "'port' is not a valid string");
        return -3;
    }

    strncpy(port, port_value, port_size - 1);
    port[port_size - 1] = '\0';

    if (!cJSON_IsNumber(param_pin)) {
        syslog(LOG_USER | LOG_ERR, "'pin' is not a valid number");
        return -4;
    }

    *pin = (int)param_pin->valuedouble;
    return 0;
}

int parse_sensor_info(cJSON *inputParamsObj, char *sensor, char *model, size_t sensor_size, size_t model_size)
{
    if (inputParamsObj == NULL) {
        syslog(LOG_USER | LOG_ERR, "Input parameters object is NULL");
        return -1;
    }

    cJSON *param_sensor = cJSON_GetObjectItemCaseSensitive(inputParamsObj, "sensor");
    cJSON *param_model = cJSON_GetObjectItemCaseSensitive(inputParamsObj, "model");

    if (param_sensor == NULL || param_model == NULL) {
        syslog(LOG_USER | LOG_ERR, "Missing 'sensor' or 'port' in input parameters");
        return -2;
    }

    const char *sensor_value = cJSON_GetStringValue(param_sensor);
    if (sensor_value == NULL) {
        syslog(LOG_USER | LOG_ERR, "'sensor' is not a valid string");
        return -3;
    }

    const char *model_value = cJSON_GetStringValue(param_model);
    if (model_value == NULL) {
        syslog(LOG_USER | LOG_ERR, "'model' is not a valid string");
        return -4;
    }

    strncpy(sensor, sensor_value, sensor_size - 1);
    sensor[sensor_size - 1] = '\0';
    strncpy(model, model_value, model_size - 1);
    model[model_size - 1] = '\0';
    
    return 0;
}