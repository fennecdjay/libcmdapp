# libcmdarg

A customizable command option and argument parsing library written in C.

### Features

**1. Short and long options**

Standard short options with a dash and a character are supported alongside long options with 2 dashes. 2 dashes by themselves, however, indicate that every subsequent value is an argument.

**2. Customizable option parsing behavior**

Short options are given arguments by a space after the option, and long options are given arguments through an equals sign after the option. However, that does not specify what do to when something like `-abc` is encountered. `CMDAPP_MODE_MULTIFLAG` and `CMDAPP_MODE_SHORTARG` exist to resolve this.

mode | behavior
---- | -------
`CMDAPP_MODE_MULTIFLAG` | `-abc` -> `-a -b -c`
`CMDAPP_MODE_SHORTARG` | `-abc` -> `-a bc`

**3. Error handling and diagnostics**

`cmdapp_run` notifies the caller of an error through its return value, and if not silenced with `CMDAPP_MODE_SILENT` will print diagnostic error messages to `stderr`.

**4. Automatic `--help` and `--version`**

If you do not set these as options, libcmdarg will use various information to automatically generate responses in such a way that `help2man` will work.

**5. Proceeding AND procedural parsing**

Not only does `cmdapp_run` terminate with all options and arguments in the proper state, it also calls optional functions *as it parses*. This allows for repeated options and usage of args sensitive to positioning of options.

If you have a interface like `-Dfoo file1 -Dbar file1` which will analyze `file1` twice with different definitions, using proceeding parsing will give you `-D=bar` and args=`file1, file1`. However, with procedural parsing you will get: `-D=foo`, `file1`, `-D=bar`, `file1`. It's also important to note that the `cmdopt_t`s on the user side will be set as well, so you can refer to as many of them as you want when you come across an arg. This is useful if you have `-a -b file1 -a file1` and you want the first analysis of `file1` to be in both modes `a` and `b`.

### Usage

You can initialize an app object with:

```c
cmdapp_t app;
const cmdapp_info_t info = { ... };
cmdapp_init(&app, argc, argv, CMDAPP_MODE, &info);
```

It's important to note here that `argv` must be of type `char**`. The penultimate argument `CMDAPP_MODE` is a combination of modes (using `|`). Modes provided will be define as macros prefixed with `CMDAPP_MODE` that resolve to integer constant expressions. The last argument contains necessary metadata for automatic `--help` and `--version` generation.

You then use `cmdapp_set` for each of your options. Here is an example setup for a hypothetical interpreter.

```c
enum Options {
    Eval, File, OPTION_COUNT
};
cmdopt_t options[OPTION_COUNT];
cmdapp_set(&app, 'e', "eval", CMDOPT_TAKESARG, "Evaluates a script", &options[Eval]);
cmdapp_set(&app, 'f', "file", CMDOPT_TAKESARG, "Interprets a file", &options[File]);
```

After this, you can simply call `cmdapp_run(&app)`. If it returns `EXIT_SUCCESS`, then you can look at the properties of your options.

To see if your option was passed, use `cmdopt_exists`. To get the arg it was passed, use the `value` member. Don't rely on any other members.

To get a list of ordinary arguments, use `cmdapp_getargs(&app)` and check if it is `NULL` before using the members `length` and `contents`:

```c
typedef struct {
    size_t length;
    const char** contents;
} cmdargs_t;
```

Once done, use `cmdapp_destroy(&app)`. Any subsequent member access is undefined. This also destroys the list of ordinary arguments, so copy it before you call this destructor.

### Documentation

This may come soon, but the header file `app.h` is heavily commented.
