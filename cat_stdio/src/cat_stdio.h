/**
 * @file cat_stdio.h
 * @author mio_wen (648137125@qq.com)
 * @brief 标准输入输出
 * @version 0.1
 * @date 2022-07-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef CAT_STDIO_H
#define CAT_STDIO_H

#include <stdarg.h>
// #include "cat_intr.h"

// #include "catos_config.h"
#include "catos_types.h"

#define CAT_STDIO_NUM_STR_BUF_SIZE    64

#define CATOS_STDIO_ENABLE 1
#if (CATOS_STDIO_ENABLE == 1)

/**
 * @brief 设置stdio输入输出设备
 * 
 * @param  name             设备名称
 * @return cat_u8          成功失败
 */
cat_u8 cat_stdio_set_device(const cat_u8 *name);

/**
 * @brief 检查stdio设备是否设置
 * 
 * @return cat_u8 0：未设置
 *                 1：已设置
 */
cat_u8 cat_stdio_is_device_is_set(void);

char    cat_getchar(void);
cat_i32 cat_putchar(char c);
cat_i32 cat_vprintf(const char *format, va_list args);
cat_i32 cat_printf(const char *format, ...);
cat_i32 cat_sprintf(char *str, const char *format, ...);

cat_i32 cat_kprintf(const char *format, ...);

#endif /* #define CATOS_STDIO_ENABLE 1 */
#endif /* #define CAT_STDIO_H */
