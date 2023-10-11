#include "include/debugger.h"
#include "include/utils.h"
#include <replxx.h>
#include <stdio.h>
#include <stdlib.h>
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
    } else if (is_prefix(command, "break")) {
        printf("break\n");
        char *address = &command[2];
        debugger_set_breakpoint_at_address(dbg, strtol(address, 0, 16));
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

void debugger_set_breakpoint_at_address(debugger_t *dbg, intptr_t address)
{
    printf("Setting breakpoint at 0x%x\n", (unsigned int)address);
    breakpoint_t brk = {dbg->pid, address, false, 0};
    breakpoint_enable(&brk);
    unsigned long count = 0;
    if (dbg->breakpoints != NULL) {
        for (; dbg->breakpoints[count]; ++count)
            ;
        dbg->breakpoints = (breakpoint_t **)realloc(
            dbg->breakpoints, sizeof(breakpoint_t) * (count + 1));
        if (!dbg->breakpoints) DEBUGGER_MALLOC_CHECK
        // TODO: this should be a hashmap...
        dbg->breakpoints[count] = NULL;
        dbg->breakpoints[count - 1] =
            &brk; // NOTE: and this should be dbg->breakpoints[address] = &brk;
                  // but its not a hashmap...
    } else {
        dbg->breakpoints = (breakpoint_t **)malloc(sizeof(breakpoint_t) * 2);
        dbg->breakpoints[1] = NULL;
    }
}
