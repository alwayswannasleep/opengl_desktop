#ifndef OPENGL_LOGS_H
#define OPENGL_LOGS_H

#include <stdio.h>

#ifdef __APPLE__

#define LOGI(format, ...) printf(format, __VA_ARGS__)

#elif WIN_32

#define LOGI(format, ...) printf_s(format, __VAR_ARGS__)

#endif

#endif //OPENGL_LOGS_H
