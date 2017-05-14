#ifndef OPENGL_LOGS_H
#define OPENGL_LOGS_H

#include <stdio.h>

#ifdef OS_APPLE

#define LOGI(format, ...) printf(format, __VA_ARGS__)

#elif OS_WINDOWS

#define LOGI(format, ...) printf_s(format, __VA_ARGS__)

#endif

#define LOGM(message) LOGI(message, NULL)

#endif //OPENGL_LOGS_H
