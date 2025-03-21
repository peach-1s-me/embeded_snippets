/**
 * @file cat_scheduler.c
 * @author 文佳源 (648137125@qq.com)
 * @brief 调度器源文件
 * @version 0.1
 * @date 2023-07-01
 * 
 * Copyright (c) 2023
 * 
 * @par 修订历史
 * <table>
 * <tr><th>版本 <th>作者 <th>日期 <th>修改内容
 * <tr><td>v1.0 <td>文佳源 <td>2023-07-01 <td>创建
 * </table>
 */
#include "cat_scheduler.h"
#include "cat_error.h"

IMPORT_SECTION(cat_scheduler_sec)

/* 遍历调度器列表 */
#define CAT_FOREACH_SCHEDULER(_temp) \
    for(_temp =  (cat_scheduler_t*)SECTION_START(cat_scheduler_sec); \
        _temp != (cat_scheduler_t*)SECTION_END(cat_scheduler_sec); \
        _temp ++)


void print_scheduler(void)
{
    cat_scheduler_t *p = NULL;
    cat_uint32_t i = 0;

    CAT_FOREACH_SCHEDULER(p)
    {
        printf("%d: name=%s, prio=%u\r\n", i, p->scheduler_name, p->scheduler_prio);
        i++;
    }
}

/**
 * @brief 根据策略获取调度器
 * 
 * @param  strategy         调度策略
 * @return cat_scheduler_t* 调度器结构体指针
 */
static inline cat_scheduler_t *cat_scheduler_get_by_strategy(cat_uint8_t strategy)
{
    cat_scheduler_t *ret = NULL, *p = NULL;

    CAT_FOREACH_SCHEDULER(p)
    {
        if(strategy == p->strategy)
        {
            ret = p;
            //break;
        }
    }

    return ret;
}


void cat_scheduler_init_all_scheduler(void)
{
    cat_scheduler_t *p = NULL;

    CAT_FOREACH_SCHEDULER(p)
    {
        CAT_ASSERT(NULL != p);

        if(NULL != p->scheduler_init)
        {
            p->scheduler_init();
        }
    }
}
void cat_scheduler_create_task_static(cat_uint8_t strategy, void *task_config)
{
    cat_scheduler_t *scheduler = cat_scheduler_get_by_strategy(strategy);

    CAT_ASSERT(NULL != scheduler);
    CAT_ASSERT(NULL != scheduler->task_create_static);

    scheduler->task_create_static(task_config);
}

void cat_scheduler_deal_in_tick(void)
{
    cat_scheduler_t *p = NULL;

    CAT_FOREACH_SCHEDULER(p)
    {
        CAT_ASSERT(NULL != p);

        if(NULL != p->deal_in_tick)
        {
            p->deal_in_tick();
        }
    }
}


void cat_scheduler_schedule(void)
{
    cat_scheduler_t *p = NULL, *highest_prio_scheduler = NULL;
    cat_uint8_t highest_prio = 0xff, has_rdy = 0;

    CAT_FOREACH_SCHEDULER(p)
    {
        CAT_ASSERT(NULL != p);
        CAT_ASSERT(NULL != p->has_task_rdy);

        has_rdy = p->has_task_rdy();

        if(
            (p->scheduler_prio < highest_prio) &&
            (0 != has_rdy)
        )
        {
            highest_prio_scheduler = p;
        }
    }

    CAT_ASSERT(NULL != highest_prio_scheduler);
    CAT_ASSERT(NULL != highest_prio_scheduler->schedule);

    highest_prio_scheduler->schedule();

    /* 实际使用中不会到达这里 */
    // while(1);
}


void cat_scheduler_task_rdy(struct _cat_task_t *task)
{
    CAT_ASSERT(task);

    cat_scheduler_t *scheduler = cat_scheduler_get_by_strategy(task->sched_strategy);

    CAT_ASSERT(NULL != scheduler);
    CAT_ASSERT(NULL != scheduler->task_rdy);

    scheduler->task_rdy(task);
}

void cat_scheduler_task_unrdy(struct _cat_task_t *task)
{
    CAT_ASSERT(task);

    cat_scheduler_t *scheduler = cat_scheduler_get_by_strategy(task->sched_strategy);

    CAT_ASSERT(NULL != scheduler);
    CAT_ASSERT(NULL != scheduler->task_unrdy);

    scheduler->task_unrdy(task);
}

void cat_scheduler_task_delay(struct _cat_task_t *task, cat_uint32_t tick)
{
    CAT_ASSERT(task);

    cat_scheduler_t *scheduler = cat_scheduler_get_by_strategy(task->sched_strategy);

    CAT_ASSERT(NULL != scheduler);
    CAT_ASSERT(NULL != scheduler->task_delay);

    /* 不要求 tick 大于零了，这样可以通过 delay(0) 来放弃cpu */
    scheduler->task_delay(task, tick);
}

void cat_scheduler_task_delay_wakeup(struct _cat_task_t *task)
{
    CAT_ASSERT(task);

    cat_scheduler_t *scheduler = cat_scheduler_get_by_strategy(task->sched_strategy);

    CAT_ASSERT(NULL != scheduler);
    CAT_ASSERT(NULL != scheduler->task_delay_wakeup);

    scheduler->task_delay_wakeup(task);
}

void cat_scheduler_task_suspend(struct _cat_task_t *task)
{
    CAT_ASSERT(task);

    cat_scheduler_t *scheduler = cat_scheduler_get_by_strategy(task->sched_strategy);

    CAT_ASSERT(NULL != scheduler);
    CAT_ASSERT(NULL != scheduler->task_suspend);

    scheduler->task_suspend(task);
}

void cat_scheduler_task_suspend_wakeup(struct _cat_task_t *task)
{
    CAT_ASSERT(task);

    cat_scheduler_t *scheduler = cat_scheduler_get_by_strategy(task->sched_strategy);

    CAT_ASSERT(NULL != scheduler);
    CAT_ASSERT(NULL != scheduler->task_suspend_wakeup);

    scheduler->task_suspend_wakeup(task);
}