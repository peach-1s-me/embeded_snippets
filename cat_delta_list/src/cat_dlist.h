/**
 * @file cat_ringbuffer.h
 * @author 文佳源 (648137125@qq.com)
 * @brief 差分链表头文件
 * @version 0.1
 * @date 2023-06-17
 * 
 * Copyright (c) 2023
 * 
 * @par 修订历史
 * <table>
 * <tr><th>版本 <th>作者 <th>日期 <th>修改内容
 * <tr><td>v1.0 <td>文佳源 <td>2023-06-17 <td>新建文件
 * </table>
 */
#ifndef CAT_DLIST_H
#define CAT_DLIST_H

#include "catos_types.h"

#define CAT_DLIST_INVAL 0xffffffff /* 差分链表节点初始值，非法 */
/**
 * @brief 差分链表节点
 */
typedef struct _cat_dnode_t
{
    cat_u32 value;
    struct _cat_dnode_t *next;
} cat_dnode_t;

/**
 * @brief 差分链表
 */
typedef struct _cat_dlist_t
{
    cat_dnode_t head;
} cat_dlist_t;

/**
 * @brief 初始化差分链表
 * 
 * @param[in] dlist    差分链表指针
 */
static inline void cat_dlist_init(cat_dlist_t *dlist)
{
    dlist->head.value = CAT_DLIST_INVAL;
    dlist->head.next = CAT_NULL;
}

/**
 * @brief 差分链表节点初始化
 * 
 * @param[in] dnode    差分链表节点指针
 */
static inline void cat_dlist_node_init(cat_dnode_t *dnode)
{
    dnode->value = CAT_DLIST_INVAL;
    dnode->next = CAT_NULL;
}

/**
 * @brief 差分链表添加节点
 * 
 * @param[in] dlist    差分链表指针
 * @param[in] dnode    节点指针
 */
static inline void cat_dlist_add(cat_dlist_t *dlist, cat_dnode_t *dnode)
{
    cat_dnode_t *cur = dlist->head.next, *pre = &(dlist->head);

    while(CAT_NULL != cur)
    {
        if(dnode->value < cur->value)
        {
            /* 找到位置了，退出循环 */
            cur->value -= dnode->value;
            break;
        }
        else
        {
            /* 还没找到位置，遍历往后找 */
            dnode->value -= cur->value;

            /* 往后移动 */
            pre = cur;
            cur = cur->next;
        }
    }

    /* 插入节点 */
    pre->next  = dnode;
    dnode->next = cur;
}

/**
 * @brief 获取差分链表第一个节点(不取出)
 * 
 * @param[in] dlist    差分链表指针
 * @return cat_dnode_t* 取出的节点指针
 */
static inline cat_dnode_t *cat_dlist_first(cat_dlist_t *dlist)
{
    return dlist->head.next;
}

/**
 * @brief 取出差分链表第一个节点(最小的)
 * 
 * @param[in] dlist    差分链表指针
 * @return cat_dnode_t* 取出的节点指针
 */
static inline cat_dnode_t *cat_dlist_pop(cat_dlist_t *dlist)
{
    cat_dnode_t *node = dlist->head.next;

    /* 取出节点 */
    dlist->head.next = node->next;
    node->next = CAT_NULL;

    if(node->value > 0)
    {
        /* 如果取出的节点值大于零, 则取出后的首节点还需要减去该值 */
        dlist->head.next->value += node->value;
    }

    return node;
}

#endif /* #ifndef CAT_DLIST_HCAT_DLIST_H */