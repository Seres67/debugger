#include <include/utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void malloc_error(char *file, int line)
{
    fprintf(stderr, "couldn't malloc at %s %d", file, line);
    exit(1);
}

char **split(char const *line, char *delimiter)
{
    size_t size = 0;
    size_t nb_of_elements = 0;
    for (; line[size]; ++size) {
        if (line[size] == *delimiter) ++nb_of_elements;
    }
    char *line_copy = malloc(sizeof(char) * (strlen(line) + 1));
    if (!line_copy) {
        DEBUGGER_MALLOC_CHECK
    }
    strcpy(line_copy, line);
    char **splitted_line = malloc(sizeof(char *) * (nb_of_elements + 2));
    splitted_line[nb_of_elements + 1] = NULL;
    char *token = strtok(line_copy, delimiter);
    for (int i = 0; token != NULL; ++i) {
        splitted_line[i] = malloc(sizeof(char) * (strlen(token) + 1));
        if (!splitted_line[i]) {
            DEBUGGER_MALLOC_CHECK
        }
        strcpy(splitted_line[i], token);
        token = strtok(NULL, delimiter);
    }
    return splitted_line;
}

bool is_prefix(char *s, char *of)
{
    if (strlen(s) > strlen(of)) return false;
    return strcmp(s, of) == 0;
}
