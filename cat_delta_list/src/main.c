#include <stdio.h>

#include <stdlib.h>
#include <time.h>

#include "cat_dlist.h"

#define NODE_NUM 10

cat_dlist_t dl;
cat_dnode_t n[NODE_NUM] = {0};

void print_dlist(cat_dlist_t *dl)
{
    cat_dnode_t *p = dl->head.next;

    cat_u32 real_value = 0;

    while(CAT_NULL != p)
    {
        real_value += p->value;
        printf("%d(%d)->", real_value, p->value);
        
        p = p->next;
    }
    printf("END\r\n");
}


int main(void)
{
    unsigned char i = 0;

    srand(time(NULL));

    /* 初始化差分链表 */
    cat_dlist_init(&dl);
    for(i=0; i<NODE_NUM; i++)
    {
        cat_dlist_node_init(&n[i]);
        n[i].value = rand() % 100;
    }

    printf("== TEST ADD ==\n");
    for(i=0; i<NODE_NUM; i++)
    {
        printf("add %d\r\n", n[i].value);
        cat_dlist_add(&dl, &n[i]);
        print_dlist(&dl);
    }
    

    printf("== TEST POP ==\n");
    cat_dnode_t *tmp;
    tmp = cat_dlist_pop(&dl);
    printf("pop %d\r\n", tmp->value);
    tmp = cat_dlist_pop(&dl);
    printf("pop %d\r\n", tmp->value);
    tmp = cat_dlist_pop(&dl);
    printf("pop %d\r\n", tmp->value);
    print_dlist(&dl);

    return 0;
}