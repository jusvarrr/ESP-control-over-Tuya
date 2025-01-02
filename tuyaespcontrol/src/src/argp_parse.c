#include "../include/argp_parse.h"

const char *argp_program_version =
    "argp-ex3 1.0";
const char *argp_program_bug_address =
    "<bug-gnu-utils@gnu.org>";

static char doc[] =
    "Argp example #3 -- a program with options and "
    "arguments using argp for parsing device";

static char args_doc[] = "";

static struct argp_option options[] = {
    {"device_id", 'd', "ID", 0, "Specify device ID"},
    {"secret", 's', "SECRET", 0, "Specify device secret"},
    {"product_id", 'p', "PRODUCT", 0, "Specify product ID"},
    {"daemonize", 'D', 0, 0, "Run as daemon"},
    {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
    case 'd':
        strncpy(arguments->device_id, arg, sizeof(arguments->device_id) - 1);
        arguments->device_id[sizeof(arguments->device_id) - 1] = '\0';  // Ensure null termination
        break;
    case 's':
        strncpy(arguments->secret, arg, sizeof(arguments->secret) - 1);
        arguments->secret[sizeof(arguments->secret) - 1] = '\0';  // Ensure null termination
        break;
    case 'p':
        strncpy(arguments->product_id, arg, sizeof(arguments->product_id) - 1);
        arguments->product_id[sizeof(arguments->product_id) - 1] = '\0';  // Ensure null termination
        break;
    case 'D':
        arguments->daemonize = 1;
        break;

    case ARGP_KEY_ARG:
        if (state->arg_num >= 4)
            return ARGP_ERR_UNKNOWN;
        break;

    case ARGP_KEY_END:
        if (strcmp(arguments->device_id, "") == 0 || 
            strcmp(arguments->secret, "") == 0 || 
            strcmp(arguments->product_id, "") == 0) {
            printf("Provide all parameters (device_id, secret, product_id).\n"); //nesuprantu, kodel is sito patikrinimo vistiek grazina 0 argp_parse
            return ARGP_ERR_UNKNOWN;
        }
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

int parse_arguments(int argc, char **argv, struct arguments *args)
{
    if (args == NULL)
        return -1;

    memset(args, 0, sizeof(*args));
    args->daemonize = 0;
    if (argp_parse(&argp, argc, argv, ARGP_NO_EXIT, 0, args)!=0) {
        return -1;
    }

    if (strcmp(args->device_id, "") == 0 || 
        strcmp(args->secret, "") == 0 || 
        strcmp(args->product_id, "") == 0)
            return -1;

    return 0;
}
