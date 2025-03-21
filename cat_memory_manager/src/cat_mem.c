/**
 * @file cat_mem.c
 * @brief 
 * @author mio (648137125@qq.com)
 * @version 1.0
 * @date 2024-04-03
 * Change Logs:
 * Date           Author        Notes
 * 2024-04-03     mio     first verion
 * 
 */
#include "catos_types.h"

#define CPU_WIDES           32
/* 使能该项以使用空闲堆空间 */
#define USE_HEAP_SECTION    0


#define MCB_MAGIC_MASK  (0xffff0000)
#define MCB_MAGIC       (0xAAAA << 16)

#define MCB_STATUS_MASK (0x0000ffff)
#define MCB_USED        (1 << 0)
#define MCB_LOCKED      (1 << 1)       

typedef struct _mcb
{
    cat_u32      flags;  /**< 内存块标志,包含魔数和状态字 */
    cat_u32      size;   /**< 内存块除去控制块后实际可用大小(字节) */
                         /**< 链表节点 */
    struct _mcb *prev;
    struct _mcb *next;
}_mem_ctl_blk_t; /* 内存控制块结构体 */

_mem_ctl_blk_t mem_head;

/**
 * @brief 初始化动态内存
 * 
 * @param  start_addr       动态内存起始地址
 * @param  bytes            总大小
 * @return cat_err 
 */
cat_err cat_mem_init(cat_ubase start_addr, cat_ubase bytes)
{
    cat_err ret = CAT_ERROR;

    if(CAT_NULL != (void *)start_addr)
    {
        /* 后期考虑是否要先对齐 */
        _mem_ctl_blk_t *mcb = (_mem_ctl_blk_t *)start_addr;
        mcb->flags = MCB_MAGIC;
        mcb->size  = bytes - sizeof(_mem_ctl_blk_t);

        /* 将内存块挂到链表上 */
        mem_head.next = mcb;
        mem_head.prev = mcb;

        mcb->next = &mem_head;
        mcb->prev = &mem_head;

        ret = CAT_EOK;
    }

    return ret;
}

/**
 * @brief 分配空间
 * 
 * @param  p_mem_var        用户存储分配到的空间的变量地址
 *         如果申请成功则该变量的值会不为空，否则为空
 * @param  bytes            申请的空间大小
 * @return cat_err        0：成功 else：失败
 * 
 * 例子：
 *    cat_u32 *mem = CAT_NULL;
 *    cat_malloc(&mem, 64);
 *    if(CAT_NULL == mem)
 *    {
 *       分配失败
 *    }
 * 
 */
cat_err cat_malloc(void **p_mem_var, cat_ubase bytes)
{
    cat_err ret = CAT_ERROR;
    _mem_ctl_blk_t *cur_mcb = mem_head.next;
    _mem_ctl_blk_t *new_mcb = CAT_NULL;

    cat_ubase left_block_size = 0;

    *p_mem_var = CAT_NULL;

    /* 4字节对齐 */
    bytes += 3;
    bytes &= ~0x3;

    if(NULL != p_mem_var)
    {
        while(cur_mcb != &mem_head)
        {
            if(
                ((cur_mcb->flags & MCB_MAGIC_MASK) == MCB_MAGIC) &&
                ((cur_mcb->flags & MCB_USED) == 0) &&
                (cur_mcb->size >= bytes)
            )
            {
                /* 设置用户变量 */
                *p_mem_var = (cat_ubase)cur_mcb + sizeof(_mem_ctl_blk_t);

                /* 更新当前块为正在使用 */
                cur_mcb->flags |= MCB_USED;

                /* 如果剩下的大小还能存下一个控制块+一个4字节数据.就作为一个单独空闲块 */
                if(cur_mcb->size - bytes >= sizeof(_mem_ctl_blk_t) + 4)
                {
                    /* 设置剩下空间划分出来的空闲节点 */
                    new_mcb = (cat_ubase)cur_mcb + sizeof(_mem_ctl_blk_t) + bytes;
                    new_mcb->flags = MCB_MAGIC;
                    new_mcb->size  = cur_mcb->size - bytes - sizeof(_mem_ctl_blk_t);

                    /* 更新当前块的大小 */
                    cur_mcb->size = bytes;

                    /* 挂进链表 */
                    new_mcb->next = cur_mcb->next;
                    cur_mcb->next->prev = new_mcb;

                    cur_mcb->next = new_mcb;
                    new_mcb->prev = cur_mcb;
                }

                ret = CAT_EOK;
                break;
            }
            else
            {
                cur_mcb = cur_mcb->next;
            }
        }

    }


    return ret;
}

cat_err cat_free(void *ptr)
{
    cat_err ret = CAT_ERROR;

    _mem_ctl_blk_t *mcb = (_mem_ctl_blk_t *)(ptr - sizeof(_mem_ctl_blk_t));
    _mem_ctl_blk_t *neighbor = CAT_NULL;
    
    if(
        ((mcb->flags & MCB_MAGIC_MASK) == MCB_MAGIC) &&
        ((mcb->flags & MCB_USED) == 1) /* 不空闲才能释放 */
    )
    {
        /* 清除使用中标志 */
        mcb->flags &= ~MCB_USED;

        neighbor = mcb->next;
        if(
            ((neighbor->flags & MCB_MAGIC_MASK) == MCB_MAGIC) &&
            ((neighbor->flags & MCB_USED) == 0)
        )
        {
            /* 后一块是空闲块 */
            /* 合并 */
            mcb->next = neighbor->next;
            neighbor->next->prev = mcb;
            
            mcb->size += sizeof(_mem_ctl_blk_t) + neighbor->size;
        }

        neighbor = mcb->prev;
        if(
            ((neighbor->flags & MCB_MAGIC_MASK) == MCB_MAGIC) &&
            ((neighbor->flags & MCB_USED) == 0)
        )
        {
            /* 前一块是空闲块 */
            /* 合并 */
            neighbor->next = mcb->next;
            mcb->next->prev = neighbor;

            neighbor->size += sizeof(_mem_ctl_blk_t) + mcb->size;
        }

        ret = CAT_EOK;
    }
    else
    {
        /* 说明出错了 */
        printf("ERROR: BLOCK FLUSHED OR NOT IN USE\n");
    }

    return ret;
}

void cat_mem_print_info(void)
{
    _mem_ctl_blk_t *mcb = mem_head.next;
    int i = 0;

    printf("id      addr_range                 size  status------\n");
  /*printf("blk%4d:[0x%o, 0x%o] %5u %s\n"*/
    while(mcb != &mem_head)
    {
        printf("blk%4d:[0x%o, 0x%o] %5u   %s\n",
            i,
            (cat_ubase)(mcb + sizeof(_mem_ctl_blk_t)),
            (cat_ubase)(mcb + sizeof(_mem_ctl_blk_t) + mcb->size),
            mcb->size,
            ((mcb->flags & MCB_USED) == 0) ? "FREE": "USED"
        );

        i++;
        mcb = mcb->next;
    }
    printf("\n");
}
