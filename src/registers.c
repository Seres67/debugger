#include <include/registers.h>
#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>

uint64_t get_register_value(pid_t pid, registers_t reg)
{
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    perror("get_register_value");
    uint64_t register_pos = N_REGISTERS;
    for (int i = 0; i < N_REGISTERS; ++i)
        if (REGISTER_DESCRIPTORS[i].reg == reg) register_pos = i;
    if (register_pos >= N_REGISTERS) {
        printf("couldn't find register\n");
        return 0;
    }
    return *((uint64_t *)&regs + register_pos);
}

void set_register_value(pid_t pid, registers_t reg, uint64_t value)
{
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    perror("set_register_value");
    uint64_t register_pos = N_REGISTERS;
    for (int i = 0; i < N_REGISTERS; ++i)
        if (REGISTER_DESCRIPTORS[i].reg == reg) register_pos = i;
    if (register_pos >= N_REGISTERS) {
        printf("couldn't find register\n");
        return;
    }
    *((uint64_t *)&regs + register_pos) = value;
    ptrace(PTRACE_SETREGS, pid, NULL, &regs);
}

uint64_t get_register_value_from_dwarf(pid_t pid, unsigned reg_number)
{
    uint64_t reg_position = N_REGISTERS;
    for (int i = 0; i < N_REGISTERS; ++i)
        if (REGISTER_DESCRIPTORS[i].dwarf_r == (signed)reg_number)
            reg_position = i;
    if (reg_position >= N_REGISTERS) {
        fprintf(stderr, "unknown dwarf register\n");
        exit(1);
    }
    return get_register_value(pid, REGISTER_DESCRIPTORS[reg_position].reg);
}

char *get_register_name(registers_t reg)
{
    for (int i = 0; i < N_REGISTERS; ++i)
        if (REGISTER_DESCRIPTORS[i].reg == reg)
            return REGISTER_DESCRIPTORS[i].name;
    return NULL;
}

registers_t get_register_from_name(char *name)
{
    for (int i = 0; i < N_REGISTERS; ++i)
        if (!strcmp(REGISTER_DESCRIPTORS[i].name, name))
            return REGISTER_DESCRIPTORS[i].reg;
    return -1;
}
