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
    struct _cat_dnode_t *prev;
    struct _cat_dnode_t *next;
} cat_dnode_t;

/**
 * @brief 差分链表
 */
typedef struct _cat_dlist_t
{
    cat_dnode_t head;
} cat_dlist_t;

void cat_dlist_init(cat_dlist_t *dlist);
void cat_dlist_node_init(cat_dnode_t *dnode);

void cat_dlist_add(cat_dlist_t *dlist, cat_dnode_t *new);

cat_dnode_t *cat_dlist_first(cat_dlist_t *dlist);

cat_dnode_t *cat_dlist_remove(cat_dnode_t *dnode);


#endif /* #ifndef CAT_DLIST_HCAT_DLIST_H */