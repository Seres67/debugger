#ifndef _DEBUGGER_UTILS_H
#define _DEBUGGER_UTILS_H

#include <stdbool.h>

#define DEBUGGER_MALLOC_CHECK malloc_error(__FILE__, __LINE__);

char **split(char const *line, char *delimiter);
bool is_prefix(char *s, char *of);
void malloc_error(char *file, int line);

#endif // !_DEBUGGER_UTILS_H
