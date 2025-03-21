#include "cat_scheduler.h"


static void edf_scheduler_init(void);
static void edf_scheduler_task_create_static(void *task_config);
static void edf_scheduler_deal_in_tick(void);
static cat_uint8_t edf_scheduler_has_task_rdy(void);
static void edf_scheduler_sched(void);

static void edf_scheduler_task_rdy(struct _cat_task_t *task);
static void edf_scheduler_task_unrdy(struct _cat_task_t *task);
static void edf_scheduler_task_delay(struct _cat_task_t *task, cat_uint32_t tick);
static void edf_scheduler_task_delay_wakeup(struct _cat_task_t *task);
static void edf_scheduler_task_suspend(struct _cat_task_t *task);
static void edf_scheduler_task_suspend_wakeup(struct _cat_task_t *task);


static void edf_scheduler_init(void)
{
    printf("%s\n", __func__);
}
static void edf_scheduler_task_create_static(void *task_config)
{
    printf("%s\n", __func__);
}
static void edf_scheduler_deal_in_tick(void)
{
    printf("%s\n", __func__);
}
static cat_uint8_t edf_scheduler_has_task_rdy(void)
{
    printf("%s\n", __func__);
    return 1;
}
static void edf_scheduler_sched(void)
{
    printf("%s\n", __func__);
}

static void edf_scheduler_task_rdy(struct _cat_task_t *task)
{
    printf("%s\n", __func__);
}
static void edf_scheduler_task_unrdy(struct _cat_task_t *task)
{
    printf("%s\n", __func__);
}
static void edf_scheduler_task_delay(struct _cat_task_t *task, cat_uint32_t tick)
{
    printf("%s\n", __func__);
}
static void edf_scheduler_task_delay_wakeup(struct _cat_task_t *task)
{
    printf("%s\n", __func__);
}
static void edf_scheduler_task_suspend(struct _cat_task_t *task)
{
    printf("%s\n", __func__);
}
static void edf_scheduler_task_suspend_wakeup(struct _cat_task_t *task)
{
    printf("%s\n", __func__);
}

CAT_SCHEDULER_DECLARE(edf) = {
    .strategy               = CAT_SCHED_STRATEGY_EDF,          
    .scheduler_prio         = CAT_SCHEDULER_PRIO_EDF,   

    .scheduler_name         = "edf",  
     
    .scheduler_init         = edf_scheduler_init,
    .task_create_static     = edf_scheduler_task_create_static,
    .deal_in_tick           = edf_scheduler_deal_in_tick,
    .has_task_rdy           = edf_scheduler_has_task_rdy,
    .schedule               = edf_scheduler_sched,

    .task_rdy               = edf_scheduler_task_rdy,
    .task_unrdy             = edf_scheduler_task_unrdy,
    .task_delay             = edf_scheduler_task_delay,
    .task_delay_wakeup      = edf_scheduler_task_delay_wakeup,
    .task_suspend           = edf_scheduler_task_suspend,
    .task_suspend_wakeup    = edf_scheduler_task_suspend_wakeup,
};
