#include <stdio.h>
#include "cat_device.h"
#include "test.h"

/* STATIC FUNCS DECL START */
static void print_device(cat_device_t *dev);
static void list_device(void);
/* STATIC FUNCS DECL END */
/* STATIC FUNCS DEF START */
static void print_device(cat_device_t *dev)
{
    printf("name=%s, type=%d, state=0x%x, aval_mode=0x%x, open_mode=0x%x, ref_count=%d, device_id=%d\n",
            dev->device_name,
            dev->type,
            dev->state,
            dev->aval_mode,
            dev->open_mode,
            dev->ref_count,
            dev->device_id
        );
}
static void list_device(void)
{
    cat_device_t *dev = NULL;
    cat_node_t *tmp = NULL;
    printf("\n*%d devices registerd*\n", cat_device_list.node_cnt);
    CAT_LIST_FOREACH(&cat_device_list, tmp)
    {
        dev = CAT_LIST_ENTRY(tmp, cat_device_t, link_node);
        
        print_device(dev);
    }
}
/* STATIC FUNCS DEF END */
/* TEST ID_ALLOC FREE */
#ifdef TEST
void test_device_id_alloc_free(void)
{
    int id = 0;
    int i = 0;

    printf("*** alloc 256 times ***\n");
    for(i = 0; i < 256; i++)
    {
        id = _cat_device_alloc_id();
        printf("i=%d, id=%d\n", i, id);
    }

#if 0
    printf("*** the 257nd time ***\n");
    id = _cat_device_alloc_id();
    printf("i=%d, id=%d\n", i, id);
#endif
    int ret = 0;
    printf("*** free 3 128 243 0 256 ***\n");
    ret += _cat_device_free_id(3);
    ret += _cat_device_free_id(128);
    ret += _cat_device_free_id(243);
    ret += _cat_device_free_id(0);
    ret += _cat_device_free_id(255);
    printf("ret = %d\n", ret);

    printf("*** alloc 5 times ***\n");
    for(i = 0; i < 5; i++)
    {
        id = _cat_device_alloc_id();
        printf("i=%d, id=%d\n", i, id);
    }

#if 1
    printf("*** free duplicated ***\n");
    ret = 0;
    ret += _cat_device_free_id(3);
    ret += _cat_device_free_id(3);
    printf("ret = %d\n", ret);
#endif
}
#endif

/* TEST_LIST */
typedef struct _test_node_t
{
    cat_node_t link_node;
    int val;
}test_node_t;

cat_list_t list;
test_node_t node_a = {
    .val = 243,
};
test_node_t node_b = {
    .val = 335
};
test_node_t node_c = {
    .val = 437
};
void test_list(void)
{
    

    cat_list_init(&list);

    cat_list_add_last(&list, &(node_a.link_node));
    cat_list_add_last(&list, &(node_b.link_node));
    cat_list_add_last(&list, &(node_c.link_node));

    test_node_t *tmp = NULL;
    cat_node_t  *cur_node = NULL;

    CAT_LIST_FOREACH(&list, cur_node)
    {
        tmp = CAT_LIST_ENTRY(cur_node, test_node_t, link_node);
        printf("%d\n", tmp->val);
    }
}

/* TEST_REGISTER */
uint8_t test_init   (cat_device_t *dev)
{
    printf("[%s]:dev=%s\n", __func__, dev->device_name);
    return CAT_EOK;
}
uint8_t test_open   (cat_device_t *dev, uint16_t open_mode)
{
    printf("[%s]:dev=%s\n", __func__, dev->device_name);

    /* 这里仅赋值操作，实际设备驱动中应该根据open_mode对硬件进行设置 */
    dev->open_mode = (open_mode & CAT_DEVICE_OPEN_MODE_MASK);

    return CAT_EOK;
}
uint8_t test_close  (cat_device_t *dev)
{
    printf("[%s]:dev=%s\n", __func__, dev->device_name);
    return CAT_EOK;
}
uint8_t test_read   (cat_device_t *dev, int32_t pos, void *buffer, uint32_t size)
{
    printf("[%s]:dev=%s\n", __func__, dev->device_name);
    return CAT_EOK;
}
uint8_t test_write  (cat_device_t *dev, int32_t pos, const void *buffer, uint32_t size)
{
    printf("[%s]:dev=%s\n", __func__, dev->device_name);
    return CAT_EOK;
}
uint8_t test_ctrl(cat_device_t *dev, int cmd, void *args)
{
    printf("[%s]:dev=%s, cmd=0x%x, arg(int)=%d\n", __func__, dev->device_name, cmd, *((int*)args));
    return CAT_EOK;
}

cat_device_t uart0_dev = {
    .type        = CAT_DEVICE_TYPE_CHAR,
    .init        = test_init,
    .open        = test_open,
    .read        = test_read,
    .write       = test_write,
    .ctrl        = test_ctrl,
};
cat_device_t uart1_dev = {
    .type        = CAT_DEVICE_TYPE_CHAR,
    .init        = test_init,
    .open        = test_open,
    .read        = test_read,
    .write       = test_write,
    .ctrl        = test_ctrl,
};
cat_device_t iic0_dev = {
    .type        = CAT_DEVICE_TYPE_IIC,
    .init        = test_init,
    .open        = test_open,
    .read        = test_read,
    .write       = test_write,
    .ctrl        = test_ctrl,
};
cat_device_t screen0_dev = {
    .type        = CAT_DEVICE_TYPE_GRAPHIC,
    .init        = test_init,
    .open        = test_open,
    .read        = test_read,
    .write       = test_write,
    .ctrl        = test_ctrl,
};

void test_device_register(void)
{
    printf("\n*** 1 test device_register ***\n");
    int err = 0;
    err += cat_device_register(&uart0_dev,   "uart0",   CAT_DEVICE_MODE_RDWR   | CAT_DEVICE_MODE_INT_RX);
    err += cat_device_register(&uart1_dev,   "uart1",   CAT_DEVICE_MODE_RDWR   | CAT_DEVICE_MODE_INT_RX);
    err += cat_device_register(&iic0_dev,    "iic0",    CAT_DEVICE_MODE_RDWR);
    err += cat_device_register(&screen0_dev, "screen0", CAT_DEVICE_MODE_WRONLY | CAT_DEVICE_MODE_DMA_TX);
    printf("err = %d\n", err);

    list_device();
}

void test_device_open_close(void)
{
    printf("\n*** 2 test device_open_close ***\n");
    cat_device_open(&uart0_dev, CAT_DEVICE_MODE_RDWR   | CAT_DEVICE_MODE_INT_RX);
    cat_device_open(&uart0_dev, CAT_DEVICE_MODE_RDWR   | CAT_DEVICE_MODE_INT_RX);
    cat_device_open(&iic0_dev, CAT_DEVICE_MODE_RDWR);
    list_device();

    cat_device_close(&uart0_dev);
    cat_device_close(&uart0_dev);
    cat_device_close(&iic0_dev);
    list_device();
}

void test_device_read_write(void)
{
    printf("\n*** 3 test device_read_write ***\n");
    cat_device_open(&uart0_dev, CAT_DEVICE_MODE_RDWR   | CAT_DEVICE_MODE_INT_RX);
    cat_device_open(&iic0_dev, CAT_DEVICE_MODE_RDWR);
    list_device();

    cat_device_read(&uart0_dev, 0, NULL, 0);
    cat_device_read(&iic0_dev, 0, NULL, 0);

    cat_device_write(&uart0_dev, 0, NULL, 0);
    cat_device_write(&iic0_dev, 0, NULL, 0);

    cat_device_close(&uart0_dev);
    cat_device_close(&iic0_dev);
    list_device();
}

void test_device_ctrl(void)
{
    printf("\n*** 4 test device_open_close ***\n");
    cat_device_open(&uart0_dev, CAT_DEVICE_MODE_RDWR   | CAT_DEVICE_MODE_INT_RX);
    cat_device_open(&iic0_dev, CAT_DEVICE_MODE_RDWR);
    list_device();

    int a = 5, b = 6;
    cat_device_ctrl(&uart0_dev, 0x2, (void*)&a);
    cat_device_ctrl(&iic0_dev, 0x4, (void*)&b);

    cat_device_close(&uart0_dev);
    cat_device_close(&iic0_dev);
    list_device();
}

void test_device_unregister(void)
{
    printf("\n*** test device_unregister ***\n");
    int err = 0;
    err += cat_device_unregister(&uart0_dev);
    err += cat_device_unregister(&uart1_dev);
    err += cat_device_unregister(&iic0_dev);
    err += cat_device_unregister(&screen0_dev);
    printf("err = %d\n", err);

    list_device();
}

int main(int argc, char **argv)
{
    printf("************* TEST DEVICE MODEL ****************\n");
    cat_device_module_init();

#ifdef TEST
    test_device_id_alloc_free();
#endif

#if 0
    test_list();
#endif

#if 1
    test_device_register();
#endif


    test_device_open_close();

    test_device_read_write();

    test_device_ctrl();

#if 1
    test_device_unregister();
#endif

    return 0;
}