#include "include/debugger.h"
#include "include/utils.h"
#include <replxx.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

void debugger_continue_execution(debugger_t *dbg)
{
    ptrace(PTRACE_CONT, dbg->pid, NULL, NULL);

    int wait_status;
    int options = 0;
    waitpid(dbg->pid, &wait_status, options);
}

void debugger_handle_command(debugger_t *dbg, char const *line)
{
    char **args = split(line, " ");
    char *command = args[0];

    if (is_prefix(command, "continue")) {
        debugger_continue_execution(dbg);
    } else {
        fprintf(stderr, "unknown command\n");
    }
}

void debugger_run(debugger_t *dbg)
{
    int wait_status;
    int options = 0;
    waitpid(dbg->pid, &wait_status, options);

    char const *line = NULL;
    Replxx *repl = replxx_init();
    char const *prompt = "minidbg> ";
    while ((line = replxx_input(repl, prompt)) != NULL) {
        debugger_handle_command(dbg, line);
        replxx_history_add(repl, line);
    }
    replxx_end(repl);
}
