#include "../include/json_formatting.h"

void get_pin_info_format_message(char *message_for_tuya, char *str) {
    cJSON *json = cJSON_Parse(str);
    if (json) {
        char *parsed_json_str = cJSON_PrintUnformatted(json);
        syslog(LOG_INFO, "Parsed JSON: %s", parsed_json_str);
        free(parsed_json_str);

        cJSON *data_item = cJSON_GetObjectItem(json, "data");
        if (data_item) {
            cJSON *humidity_item = cJSON_GetObjectItem(data_item, "humidity");
            cJSON *temperature_item = cJSON_GetObjectItem(data_item, "temperature");

            if (humidity_item && temperature_item) {
                if (cJSON_IsNumber(humidity_item)) {
                    int humidity_int = (int)humidity_item->valuedouble;
                    cJSON_ReplaceItemInObject(data_item, "humidity", cJSON_CreateNumber(humidity_int));
                }

                if (cJSON_IsNumber(temperature_item)) {
                    int temperature_int = (int)temperature_item->valuedouble;
                    cJSON_ReplaceItemInObject(data_item, "temperature", cJSON_CreateNumber(temperature_int));
                }
            }
            char *modified_json = cJSON_PrintUnformatted(json);
            snprintf(message_for_tuya, 512, "%s", modified_json);

            syslog(LOG_INFO, "Modified JSON for Tuya: %s", modified_json);

            free(modified_json);
        } else {
            snprintf(message_for_tuya, 512, "%s", str);
        }
        cJSON_Delete(json);

        
    } else {
        syslog(LOG_ERR, "Failed to parse JSON: %s", str);
        snprintf(message_for_tuya, 512, "{\"error\": \"Failed to parse response\"}");
    }
    free(str);
}
