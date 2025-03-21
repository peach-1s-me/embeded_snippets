#include "cat_scheduler.h"


static void static_prio_scheduler_init(void);
static void static_prio_scheduler_task_create_static(void *task_config);
static void static_prio_scheduler_deal_in_tick(void);
static cat_uint8_t static_prio_scheduler_has_task_rdy(void);
static void static_prio_scheduler_sched(void);

static void static_prio_scheduler_task_rdy(struct _cat_task_t *task);
static void static_prio_scheduler_task_unrdy(struct _cat_task_t *task);
static void static_prio_scheduler_task_delay(struct _cat_task_t *task, cat_uint32_t tick);
static void static_prio_scheduler_task_delay_wakeup(struct _cat_task_t *task);
static void static_prio_scheduler_task_suspend(struct _cat_task_t *task);
static void static_prio_scheduler_task_suspend_wakeup(struct _cat_task_t *task);


static void static_prio_scheduler_init(void)
{
    printf("%s\n", __func__);
}
static void static_prio_scheduler_task_create_static(void *task_config)
{
    printf("%s\n", __func__);
}
static void static_prio_scheduler_deal_in_tick(void)
{
    printf("%s\n", __func__);
}
static cat_uint8_t static_prio_scheduler_has_task_rdy(void)
{
    printf("%s\n", __func__);
    return 1;
}
static void static_prio_scheduler_sched(void)
{
    printf("%s\n", __func__);
}

static void static_prio_scheduler_task_rdy(struct _cat_task_t *task)
{
    printf("%s\n", __func__);
}
static void static_prio_scheduler_task_unrdy(struct _cat_task_t *task)
{
    printf("%s\n", __func__);
}
static void static_prio_scheduler_task_delay(struct _cat_task_t *task, cat_uint32_t tick)
{
    printf("%s\n", __func__);
}
static void static_prio_scheduler_task_delay_wakeup(struct _cat_task_t *task)
{
    printf("%s\n", __func__);
}
static void static_prio_scheduler_task_suspend(struct _cat_task_t *task)
{
    printf("%s\n", __func__);
}
static void static_prio_scheduler_task_suspend_wakeup(struct _cat_task_t *task)
{
    printf("%s\n", __func__);
}

CAT_SCHEDULER_DECLARE(static_prio)  = {
    .strategy               = CAT_SCHED_STRATEGY_STATIC_PRIO,          
    .scheduler_prio         = CAT_SCHEDULER_PRIO_STATIC_PRIO,     

    .scheduler_name         = "static_prio",
     
    .scheduler_init         = static_prio_scheduler_init,
    .task_create_static     = static_prio_scheduler_task_create_static,
    .deal_in_tick           = static_prio_scheduler_deal_in_tick,
    .has_task_rdy           = static_prio_scheduler_has_task_rdy,
    .schedule               = static_prio_scheduler_sched,

    .task_rdy               = static_prio_scheduler_task_rdy,
    .task_unrdy             = static_prio_scheduler_task_unrdy,
    .task_delay             = static_prio_scheduler_task_delay,
    .task_delay_wakeup      = static_prio_scheduler_task_delay_wakeup,
    .task_suspend           = static_prio_scheduler_task_suspend,
    .task_suspend_wakeup    = static_prio_scheduler_task_suspend_wakeup,
};
