#ifndef SYS_PARAM_H
#define SYS_PARAM_H
#include <sys/sysinfo.h>
#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include "../include/tuya_parse_command.h"
#include "../include/json_formatting.h"

int ubus_toggle_pin(char *port, int pin, char *command, char *message_for_tuya);
int ubus_get_pin_info(char *port, int pin, char *sensor, char *model, char *message_for_tuya);
int ubus_get_devices(char *message_for_tuya);
void cleanup_ubus_context();

#endif