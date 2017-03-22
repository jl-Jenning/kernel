#ifndef __HASH_H
#define __HASH_H

// Include functionality relating to newlib (the standard C library).

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "types/types.h"
#include "list/list.h"
#include "sched/task.h"

#define SIZE 10

typedef struct {
    list_head node;
    task_t * task;
} pid_node_t;

typedef struct {
    list_head entry[SIZE];
} pid_table_t;

void HASH_INIT (pid_table_t * table);

int hash_PID(pid_t pid);

task_t * find_task_PID(pid_table_t * table, pid_t pid);

void add_hash_entry(pid_table_t * table, task_t * entry);

void remove_hash_entry(pid_table_t * table, pid_t pid);

#endif