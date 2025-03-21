#ifndef STATIC_PRIO_SCHED_H
#define STATIC_PRIO_SCHED_H

#include "catos_types.h"
#include "catos_config.h"

typedef struct _static_prio_task_config_t static_prio_task_config_t;
struct _static_prio_task_config_t
{
    const cat_uint8_t *task_name;
    cat_uint32_t stack_start_addr;
    cat_uint32_t stack_size;
};

void static_prio_scheduler_register(void);

#endif