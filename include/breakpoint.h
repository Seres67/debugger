#ifndef DEBUGGER_BREAKPOINT_H
#define DEBUGGER_BREAKPOINT_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

struct breakpoint_s
{
    pid_t pid;
    intptr_t addr;
    bool enabled;
    uint8_t saved_data;
};

typedef struct breakpoint_s breakpoint_t;

void breakpoint_enable(breakpoint_t *brk);
void breakpoint_disable(breakpoint_t *brk);
bool breakpoint_is_enabled(breakpoint_t *brk);
intptr_t breakpoint_get_address(breakpoint_t *brk);

#endif // !DEBUGGER_BREAKPOINT_H
