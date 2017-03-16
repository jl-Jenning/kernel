#ifndef __TASK_H
#define __TASK_H

#define task_current_entry(ptr) \
        list_entry(ptr, task_t, node)

#define task_next_entry(ptr) \
        list_first_entry(ptr, task_t, node)

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "list/list.h"

/*
 * For manipulating tasks
 */

typedef struct {
    volatile int state; // program state
    int prio;
    pid_t pid;
    ctx_t ctx;
    list_head node;
} task_t;

task_t create_task(pid_t pid, uint32_t cpsr, uint32_t pc, uint32_t sp); 

void TASK_INIT(task_t * new_task,pid_t pid, uint32_t cpsr, uint32_t pc, uint32_t sp);

void set_task_state(task_t * task, int state);

void set_task_prio(task_t * task, int prio);

void add_task_next(task_t * new, list_head * head);

void add_task_last(task_t * new, list_head * head);

#endif