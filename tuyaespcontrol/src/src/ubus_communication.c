#include "../include/ubus_communication.h"


static struct ubus_context *ctx = NULL;

static void get_response_message(struct ubus_request *req, int type, struct blob_attr *msg) {
    char *message_for_tuya = (char *)req->priv;
    if (!msg) {
        syslog(LOG_ERR, "No response received from UBUS.");
        snprintf(message_for_tuya, 256, "{\"error\": \"No response received\"}");
        return;
    }

    char *str = blobmsg_format_json(msg, true);
    if (str) {
        syslog(LOG_INFO, "UBUS response: %s", str);
        snprintf(message_for_tuya, 256, "%s", str);
        free(str);
    } else {
        syslog(LOG_ERR, "Failed to format UBUS response as JSON.");
        snprintf(message_for_tuya, 256, "{\"error\": \"Failed to format response\"}");
    }
}

static void get_response_message_pin_info(struct ubus_request *req, int type, struct blob_attr *msg) {
    char *message_for_tuya = (char *)req->priv;
    if (!msg) {
        syslog(LOG_ERR, "No response received from ubus.");
        snprintf(message_for_tuya, 256, "{\"error\": \"No response received\"}");
        return;
    }

    char *str = blobmsg_format_json(msg, true);
    if (str) {
        syslog(LOG_INFO, "Ubus response (raw): %s", str); 
        get_pin_info_format_message(message_for_tuya, str);

    } else {
        syslog(LOG_ERR, "Failed to format ubus response as JSON.");
        snprintf(message_for_tuya, 512, "{\"error\": \"Failed to format response\"}");
    }
}

int ubus_toggle_pin(char *port, int pin, char *command, char *message_for_tuya)
{
    uint32_t id;
    int rc = 0;
    struct blob_buf b = {};

    if (!ctx) {
        ctx = ubus_connect(NULL);
        if (!ctx) {
            return -1;
        }
    }
    
    if ((rc = ubus_lookup_id(ctx, "esp", &id))) {
        syslog(LOG_ERR, "Failed to find 'esp' object. ID not found.");
        return -2;
    }

    blob_buf_init(&b, 0);
    blobmsg_add_string(&b, "port", port);
    blobmsg_add_u32(&b, "pin", pin);

    if ((rc = ubus_invoke(ctx, id, command, b.head, get_response_message, message_for_tuya, 5000))) {
        syslog(LOG_ERR, "Failed to invoke the method '%s' of object '%s'. Error code: %d, Description: %s", command, "esp", rc, ubus_strerror(rc));
        blob_buf_free(&b);
        return -3;
    }

    blob_buf_free(&b);

    return 0;
}

int ubus_get_pin_info(char *port, int pin, char *sensor, char *model, char *message_for_tuya)
{
    uint32_t id;
    int rc = 0;
    struct blob_buf b = {};

    if (!ctx) {
        ctx = ubus_connect(NULL);
        if (!ctx) {
            return -1;
        }
    }
    
    if ((rc = ubus_lookup_id(ctx, "esp", &id))) {
        syslog(LOG_ERR, "Failed to find 'esp' object. ID not found.");
        return -2;
    }

    blob_buf_init(&b, 0);
    blobmsg_add_string(&b, "port", port);
    blobmsg_add_u32(&b, "pin", pin);
    blobmsg_add_string(&b, "sensor", sensor);
    blobmsg_add_string(&b, "model", model);

    if ((rc = ubus_invoke(ctx, id, "get", b.head, get_response_message_pin_info, message_for_tuya, 5000))) {
        syslog(LOG_ERR, "Failed to invoke the method '%s' of object '%s'. Error code: %d, Description: %s", "get", "esp", rc, ubus_strerror(rc));
        blob_buf_free(&b);
        return -3;
    }

    blob_buf_free(&b);

    return 0;
}
int ubus_get_devices(char *message_for_tuya)
{
    uint32_t id;
    int rc = 0;

    if (!ctx) {
        ctx = ubus_connect(NULL);
        if (!ctx) {
            return -1;
        }
    }
    
    if ((rc = ubus_lookup_id(ctx, "esp", &id))) {
        syslog(LOG_ERR, "Failed to find 'esp' object. ID not found.");
        return -2;
    }

    if ((rc = ubus_invoke(ctx, id, "devices", NULL, get_response_message, message_for_tuya, 5000))) {
        syslog(LOG_ERR, "Failed to invoke the method '%s' of object '%s'. Error code: %d, Description: %s", "devices", "esp", rc, ubus_strerror(rc));
        return -3;
    }

    return 0;
}

void cleanup_ubus_context() {
    if (ctx) {
        ubus_free(ctx);
        ctx = NULL;
    }
}