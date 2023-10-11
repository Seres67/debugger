#ifndef _DEBUGGER_UTILS_H
#define _DEBUGGER_UTILS_H

#include <stdbool.h>

char **split(char const *line, char *delimiter);
bool is_prefix(char *s, char *of);

#endif // !_DEBUGGER_UTILS_H
