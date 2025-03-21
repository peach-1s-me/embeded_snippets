/**
 * @file catos_defs.h
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-17
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-17 1.0    amoigus             基本类型定义
 */

#ifndef CATOS_DEF_H
#define CATOS_DEF_H

#include <stdio.h>

/************** MACROS*********************/
#define U32_MAX_VAL 0xffffffff

/* 基本数据类型 */
typedef signed char     cat_i8;
typedef short int       cat_i16;
typedef int             cat_i32;

typedef unsigned char           cat_u8;
typedef unsigned short int      cat_u16;
typedef unsigned int            cat_u32;

typedef float                   cat_float;
typedef double                  cat_double;


/* 最高位宽数据类型 */
typedef long                    cat_base;
typedef unsigned long           cat_ubase;

/* 布尔值定义 */
typedef cat_u8                  cat_bool; /* 布尔值 */
#define CAT_TRUE                (1)
#define CAT_FALSE               (0)


/* 错误代码定义 */
typedef cat_base                cat_err;  /* 错误类型 */
#define CAT_EOK                 (0)     /**< 成功 */
#define CAT_ERROR               (1)     /**< 失败 */
#define CAT_EINVAL              (2)     /**< 非法值 */

typedef cat_u8 cat_stack_type_t;   /* 堆栈类型 */

/* 空指针 */
#define CAT_NULL                ((void *)0)


/* 对齐 */
/**
 * @brief 向上取整对齐
 */
#define CAT_ALIGN(addr, align) \
    (((addr) + (align) - 1) & ~((align) - 1))
/**
 * @brief 向下取整对齐
 */
#define CAT_ALIGN_DOWN(addr, align) \
    (((cat_uint32_t)addr) & ~(((cat_uint32_t)align) - 1))

/************** struct type*********************/
/* cat_list.c */
typedef struct _cat_node_t     cat_node_t;
typedef struct _cat_list_t     cat_list_t;

/* cat_device.c */
typedef enum _cat_device_type_t         cat_device_type_t;      /**< 设备类型 */
typedef struct _cat_device_t            cat_device_t;           /**< 设备结构体 */


#endif


