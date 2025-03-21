#include <stdio.h>
#include "cat_scheduler.h"

struct _cat_task_t task;
IMPORT_SECTION(cat_scheduler_sec);
int main(void)
{
    printf("sizeof(scheduler_t)=%d\r\n", sizeof(cat_scheduler_t));

    printf("0x%x\r\n", SECTION_START(cat_scheduler_sec));

    print_scheduler();

    void print_scheduler(void);

    cat_scheduler_init_all_scheduler();

    cat_scheduler_create_task_static(CAT_SCHED_STRATEGY_STATIC_PRIO, NULL);
    cat_scheduler_create_task_static(CAT_SCHED_STRATEGY_EDF, NULL);

    cat_scheduler_deal_in_tick();

    task.sched_strategy = CAT_SCHED_STRATEGY_STATIC_PRIO;
    cat_scheduler_task_rdy(&task);
    cat_scheduler_task_unrdy(&task);
    cat_scheduler_task_delay(&task, 0);
    cat_scheduler_task_delay_wakeup(&task);
    cat_scheduler_task_suspend(&task);
    cat_scheduler_task_suspend_wakeup(&task);

    task.sched_strategy = CAT_SCHED_STRATEGY_EDF;
    cat_scheduler_task_rdy(&task);
    cat_scheduler_task_unrdy(&task);
    cat_scheduler_task_delay(&task, 0);
    cat_scheduler_task_delay_wakeup(&task);
    cat_scheduler_task_suspend(&task);
    cat_scheduler_task_suspend_wakeup(&task);

    cat_scheduler_schedule();

    return 0;
}