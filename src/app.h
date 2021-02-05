// cmdapp: app.h
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

#ifndef _CMDAPP_APP_H
#define _CMDAPP_APP_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

typedef uint8_t cmdopt_flags_t;
typedef uint8_t cmdapp_mode_t;

typedef struct {
    char shorto;
    const char* longo;
    const char* value;
    cmdopt_flags_t flags;
} cmdopt_t;

#define CMDOPT_EXISTS   0b00000001
#define CMDOPT_OPTIONAL 0b00000010
#define CMDOPT_TAKESARG 0b00000100

#define CMDAPP_MODE_MULTIFLAG 0b00000000
#define CMDAPP_MODE_SHORTARG  0b00000001
#define CMDAPP_MODE_SILENT    0b00000000
#define CMDAPP_MODE_PRINT     0b00000010

// Returns nonzero if the option was provided to the app
#define cmdopt_exists(opt)      ((opt).flags & CMDOPT_EXISTS)
// Returns nonzero if the option was declared as optional
#define cmdopt_is_optional(opt) ((opt).flags | CMDOPT_OPTIONAL)

typedef struct _cmdopt_internal_t cmdopt_internal_t;

typedef struct {
    const char* program;
    const char** synopses;
    const char* version;
    const char* author;
    int year;
    const char* description;
    const char* ver_extra;
} cmdapp_info_t;

typedef struct {
    size_t length;
    const char** contents;
} cmdargs_t;

typedef struct {
    int _argc;
    char** _argv;
    cmdapp_mode_t _mode;
    cmdapp_info_t _info;
    size_t _length;
    size_t _capacity;
    cmdopt_internal_t** _start;
    cmdargs_t _args;
    cmdapp_procedure _proc;
} cmdapp_t;

typedef void (*cmdapp_procedure)(cmdopt_t* option, const char* arg, bool is_plain_arg);

// Initializes a cmdapp_t with the given program environment, mode and metadata.
void cmdapp_init(cmdapp_t* app, int argc, char** argv, cmdapp_mode_t mode,
                 const cmdapp_info_t* info);

// Destroys the given cmdapp_t. Any subsequent member access is undefined.
void cmdapp_destroy(cmdapp_t* app);

// Registers an option to the app with the given values and flags
void cmdapp_set(cmdapp_t* app, char shorto, const char* longo, uint8_t flags,
                cmdopt_t** conflicts, const char* description,
                cmdopt_t* option);

// Generates a --help output to stdout. If cmdapp_info was not called, the
// function behavior is undefined
void cmdapp_print_help(cmdapp_t* app);

// Generates a --version output to stdout. If cmdapp_info was not called, the
// function behavior is undefined
void cmdapp_print_version(cmdapp_t* app);

// Sets the cmdapp_t's procedural parsing function to the given function. If set
// to NULL, it disables procedural parsing (default).
void cmdapp_enable_procedure(cmdapp_t* app, cmdapp_procedure proc);

// Returns EXIT_SUCCESS on success and EXIT_FAILURE otherwise (printing a
// diagnostic to stderr if configured)
int cmdapp_run(cmdapp_t* app);

// Returns a pointer to an array of standalone command line arguments, or NULL
// if none exist.
cmdargs_t* cmdapp_getargs(cmdapp_t* app);

#endif /* _CMDAPP_APP_H */
