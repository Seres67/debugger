#ifndef DEBUGGER_REGISTERS_H
#define DEBUGGER_REGISTERS_H

#include <stdint.h>
#include <stdlib.h>

#define N_REGISTERS 27

enum registers_e
{
    rax,
    rbx,
    rcx,
    rdx,
    rdi,
    rsi,
    rbp,
    rsp,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,
    rip,
    rflags,
    cs,
    orig_rax,
    fs_base,
    gs_base,
    fs,
    gs,
    ss,
    ds,
    es
};

typedef enum registers_e registers_t;

struct register_descriptor_s
{
    registers_t reg;
    int dwarf_r;
    char *name;
};

typedef struct register_descriptor_s register_descriptor_t;

static register_descriptor_t REGISTER_DESCRIPTORS[N_REGISTERS] = {
    {r15, 15, "r15"},
    {r14, 14, "r14"},
    {r13, 13, "r13"},
    {r12, 12, "r12"},
    {rbp, 6, "rbp"},
    {rbx, 3, "rbx"},
    {r11, 11, "r11"},
    {r10, 10, "r10"},
    {r9, 9, "r9"},
    {r8, 8, "r8"},
    {rax, 0, "rax"},
    {rcx, 2, "rcx"},
    {rdx, 1, "rdx"},
    {rsi, 4, "rsi"},
    {rdi, 5, "rdi"},
    {orig_rax, -1, "orig_rax"},
    {rip, -1, "rip"},
    {cs, 51, "cs"},
    {rflags, 49, "rflags"},
    {rsp, 7, "rsp"},
    {ss, 52, "ss"},
    {fs_base, 58, "fs_base"},
    {gs_base, 59, "gs_base"},
    {ds, 53, "ds"},
    {es, 50, "es"},
    {fs, 54, "fs"},
    {gs, 55, "gs"}};

uint64_t get_register_value(pid_t pid, registers_t reg);
void set_register_value(pid_t pid, registers_t reg, uint64_t value);
uint64_t get_register_value_from_dwarf(pid_t pid, unsigned reg_number);
char *get_register_name(registers_t reg);
registers_t get_register_from_name(char *name);

#endif // !DEBUGGER_REGISTERS_H
