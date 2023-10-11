#ifndef _DEBUGGER_DEBUGGER_H
#define _DEBUGGER_DEBUGGER_H

#include <sys/types.h>
struct debugger_s
{
    char *prog_name;
    pid_t pid;
};
typedef struct debugger_s debugger_t;

void debugger_run(debugger_t dbg);

#endif // !_DEBUGGER_DEBUGGER_H
