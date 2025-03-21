## 关于内存碎片整理的想法

### 核心思想：在alloc和free时跟踪存储内存首地址的变量地址

```c
typedef struct
{
    uint32_t *p_addr;/* 使用者申请到内存后保存到的变量的地址 */
    uint32_t size;   /* 内存大小 */
    //uint32_t ideal_max_use_time; /* 理想中从申请到释放的最大时间 */
    //uint32_t used_time; /* 从申请到当前时刻经过的时间 */
    //uint32_t free_lock; /* 是否允许释放 */
}mem_block_info;

cat_list_t blk_list;

err alloc(mem_block_info *blk_info, uint32_t *p_addr, uint32_t size)
{
    /* 分配内存, 首地址为ADDR */

    /* 修改blk_info的值 */
    ...
    blk_info->p_addr = p_addr;/* 记录变量地址 */
    ...

    register_block(blk_info);/* 按起始顺序挂到管理链表中 */
    *p_addr = (uint32_t)ADDR；
}

err free(void *start_addr)
{
    /* 在已分配的内存块链表中搜索目标块 */
    mem_block_info *blk_info = find_mem_blk_info(start_addr);

    if(blk_info != NULL)
    {
        /* 内存块是已经分配且未释放的 */

        /* 释放内存 */
    }
    else
    {
        /* 内存块不存在或者已经释放 */
        if(/* 不允许重复释放 */)
        {
            /* 处理重复释放的错误 */
        }
    }
}

err mem_arange(void)
{
    uint32_t pre_blk_end_addr = FIRST_BLOCK_END_ADDR;

    /* 从第二块内存开始遍历blk_list */
    for(/* 假设遍历的变量为 mem_block_info *blk_info */)
    {
        if(/* 该内存块起始地址和上一块内存结束地址空隙满足调整(如对齐)要求 */)
        {
            /* 处理对齐 */
            uint32_t aligned_start_addr = pre_blk_end_addr & 0xff;
            ALIGN_BYTES_TYPE p, q;

            /* 将内存块内容拷贝到满足要求的地址 */
            for(p = aligned_start_addr, q = cur_blk_start_addr;
                p++, q++;
                q != cur_blk_end_addr
            )
            {
                *p = *q;
            }

            /* 核心!! 修改申请内存时保存起始地址的指针变量的值 */
            *(mem_block_info->p_addr) = aligned_start_addr;
        }
        
    }
}
```

给出一个实例,例如用户申请了三块内存.假设内存分配为4字节对齐`((addr+(1<<4-1))&~0xf)`

```c
task1()
{
    mem_block_info info1, info2, info3;
    uint32_t *mem1, *mem2, *mem3;

    alloc(&info1, &mem1, 61);
    alloc(&info2, &mem2, 15);
    alloc(&info3, &mem3, 32);

    /* 
     * 此时内存分配情况为:
       mem1:[0-60], 61bytes
       gap1: [61-63], 3bytes
       mem2:[64-78], 15bytes
       gap2:[79-79], 1byte
       mem3:[80-111], 32bytes
     */

     /* 
       此时各变量值为
       mem1 : 0x00000000
       mem2 : 0x00000040
       mem3 : 0x00000050
      */


    free(mem2);

    /* 
     * 此时内存分配情况为:
       mem1:[0-60], 61bytes
       gap1: [61-79], 19byte
       mem3:[80-111], 32bytes
     */

     /* 若此刻之后分配的内存都大于19bytes,则gap1所在内存就无法利用 */

    arrange();

    /* 
     * 此时内存分配情况为:
       mem1:[0-60], 61bytes
       gap1: [61-63], 3byte
       mem3:[64-95], 32bytes
     */

     /* 
       此时各变量值为
       mem1 : 0x00000000
       mem3 : 0x00000050
      */

      /* 此时设每个空隙(gap)大小为gi,则有 gi%align 大小的空间可以被拼到
         后面的空闲空间供分配使用  
       */

    

}
```
