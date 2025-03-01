#ifndef TUYA_PARSE_COMMAND_H
#define TUYA_PARSE_COMMAND_H

#include "cJSON.h"
#include "time.h"
#include <stdio.h>
#include "syslog.h"
#include <string.h>

int parse_pin_and_port(cJSON *inputParamsObj, char *port, size_t port_size, int *pin);
int parse_sensor_info(cJSON *inputParamsObj, char *sensor, char *model, size_t sensor_size, size_t model_size);

#endif