/*
 * Copyright 2018, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#include <autoconf.h>
#include <utils/gen_config.h>
#include <utils/stack.h>

void *
utils_run_on_stack(void *stack_top, void * (*func)(void *arg), void *arg)
{
    void *ret;
    asm volatile (
        "move $fp, $sp\n\t"                /* Save sp - s0 is callee saved so we don't need to save it. */
        "move $sp, %[new_stack]\n\t"      /* Switch to new stack. */
        "move $a0, %[arg]\n\t"            /* Setup argument to func. */
        "jirl $ra, %[func], 0\n\t"
        "move $sp, $fp\n\t"
        "move %[ret], $a0\n\t"
        : [ret] "=r" (ret)
        : [new_stack] "r" (stack_top),
        [func] "r" (func),
        [arg] "r" (arg)
        /* caller saved registers */
        : "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$a0", "$a1", "$a2", "$a3", "$a4", "$a5", "$a6", "$a7", "$ra");
    return ret;
}
