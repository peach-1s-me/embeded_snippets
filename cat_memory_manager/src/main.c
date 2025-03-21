#include <stdio.h>

#include "cat_mem.h"

#define TEST_TITLE(info) \
    do{ \
        printf("========= TEST %s =========\n", #info); \
    }while(0)

#define DINAMIC_MEM_SIZE    2048
cat_u8 mem_space[DINAMIC_MEM_SIZE];

cat_ubase mem_end = (cat_ubase)mem_space + DINAMIC_MEM_SIZE - 1;

int main(int argc, char **argv)
{
    cat_u8 *mem1, *mem2, *mem3, *mem4, *mem5;

    cat_mem_init((cat_ubase)mem_space, DINAMIC_MEM_SIZE);

    TEST_TITLE(malloc);
    cat_malloc((void**)&mem1, 512);
    cat_malloc((void**)&mem2, 64);
    cat_malloc((void**)&mem3, 1024);
    cat_malloc((void**)&mem4, 128);
    cat_mem_print_info();

    TEST_TITLE(free);
    cat_free(mem2);
    cat_mem_print_info();

    TEST_TITLE(fragment);
    cat_malloc(&mem5, 256);
    if(CAT_NULL == mem5)
    {
        printf("fail to malloc mem5:256bytes\n");
    }
    cat_mem_print_info();

    return 0;
}