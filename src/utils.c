#include <include/utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void malloc_error(char const *file, int line)
{
    fprintf(stderr, "couldn't malloc at %s %d", file, line);
    exit(1);
}

char **split(char const *line, char const *delimiter)
{
    size_t size = 0;
    size_t nb_of_elements = 0;
    for (; line[size]; ++size) {
        if (line[size] == *delimiter) ++nb_of_elements;
    }
    char *line_copy = (char *)malloc(sizeof(char) * (strlen(line) + 1));
    if (!line_copy) DEBUGGER_MALLOC_CHECK
    strcpy(line_copy, line);
    char **splitted_line =
        (char **)malloc(sizeof(char *) * (nb_of_elements + 2));
    if (!splitted_line) DEBUGGER_MALLOC_CHECK
    splitted_line[nb_of_elements + 1] = NULL;
    char *token = strtok(line_copy, delimiter);
    for (int i = 0; token != NULL; ++i) {
        splitted_line[i] = (char *)malloc(sizeof(char) * (strlen(token) + 1));
        if (!splitted_line[i]) DEBUGGER_MALLOC_CHECK
        strcpy(splitted_line[i], token);
        token = strtok(NULL, delimiter);
    }
    free(line_copy);
    return splitted_line;
}

bool is_prefix(char *s, char const *of)
{
    if (strlen(s) > strlen(of)) return false;
    return strcmp(s, of) == 0;
}
