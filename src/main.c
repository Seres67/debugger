#include <debugger.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <unistd.h>

int main(int ac, char **av)
{
    if (ac < 2) {
        fprintf(stderr, "program name not specified\n");
        return 1;
    }
    size_t size = strlen(av[1]);
    char *prog_name = malloc((size + 1) * sizeof(char));
    if (!prog_name) {
        fprintf(stderr, "couldn't allocate memory");
        return 1;
    }
    strncpy(prog_name, av[1], size);
    int pid = fork();
    if (pid == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl(prog_name, prog_name, NULL);
    } else if (pid >= 1) {
        printf("started debugging %d\n", pid);
        debugger_t dbg = {prog_name, pid};
        debugger_run(&dbg);
    }
}
