#include <include/breakpoint.h>
#include <sys/ptrace.h>
#include <unistd.h>

void breakpoint_enable(breakpoint_t *brk)
{
    uint64_t data = ptrace(PTRACE_PEEKDATA, brk->pid, brk->addr, NULL);
    brk->saved_data = (uint8_t)(data & 0xFF);
    uint64_t int3 = 0xCC;
    uint64_t data_int3 = ((data & ~0xFF) | int3);
    ptrace(PTRACE_POKEDATA, brk->pid, brk->addr, data_int3);
    brk->enabled = true;
}

void breakpoint_disable(breakpoint_t *brk)
{
    uint64_t data = ptrace(PTRACE_PEEKDATA, brk->pid, brk->addr, NULL);
    uint64_t restored = ((data & ~0xFF) | brk->saved_data);
    ptrace(PTRACE_POKEDATA, brk->pid, brk->addr, restored);
    brk->enabled = false;
}

bool breakpoint_is_enabled(breakpoint_t *brk) { return brk->enabled; }

intptr_t breakpoint_get_address(breakpoint_t *brk) { return brk->addr; }
