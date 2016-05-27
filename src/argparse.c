#include "argparse.h"

args *args_new() {
    args *args = NULL;

    args = malloc(sizeof(args));

    if (!args)
        return NULL;

    args->opts = NULL;
    args->operandsc = 0;
    args->operands = NULL;

    return args;
}

void args_free(args *args) {
    if (args->opts)
        option_free(args->opts);
    args->opts = NULL;

    if (args->operands)
        operand_free(args->operands);
    args->operands = NULL;

    free(args);
}

int args_add_opt(args *args, option *opt) {
    if (!args || !opt)
        return EXIT_FAILURE;

    if (!opt->short_opt && !opt->long_opt)
        return EXIT_FAILURE;

    option *last = args->opts;
    if (last != NULL) {
        while (last->next != NULL)
            last = last->next;
        last->next = opt;
    } else
        args->opts = opt;

    return EXIT_SUCCESS;
}

int args_help(args *args, FILE *stream) {
    if (!args || !stream || !args->opts)
        return EXIT_FAILURE;

    option *opt = args->opts;
    while (opt) {
        option_help(opt, stream);
        opt = opt->next;
    }

    return EXIT_SUCCESS;
}

option *option_find(args *args, char *opt) {
    bool short_opt = false;
    char *passed;
    if (strlen(opt) == 1)
        short_opt = true;

    option *cur = args->opts;
    char *compare;
    while (cur) {
        if (short_opt && cur->short_opt)
            compare = cur->short_opt;
        else if (cur->long_opt)
            compare = cur->long_opt;

        if (compare && strcmp(opt, compare) == 0)
            return cur;

        compare = NULL;
        cur = cur->next;
    }

    return NULL;
}