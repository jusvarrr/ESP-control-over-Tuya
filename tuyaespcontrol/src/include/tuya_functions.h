#ifndef TUYA_FUNCTIONS_DEF
#define TUYA_FUNCTIONS_DEF
#include "syslog.h"
#include "../include/json_formatting.h"
#include "tuya_error_code.h"

int connect_to_tuya_cloud(char *deviceId, char *deviceSecret);
int send_data_to_cloud();
int disconnect();

#endif