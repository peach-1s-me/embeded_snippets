/**
 * @file cat_dlist.c
 * @author 文佳源 (648137125@qq.com)
 * @brief 差分链表源文件
 * @version 0.1
 * @date 2025-04-11
 * 
 * Copyright (c) 2025
 * 
 * @par 修订历史
 * <table>
 * <tr><th>版本 <th>作者 <th>日期 <th>修改内容
 * <tr><td>v1.0 <td>文佳源 <td>2025-04-11 <td>内容
 * </table>
 */
#include "cat_dlist.h"

/**
 * @brief 初始化差分链表
 * 
 * @param[in] dlist    差分链表指针
 */
void cat_dlist_init(cat_dlist_t *dlist)
{
    dlist->head.value = CAT_DLIST_INVAL;
    dlist->head.prev  = &(dlist->head);
    dlist->head.next  = &(dlist->head);
}

/**
 * @brief 差分链表节点初始化
 * 
 * @param[in] dnode    差分链表节点指针
 */
void cat_dlist_node_init(cat_dnode_t *dnode)
{
    dnode->value = CAT_DLIST_INVAL;
    dnode->prev  = dnode;
    dnode->next  = dnode;
}

/**
 * @brief 差分链表添加节点
 * 
 * @param[in] dlist    差分链表指针
 * @param[in] new    节点指针
 */
void cat_dlist_add(cat_dlist_t *dlist, cat_dnode_t *new)
{
    cat_dnode_t *after = dlist->head.next;

    while(&(dlist->head) != after)
    {
        if(new->value < after->value)
        {
            /* 找到位置了，退出循环 */
            after->value -= new->value;
            break;
        }
        else
        {
            /* 还没找到位置，遍历往后找 */
            new->value -= after->value;

            /* 往后移动 */
            after = after->next;
        }
    }

    /* 插入节点 */
    new->prev = after->prev;
    new->next = after;
    after->prev->next = new;
    after->prev = new;
}

/**
 * @brief 获取差分链表第一个节点(不取出)
 * 
 * @param[in] dlist    差分链表指针
 * @return cat_dnode_t* 取出的节点指针
 */
cat_dnode_t *cat_dlist_first(cat_dlist_t *dlist)
{
    cat_dnode_t *first = CAT_NULL;
    if(&(dlist->head) != dlist->head.next)
    {
        return dlist->head.next;
    }

    return first;
}

/**
 * @brief 从差分链表移除节点
 * 
 * @param[in] dnode    要移除的节点指针
 */
cat_dnode_t *cat_dlist_remove(cat_dnode_t *dnode)
{
    if(dnode->value > 0)
    {
        /* 如果取出的节点值大于零, 则取出后的首节点还需要减去该值 */
        dnode->next->value += dnode->value;
    }

    dnode->next->prev = dnode->prev;
    dnode->prev->next = dnode->next;

    dnode->prev = dnode;
    dnode->next = dnode;

    return dnode;
}
