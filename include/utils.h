#ifndef DEBUGGER_UTILS_H
#define DEBUGGER_UTILS_H

#include <stdbool.h>

#define DEBUGGER_MALLOC_CHECK malloc_error(__FILE__, __LINE__);

char **split(char const *line, char const *delimiter);
bool is_prefix(char *s, char const *of);
_Noreturn void malloc_error(char const *file, int line);

#endif // !DEBUGGER_UTILS_H
