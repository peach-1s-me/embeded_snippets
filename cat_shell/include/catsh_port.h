/**
 * @file catsh_port.h
 * @brief 
 * @author mio (648137125@qq.com)
 * @version 1.0
 * @date 2024-06-05
 * Change Logs:
 * Date           Author        Notes
 * 2024-06-05     mio     first verion
 * 
 */
#ifndef _CATSH_PORT_H
#define _CATSH_PORT_H

#include "cat_types.h"

typedef cat_i8     catsh_i8;
typedef cat_i16    catsh_i16;
typedef cat_i32    catsh_i32;
typedef cat_u8     catsh_u8;
typedef cat_u16    catsh_u16;
typedef cat_u32    catsh_u32;
typedef cat_ibase  catsh_ibase;
typedef cat_ubase  catsh_ubase;


typedef catsh_u8   catsh_err;
#define CATSH_EOK       0x0
#define CATSH_ERROR     0x1

#define CATSH_NULL CAT_NULL

#include <stdio.h>
#include <string.h>
#define CATSH_PUTC(c) putchar(c)
#define CATSH_PRINTF(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define CATSH_STRCMP(str1, str2) strcmp(str1, str2)


#endif
