#include "include/debugger.h"
#include "breakpoint.h"
#include "include/utils.h"
#include "registers.h"
#include <replxx.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

uint64_t debugger_get_pc(debugger_t *dbg)
{
    return get_register_value(dbg->pid, rip);
}

void debugger_set_pc(debugger_t *dbg, uint64_t pc)
{
    set_register_value(dbg->pid, rip, pc);
}

void debugger_wait_for_signal(debugger_t *dbg)
{
    int debugger_wait_status;
    int options = 0;
    waitpid(dbg->pid, &debugger_wait_status, options);
}

void debugger_step_over_breakpoint(debugger_t *dbg)
{
    intptr_t possible_breakpoint = debugger_get_pc(dbg) - 1;
    for (int i = 0; dbg->breakpoints[i]; ++i) {
        if (dbg->breakpoints[i]->addr == possible_breakpoint) {
            breakpoint_t *breakpoint = dbg->breakpoints[i];
            if (breakpoint_is_enabled(breakpoint)) {
                uint64_t previous_instruction_address = possible_breakpoint;
                debugger_set_pc(dbg, previous_instruction_address);
                breakpoint_disable(breakpoint);
                ptrace(PTRACE_SINGLESTEP, dbg->pid, NULL, NULL);
                debugger_wait_for_signal(dbg);
                breakpoint_enable(breakpoint);
            }
        }
    }
}

void debugger_continue_execution(debugger_t *dbg)
{
    debugger_step_over_breakpoint(dbg);
    ptrace(PTRACE_CONT, dbg->pid, NULL, NULL);
    debugger_wait_for_signal(dbg);
}

void debugger_dump_registers(debugger_t *dbg)
{
    for (int i = 0; i < N_REGISTERS; ++i)
        printf("%s 0x%016lx\n", REGISTER_DESCRIPTORS[i].name,
               get_register_value(dbg->pid, REGISTER_DESCRIPTORS[i].reg));
}

uint64_t debugger_read_memory(debugger_t *dbg, uint64_t address)
{
    return ptrace(PTRACE_PEEKDATA, dbg->pid, address, NULL);
}

void debugger_write_memory(debugger_t *dbg, uint64_t address, uint64_t value)
{
    ptrace(PTRACE_POKEDATA, dbg->pid, address, value);
}

void debugger_handle_command(debugger_t *dbg, char const *line)
{
    char **args = split(line, " ");
    char *command = args[0];

    if (is_prefix(command, "continue")) {
        debugger_continue_execution(dbg);
    } else if (is_prefix(command, "break")) {
        char *address = &args[1][2];
        debugger_set_breakpoint_at_address(dbg, strtol(address, 0, 16));
    } else if (is_prefix(command, "register")) {
        if (is_prefix(args[1], "dump")) {
            debugger_dump_registers(dbg);
        } else if (is_prefix(args[1], "read")) {
            printf("%016lx\n", get_register_value(
                                   dbg->pid, get_register_from_name(args[2])));
        } else if (is_prefix(args[1], "write")) {
            char *value = &args[3][2];
            set_register_value(dbg->pid, get_register_from_name(args[2]),
                               strtol(value, 0, 16));
        }
    } else if (is_prefix(command, "memory")) {
        char *address = &args[2][2];
        if (is_prefix(args[1], "read")) {
            printf("%016lx\n",
                   debugger_read_memory(dbg, strtol(address, 0, 16)));
        } else if (is_prefix(args[1], "write")) {
            char *value = &args[3][2];
            debugger_write_memory(dbg, strtol(address, 0, 16),
                                  strtol(value, 0, 16));
        }
    } else {
        fprintf(stderr, "unknown command\n");
    }
}

void debugger_run(debugger_t *dbg)
{
    debugger_wait_for_signal(dbg);
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
    breakpoint_t *brk = (breakpoint_t *)malloc(sizeof(breakpoint_t));
    if (!brk) DEBUGGER_MALLOC_CHECK
    brk->pid = dbg->pid;
    brk->addr = address;
    brk->enabled = false;
    brk->saved_data = 0;
    breakpoint_enable(brk);
    unsigned long count = 0;
    if (dbg->breakpoints != NULL) {
        for (; dbg->breakpoints[count]; ++count)
            ;
        dbg->breakpoints = (breakpoint_t **)realloc(
            dbg->breakpoints, sizeof(breakpoint_t *) * (count + 1));
        if (!dbg->breakpoints) DEBUGGER_MALLOC_CHECK
        // TODO: this should be a hashmap...
        dbg->breakpoints[count] = NULL;
        dbg->breakpoints[count - 1] =
            brk; // NOTE: and this should be dbg->breakpoints[address] = brk;
                 // but its not a hashmap...
    } else {
        dbg->breakpoints = (breakpoint_t **)malloc(sizeof(breakpoint_t *) * 2);
        if (!dbg->breakpoints) DEBUGGER_MALLOC_CHECK
        dbg->breakpoints[0] = brk;
        dbg->breakpoints[1] = NULL;
    }
}
