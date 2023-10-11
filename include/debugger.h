#ifndef _DEBUGGER_DEBUGGER_H
#define _DEBUGGER_DEBUGGER_H

#include "breakpoint.h"
#include <stdint.h>
#include <sys/types.h>

struct debugger_s
{
    char *prog_name;
    pid_t pid;
    breakpoint_t **breakpoints;
};
typedef struct debugger_s debugger_t;

void debugger_run(debugger_t *dbg);
void debugger_handle_command(debugger_t *dbg, char const *line);
void debugger_continue_execution(debugger_t *dbg);
void debugger_set_breakpoint_at_address(debugger_t *dbg, intptr_t address);

#endif // !_DEBUGGER_DEBUGGER_H
