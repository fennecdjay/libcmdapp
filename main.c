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

#include "app.h"
#include <stdio.h>

void setup_options(cmdapp_t* app, cmdopt_t* file, cmdopt_t* eval) {
    cmdopt_t* file_confl[] = { eval, NULL };
    cmdopt_t* eval_confl[] = { file, NULL };
    cmdapp_set(
        app,
        'f',
        "file",
        CMDOPT_TAKESARG,
        file_confl /* either accepts file or in-command text. */,
        "Does nothing with a file",
        file
    );
    cmdapp_set(
        app,
        'e',
        "eval",
        CMDOPT_TAKESARG,
        eval_confl /* either accepts file or in-command text. */,
        "Does nothing with a script",
        eval
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
    if (is_plain_arg) {
        printf("arg: %s\n", arg);
    } else {
        printf("-%c%s\n", option->shorto, option->value);
    }
}

int main(int argc, char* argv[]) {
    cmdapp_t app;
    const char* custom_synopses[] = {
        "subcommand [OPTION]...",
        "[OPTION]... FILE...",
        NULL
    };
    const cmdapp_info_t info = {
        .program = "main",
        // .synopses = NULL, // so it's automatic
        .synopses = custom_synopses,
        .version = "1.0",
        .author = "*lolptr",
        .year = 2021,
        .description = "A program that does nothing.",
        .help_des_offset = 20,
        .ver_extra =
        "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>\n"
        "This is free software: you are free to change and redistribute it.\n"
        "There is NO WARRANTY, to the extent permitted by law.\n"
    };
    cmdapp_init(&app, argc, argv, CMDAPP_MODE_SHORTARG, &info);
    cmdapp_enable_procedure(&app, myproc);
    cmdopt_t file, eval;
    setup_options(&app, &file, &eval);

    if (cmdapp_run(&app) == EXIT_SUCCESS) {
        handle_options(&file, &eval);
    } else {
        return 1;
    }

    cmdapp_destroy(&app);

    return 0;
}
