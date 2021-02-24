// cmdapp: main.c: A program to test the cmdapp library.
// Copyright (C) 2021 Ethan Uppal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "cmdapp.h"
#include <stdio.h>

static void setup_options(cmdapp_t* app, cmdopt_t* opt) {
    cmdapp_set(
        app,
        'D',
        "define",
        CMDOPT_TAKESARG,
        NULL /* either accepts file or in-command text. */,
        "define a macro",
        &opt[0]
    );
    cmdapp_set(
        app,
        'U',
        "undef",
        CMDOPT_TAKESARG,
        NULL /* either accepts file or in-command text. */,
        "undefine a macro",
        &opt[1]
    );
    cmdapp_set(
        app,
        'c',
        "config",
        CMDOPT_TAKESARG,
        NULL /* either accepts file or in-command text. */,
        "parse a config file",
        &opt[2]
    );
    cmdapp_set(
        app,
        'p',
        "plugdir",
        CMDOPT_TAKESARG,
        NULL,
        "add ARG to the plugin search path",
        &opt[3]
    );
    cmdapp_set(
        app,
        '\b',
        "\b\b\b\b\b\b\b\b\b\b\\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b",
        CMDOPT_OPTIONAL,
        NULL,
        "Audio Ouput",
        &opt[13]
    );
    cmdapp_set(
        app,
        's',
        "samplerate",
        CMDOPT_TAKESARG,
        NULL,
        "set the samplerate",
        &opt[4]
    );
    cmdapp_set(
        app,
        'i',
        "input",
        CMDOPT_TAKESARG,
        NULL,
        "number of input channel",
        &opt[5]
    );
    cmdapp_set(
        app,
        'o',
        "output",
        CMDOPT_TAKESARG,
        NULL,
        "number of output channel",
        &opt[6]
    );
    cmdapp_set(
        app,
        'd',
        "driver",
        CMDOPT_TAKESARG,
        NULL,
        "set driver (and arguments)",
        &opt[7]
    );
    cmdapp_set(
        app,
        'l',
        "module",
        CMDOPT_TAKESARG,
        NULL,
        "activate module (and arguments)",
        &opt[8]
    );
    cmdapp_set(
        app,
        'l',
        "loop",
        CMDOPT_TAKESARG,
        NULL,
        "set loop mode",
        &opt[9]
    );
    cmdapp_set(
        app,
        'g',
        "passes",
        CMDOPT_TAKESARG,
        NULL,
        "set pass order",
        &opt[10]
    );
    cmdapp_set(
        app,
        'I',
        "include",
        CMDOPT_TAKESARG,
        NULL,
        "add ARG to include path",
        &opt[11]
    );
    cmdapp_set(
        app,
        '\0',
        "stdin",
        CMDOPT_OPTIONAL,
        NULL,
        "read from stdin",
        &opt[12]
    );

}

void handle_options(cmdopt_t* file, cmdopt_t* eval) {
    if (cmdopt_exists(*file)) {
        printf("file - %s\n", file->value);
    } else if (cmdopt_exists(*eval)) {
        printf("eval - %s\n", eval->value);
    }
}

void myproc(cmdopt_t* option, const char* arg, bool is_plain_arg) {
/*
    if (is_plain_arg) {
        printf("arg: %s\n", arg);
    } else {
        printf("-%c%s\n", option->shorto, option->value);
    }
*/
}

int main(int argc, char* argv[]) {
    cmdapp_t app;
/*
    const char* custom_synopses[] = {
        "subcommand [OPTION]...",
        "[OPTION]... FILE...",
        NULL
    };
*/
const char* synopses[] = { "[Audio Options]... [Preprocessor Options]... Files,,,", NULL };
    const cmdapp_info_t info = {
        .program = "gwion",
        .synopses = NULL, // so it's automatic
        .synopses = synopses,
        .version = "N.A.",
        .author = "Jérémie Astor",
        .year = 2016,
        .description = "Strongly timed musical programming language.",
        .help_des_offset = 28,
        .ver_extra =
        "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>\n"
        "This is free software: you are free to change and redistribute it.\n"
        "There is NO WARRANTY, to the extent permitted by law.\n"
    };
    cmdapp_init(&app, argc, argv, CMDAPP_MODE_SHORTARG, &info);
    cmdapp_enable_procedure(&app, myproc);
    cmdopt_t opt[16];
    setup_options(&app, opt);

    if (cmdapp_run(&app) == EXIT_SUCCESS) {
       if (cmdapp_should_exit(&app)) return 0;
        handle_options(&opt[0], &opt[1]);
    } else {
        return 1;
    }

    cmdapp_destroy(&app);

    return 0;
}
