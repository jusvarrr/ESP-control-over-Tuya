#ifndef ARGP_PARSE_H
#define ARGP_PARSE_H

#include <argp.h>
#include <string.h>

struct arguments {
    char device_id[50];
    char secret[50];
    char product_id[50];
    int daemonize;
};

int parse_arguments(int argc, char **argv, struct arguments *args);

#endif