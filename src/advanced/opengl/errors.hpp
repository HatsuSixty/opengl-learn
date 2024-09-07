#pragma once

void gl_clear_errors();
void gl_check_errors();

#define gl(name, ...)          \
    do {                       \
        gl_clear_errors();     \
        gl##name(__VA_ARGS__); \
        gl_check_errors();     \
    } while (0);

#define gl_call(...)       \
    do {                   \
        gl_clear_errors(); \
        __VA_ARGS__;       \
        gl_check_errors(); \
    } while (0);