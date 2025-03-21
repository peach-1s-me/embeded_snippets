/**
 * @file catsh.h
 * @brief 
 * @author mio (648137125@qq.com)
 * @version 1.0
 * @date 2024-06-05
 * Change Logs:
 * Date           Author        Notes
 * 2024-06-05     mio     first verion
 * 
 */
#ifndef _CATSH_H
#define _CATSH_H

#include "catsh_port.h"

/* 1-CONFIG start */
#define CATSH_CPU_64BIT             (1)   /**< 64位还是32位 */
#define CATSH_USE_SECTION           (1)   /**< 使用段*/
#define CATSH_MAX_CMD_NUM           (20)  /**< 最大命令数（命令结构体数）*/

#define CATSH_MAX_CMD_LEN           (20)  /**< 单条命令最大长度*/

#define CATSH_MAX_CMD_ARGS          (5)   /**< 最大参数数量*/
#define CATSH_MAX_ARG_LEN           (20)  /**< 单条参数最大长度*/

#define CATSH_MAX_HISTORY           (2)  /**< 最大历史记录数*/


/* 1-extend start 根据设定自动计算的配置项 */
                                        /**< 每次可输入的最大字符数*/
#define CATSH_MAX_SHELL_INPUT       (CATSH_MAX_CMD_LEN + CATSH_MAX_CMD_ARGS*(CATSH_MAX_ARG_LEN + 1) + 1)


#define CATSH_MAX_HIS_LEN           (CATSH_MAX_SHELL_INPUT) /**< 单条历史记录最大长度*/
#define CATSH_MAX_HIS_MEM           (CATSH_MAX_HISTORY * CATSH_MAX_HIS_LEN)

#define CATSH_BUF_SIZE              (CATSH_MAX_SHELL_INPUT)

#define CATSH_SHELL_MEM_SIZE        ( \
    CATSH_BUF_SIZE + \
    CATSH_MAX_HIS_MEM \
)

#if (CATSH_USE_SECTION == 1)
    #if (CATSH_CPU_64BIT == 1)
        #define CATSH_KEY_CMD_ALIGN             (aligned(16))
    #else
        #define CATSH_KEY_CMD_ALIGN             (aligned(4))
    #endif
#endif
/* 1-extend end */
/* 1-CONFIG end */
/* 2-TYPEDEF start */
/* 按键类型描述结构体 */
typedef struct _catsh_key_t
{
    cat_u8 *name;
    catsh_u32 key_value;
    void (*action)(void *p_instance);
} __attribute__(CATSH_KEY_CMD_ALIGN) catsh_key_t;

/* 命令类型描述结构体 */
typedef struct _catsh_cmd_t
{
    const catsh_u8 *name;
    const catsh_u8 *desc;
    void *(*do_cmd)(void *cmd_arg);
} __attribute__(CATSH_KEY_CMD_ALIGN) catsh_cmd_t;

/* shell实体（实例）描述结构体 */
typedef struct _catsh_instance_t
{
    struct
    {
        catsh_u8   is_inited;                       /**< 是否初始化 */
    } status; /**< shell实例状态 1  1*/
    struct
    {
        catsh_u8  *buffer_space;                    /**< 缓冲空间内存地址 */
        catsh_u8  *parsed_args[CATSH_MAX_CMD_ARGS]; /**< 根据输入解析的参数(4 */
        catsh_u16  cur_input_len;                   /**< 当前输入数据的长度(2*/
        catsh_u16  arg_num;                         /**< 参数的个数(2*/
        catsh_u16  buffer_space_size;               /**< 缓冲区总大小(2*/
        catsh_u32  cur_comb_key_val;                /**< 当前累计的键值*/
        // catsh_u8   cur_comb_key_off;                /**< 当前组合键偏移*/
    } buffer_desc; /**< 缓冲区状态描述  64 */
    struct 
    {
        catsh_u8  *historys[CATSH_MAX_HISTORY];     /**< 历史记录列表 */
        catsh_u16  his_num;                         /**< 历史记录数量 */
        catsh_u16  current;                         /**< 当前的记录索引值 */
    } history_desc; /**< 命令历史描述   24 */
    struct
    {
        catsh_key_t *key_base;                      /**< 按键列表起始地址 */
        catsh_u16    key_num;                       /**< 按键种类数量 */
    } key_list_desc; /**< 按键列表描述  16 */
    struct
    {
        catsh_cmd_t *cmd_base;                      /**< 命令结构体列表起始地址 */
        catsh_u16    cmd_num;                       /**< 命令数量 */
    } cmd_list_desc; /**< 命令列表描述   16 */
} catsh_instance_t;
/* 2-TYPEDEF end */
/* 3-MACRO start */
/* 3-1-key_related start*/
#define CATSH_KEY_COMB_1_MASK     (0xff000000)
#define CATSH_KEY_COMB_2_MASK     (0xffff0000)
#define CATSH_KEY_COMB_3_MASK     (0xffffff00)

#define CATSH_KEY_HAS_0(comb_val) ((comb_val & CATSH_KEY_COMB_1_MASK) == 0)
#define CATSH_KEY_HAS_1(comb_val) ((comb_val & CATSH_KEY_COMB_1_MASK) == comb_val)
#define CATSH_KEY_HAS_2(comb_val) ((comb_val & CATSH_KEY_COMB_2_MASK) == comb_val)
#define CATSH_KEY_HAS_3(comb_val) ((comb_val & CATSH_KEY_COMB_3_MASK) == comb_val)

#define CATSH_KEY_VAL_BACKSPACE (0x08000000) /**< 退格键键值 */
#define CATSH_KEY_VAL_HT        (0x09000000) /**< 制表键键值 */

#define CATSH_KEY_VAL_LF                (0x0a)  /**< 换行*/
#define CATSH_KEY_VAL_CR                (0x0d)  /**< 归位*/
#define CATSH_KEY_VAL_LFCR              (0x0a0d)
#define CATSH_KEY_VAL_ENTER             (0x0d000000)

/*
 * note: 方向键键值是三个键值有序组合而成
 */

#define CATSH_KEY_VAL_UP        (0x1b5b4100)
#define CATSH_KEY_VAL_DOWN      (0x1b5b4200)
#define CATSH_KEY_VAL_LEFT      (0x1b5b4400)
#define CATSH_KEY_VAL_RIGHT     (0x1b5b4300) 
/* 3-1-key_related end*/
/* 3-2-greeting start */
#define GREET_MSG_SMALL \
"_____________________________________\r\n \
    ___      _   ___ _        _ _   \r\n \
   / __|__ _| |_/ __| |_  ___| | |  \r\n \
  | (__/ _` |  _\\__ \\ ' \\/ -_) | |\r\n \
   \\___\\__,_|\\__|___/_||_\\___|_|_| \r\n \
_____________________________________\r\n\r\n \
CatShell v2.0.0 created by mio \r\n \
type \"help\" to get more info\r\n\r\n"
/* 3-2-greeting end */
/* 3-3-ANSI control symbol start */
#define CAT_SHELL_CLR_LINE          "\r\033[2k"
#define CAT_SHELL_CLR_SCREEN        "\033[2J\033[0;0H"
/* 3-3-ANSI control symbol end */
/* 3-4-section related start */
#if (CATSH_USE_SECTION == 1)
#if defined(__CC_ARM)
    #define IMPORT_SECTION(x) \
        extern catsh_ubase x##$$Base; \
        extern catsh_ubase x##$$Limit;

    #define SECTION_START(x)  (&x##$$Base)
    #define SECTION_END(x)    (&x##$$Limit)
    #define SECTION_SIZE(x)   ((catsh_ubase)&x##$$Limit - (catsh_ubase)&x##$$Base)

    #define SECTION(x)  __attribute__((used, section(x)))
#elif defined(__GNUC__)
    /* 将在链接脚本中指定的段引入到源文件中，表现为两个变量：段起始地址和段结束地址 */
    #define IMPORT_SECTION(x) \
        extern catsh_ubase x##_start; \
        extern catsh_ubase x##_end;

    /* 获取段信息 */
    #define SECTION_START(x)  (&x##_start)
    #define SECTION_END(x)    (&x##_end)
    #define SECTION_SIZE(x)   ((catsh_ubase)&x##_end - (catsh_ubase)&x##_start)

    /* 声明将被修饰的数据放入指定段中 */
    #define SECTION(x)  __attribute__((section(x)))
#else
    #error "no match compiler"
#endif
#endif /* (CATSH_USE_SECTION == 1) */
/* 3-4-section related end */
/* 3-5-cmd&key ops start */
#define CATSH_DECLARE_CMD(_name, _desc, _do_cmd) \
    const catsh_cmd_t \
    catsh_cmd_##_name SECTION("catsh_cmd") = \
    { \
        .name = #_name, \
        .desc = #_desc, \
        .do_cmd = _do_cmd \
    }

#define CATSH_DECLARE_KEY(_name, _key_value, _action) \
    const catsh_key_t \
    catsh_key_##_name SECTION("catsh_key") = \
    { \
        .name = #_name, \
        .key_value = _key_value, \
        .action = _action \
    }

#define CATSH_FOREACH_CMD(p_tmp_cmd) \
    for( \
        p_tmp_cmd = (catsh_cmd_t *)SECTION_START(catsh_cmd); \
        p_tmp_cmd != (catsh_cmd_t *)SECTION_END(catsh_cmd); \
        p_tmp_cmd++ \
    )

#define CATSH_FOREACH_KEY(p_tmp_key) \
    for( \
        p_tmp_key = (catsh_key_t *)SECTION_START(catsh_key); \
        p_tmp_key != (catsh_key_t *)SECTION_END(catsh_key); \
        p_tmp_key++ \
    )
/* 3-5-cmd&key ops end */
/* 3-6-error proccess start */
#define CATSH_ASSERT(expr) \
do{ \
    if(!(expr)) \
    { \
        CATSH_PRINTF("line%u assert %s fail!\r\n", __LINE__, #expr); \
        while(1); \
    } \
}while(0)
/* 3-6-error proccess end */
/* 3-MACRO end*/
/* 4-function declare start */
catsh_err catsh_init(catsh_instance_t *inst, void *buffer_space, catsh_ubase buffer_total_size);
void catsh_greeting(catsh_instance_t *inst);
void catsh_handle_input_char(catsh_instance_t *inst, catsh_u8 c);
/* 4-function declare end */
#endif
