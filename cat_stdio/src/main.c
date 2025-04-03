
#include "cat_stdio.h"
#include <stdio.h>


char buffer[100];

// 测试函数
// 测试代码
int main() {
    cat_float tf = 32.1234;
    
    // 测试cat_sprintf
    cat_sprintf(buffer, "int:%4d, hex:0x%x, float:%8.2f, char:%-3c, str:%12s ,frac:%%, undef:%z", -42, 0xdeadbeaf, tf, 'j', "string", "unused");
    printf("cat_sprintf: %s\n", buffer);

    cat_printf("cat_printf:  int:%4d, hex:0x%x, float:%8.2f, char:%-3c, str:%12s ,frac:%%, undef:%z", -42, 0xdeadbeaf, tf, 'j', "string", "unused");
    
    return 0;
}
