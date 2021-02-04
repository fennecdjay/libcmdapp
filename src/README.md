### Usage

You can initialize an app object with:

```c
cmdapp_t app;
const cmdapp_info_t info = { ... };
cmdapp_init(&app, argc, argv, CMDAPP_MODE, &info);
```

It's important to note here that `argv` must be of type `char**`. The penultimate argument `CMDAPP_MODE` is a combination of modes (using `|`). Modes provided will be define as macros prefixed with `CMDAPP_MODE` that resolve to integer constant expressions. The last argument contains necessary metadata for automatic `--help` and `--version` generation.
