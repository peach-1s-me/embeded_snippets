
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "catsh.h"

catsh_instance_t test_inst;
catsh_u8 *buffer[1024];

int main(int argc, char **argv)
{
    printf("sizeof(cmd)=%u, sizeof(key)=%u\r\n", sizeof(catsh_cmd_t), sizeof(catsh_key_t));

    struct
    {
         catsh_key_t *key_base;                      /**< 按键列表起始地址 */
        catsh_u16    key_num; 
    } buffer_desc; /**< 缓冲区状态描述 */
    printf("%lu\r\n", sizeof(buffer_desc));

    printf("u8=%lu\r\n", sizeof(catsh_u8));
    printf("u16=%lu\r\n", sizeof(catsh_u16));
    printf("u32=%lu\r\n", sizeof(catsh_u32));
    printf("u8*=%lu\r\n", sizeof(catsh_u8*));

    catsh_init(&test_inst, buffer, 1024);

    catsh_greeting(&test_inst);

    printf("cat>");

    while(1)
    {
        char c = getch();
        catsh_handle_input_char(&test_inst, c);
    }

    return 0;
}

void *do_hello(void *arg)
{
    (void)arg;
	
    printf("hello\r\n");

	return NULL;
}
CATSH_DECLARE_CMD(hello, print hello, do_hello);

void *do_exit(void *arg)
{
    (void)arg;

    exit(0);

    return NULL;
}
CATSH_DECLARE_CMD(exit, exit the prog, do_exit);
