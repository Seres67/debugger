#include <debugger.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/personality.h>
#include <sys/ptrace.h>
#include <unistd.h>

int main(int ac, char **av)
{
    if (ac < 2) {
        fprintf(stderr, "program name not specified\n");
        return 1;
    }
    size_t size = strlen(av[1]);
    char *prog_name = (char *)malloc((size + 1) * sizeof(char));
    if (!prog_name) {
        fprintf(stderr, "couldn't allocate memory");
        return 1;
    }
    prog_name[size] = 0;
    strncpy(prog_name, av[1], size);
    int pid = fork();
    if (pid == 0) {
        personality(ADDR_NO_RANDOMIZE);
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0) {
            printf("couldn't start ptrace\n");
            return 0;
        }
        execl(prog_name, prog_name, NULL);
    } else if (pid >= 1) {
        printf("started debugging %d\n", pid);
        debugger_t dbg = {prog_name, pid, NULL};
        debugger_run(&dbg);
    }
}
