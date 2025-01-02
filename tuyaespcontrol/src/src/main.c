#include "../include/argp_parse.h"
#include "../include/daemon.h"
#include "../include/ubus_communication.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#include "../include/tuya_functions.h"

#include <syslog.h>

void sig_handler(int signum){
    cleanup_ubus_context();
    int disc = disconnect();
    if (disc == 0)
        syslog(LOG_USER | LOG_INFO, "Disconnected gracefully");
    else
        syslog(LOG_USER | LOG_ERR, "Disconnecting failed");
        closelog();
    exit(1);
}

void set_signal_handlers(){
    signal(SIGINT,sig_handler);
    signal(SIGHUP,sig_handler);
    signal(SIGQUIT,sig_handler);
    signal(SIGILL,sig_handler);
    signal(SIGTRAP,sig_handler);
    signal(SIGABRT,sig_handler);
    signal(SIGFPE,sig_handler);
    signal(SIGTERM,sig_handler);
}


int main(int argc, char **argv)
{
    int ret;
    const char *LOGNAME = "tuyad";
    int cnt = 0;

    openlog("tuyadevicemonitor", LOG_PID | LOG_CONS, LOG_DAEMON);
    set_signal_handlers();
    struct arguments arguments;
    int parse_result = parse_arguments(argc, argv, &arguments);
    if (parse_result != 0) {
        syslog(LOG_USER | LOG_ERR, "Init parsing failed");
        closelog();
        cleanup_ubus_context();
        return EXIT_FAILURE;
    }
    // turn this process into a daemon
    if (arguments.daemonize == 1) {
        syslog(LOG_USER | LOG_INFO, "tuya daemon starting1");
        ret = become_daemon(0);
        if(ret)
        {
            syslog(LOG_USER | LOG_ERR, "error starting");
            closelog();
            cleanup_ubus_context();
            return EXIT_FAILURE;
        }
    }

    // we are now a daemon!

    syslog(LOG_USER | LOG_INFO, "tuya daemon starting");

    ret = connect_to_tuya_cloud(arguments.device_id, arguments.secret);
    if (ret != OPRT_OK) {
        syslog(LOG_USER | LOG_ERR, "Failed to connect to Tuya cloud. Error: %d", ret);
        closelog();
        cleanup_ubus_context();
        disconnect();
        return EXIT_FAILURE;
    }

   while(1){
        ret = send_data_to_cloud();
        if (ret == OPRT_NETWORK_ERROR) {
            syslog(LOG_USER | LOG_INFO, "%d", ret);
            syslog(LOG_USER | LOG_INFO, "Failed to send data to cloud. Disconnecting...");
            disconnect();
            cleanup_ubus_context();
            return EXIT_FAILURE;
        } else if (ret != OPRT_OK) {
            syslog(LOG_USER | LOG_WARNING, "Error sending data. Retrying...");
            sleep(5);
        } else if (ret == OPRT_OK) {
           sleep(10);
        }
    }

    cleanup_ubus_context();
    disconnect();
    return EXIT_SUCCESS;

}