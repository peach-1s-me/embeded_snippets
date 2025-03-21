/**
 * @file catsh.c
 * @brief 
 * @author mio (648137125@qq.com)
 * @version 1.0
 * @date 2024-06-05
 * Change Logs:
 * Date           Author        Notes
 * 2024-06-05     mio     first verion
 * 
 */
#include "catsh.h"

IMPORT_SECTION(catsh_cmd)
IMPORT_SECTION(catsh_key)

static catsh_cmd_t *_catsh_seek_cmd(catsh_instance_t *inst);
static void _catsh_parse_args(catsh_instance_t *inst);
static void _catsh_execute_cmd(catsh_instance_t *inst);


catsh_err catsh_init(catsh_instance_t *inst, void *buffer_space, catsh_ubase buffer_desc_total_size)
{
    catsh_err ret = CATSH_ERROR;
    cat_ubase i;

    if(
        (CATSH_NULL != buffer_space) ||
        (buffer_desc_total_size < CATSH_SHELL_MEM_SIZE)
    )
    {
        inst->buffer_desc.buffer_space       = buffer_space;
        for(i=0; i<CATSH_MAX_CMD_ARGS; i++)
        {
            inst->buffer_desc.parsed_args[i] = CATSH_NULL;
        }
        inst->buffer_desc.cur_input_len      = 0;
        inst->buffer_desc.buffer_space_size  = buffer_desc_total_size;
        inst->buffer_desc.cur_comb_key_val   = 0x00000000;

        for(i=0; i<CATSH_MAX_HISTORY; i++)
        {
            inst->history_desc.historys[i] = inst->buffer_desc.buffer_space + CATSH_BUF_SIZE + CATSH_MAX_HIS_LEN * i;
            inst->history_desc.historys[i][0] = '\0';
        }
        inst->history_desc.his_num = 0;
        inst->history_desc.current = 0;

        inst->cmd_list_desc.cmd_base = (catsh_cmd_t *)SECTION_START(catsh_cmd);
        inst->cmd_list_desc.cmd_num = SECTION_SIZE(catsh_cmd) / sizeof(catsh_cmd_t);

        inst->key_list_desc.key_base = (catsh_key_t *)SECTION_START(catsh_key);
        inst->key_list_desc.key_num = SECTION_SIZE(catsh_key) / sizeof(catsh_key_t);
    
        inst->status.is_inited = 1;

        ret = CATSH_EOK;
    }

    return ret;
}

void catsh_greeting(catsh_instance_t *inst)
{
    if(inst->status.is_inited == 1)
    {
        CATSH_PRINTF(GREET_MSG_SMALL);
    }
    else
    {
        CATSH_PRINTF("[catsh]ERROR: this instance has not been inited!\r\n");
    }
}

void catsh_handle_input_char(catsh_instance_t *inst, catsh_u8 c)
{
    CATSH_ASSERT(c);

    catsh_u8 expected_comb_offset = 0;        /**< 期待的新输入键值在组合键值中的偏移位置，注意是从左到右累计先后顺序 */
    catsh_u32 current_comb_mask = 0x00000000; /**< 当前已经累积的键值的掩码 */
    if(CATSH_KEY_HAS_1(inst->buffer_desc.cur_comb_key_val))
    {
        expected_comb_offset = 24;
        current_comb_mask = 0x00000000;
    }
    else if(CATSH_KEY_HAS_2(inst->buffer_desc.cur_comb_key_val))
    {
        expected_comb_offset = 16;
        current_comb_mask = CATSH_KEY_COMB_1_MASK;
    }
    else if(CATSH_KEY_HAS_3(inst->buffer_desc.cur_comb_key_val))
    {
        expected_comb_offset = 8;
        current_comb_mask = CATSH_KEY_COMB_2_MASK;
    }
    else
    {
        expected_comb_offset = 0;
        current_comb_mask = CATSH_KEY_COMB_3_MASK;
    }

    catsh_key_t *p_key = CATSH_NULL;
    CATSH_FOREACH_KEY(p_key)
    {
        /* 匹配前序键值 */
        if(
            ((p_key->key_value & current_comb_mask) == inst->buffer_desc.cur_comb_key_val) &&
            ((p_key->key_value & (0xff << expected_comb_offset)) == (c << expected_comb_offset))
        )
        {
            /* 至此说明已经有的累计键值加上当前键值，和列表中这个按键键值前序匹配 */
            inst->buffer_desc.cur_comb_key_val |= c << expected_comb_offset;

            /* 将data清空，防止被后面识别为普通输入 */
            c = 0x0;

            /* 如果完全匹配，即判断是否键值到此为止 */
            if(
                (expected_comb_offset == 0) || /* 说明当前读入的键值已经是第四个 */
                ((p_key->key_value & (0xff << (expected_comb_offset - 8))) == 0x00000000) /* 或者该命令项在当前偏移之后都为零，即没有下一个键值 */
            )
            {
                if(CATSH_NULL != p_key->action)
                {
                    p_key->action(inst);
                }
                inst->buffer_desc.cur_comb_key_val = 0x00000000;
            }/* 完全匹配 */
        }/* 前序匹配 */
    }

    /* 如果是特定按键，则此时data已经被置零，若没有则说明是普通输入 */
    if(0x00 != c)
    {
        inst->buffer_desc.cur_comb_key_val = 0x00000000;
        inst->buffer_desc.buffer_space[inst->buffer_desc.cur_input_len++] = c;
        CATSH_PUTC(c);
    }
}

static catsh_cmd_t *_catsh_seek_cmd(catsh_instance_t *inst)
{
    catsh_cmd_t *ret = CATSH_NULL;
    catsh_cmd_t *tmp_cmd = CATSH_NULL;
    CATSH_FOREACH_CMD(tmp_cmd)
    {
        if(0 == CATSH_STRCMP(tmp_cmd->name, inst->buffer_desc.buffer_space))
        {
            ret = tmp_cmd;
        }
    }

    return ret;
}

static void _catsh_parse_args(catsh_instance_t *inst)
{
    catsh_ubase i = 0;

    catsh_u8 *buffer_desc = inst->buffer_desc.buffer_space;

    /* 初始化，清空上一次的记录 */
    inst->buffer_desc.arg_num = 0;
    for(i=0; i<CATSH_MAX_CMD_NUM; i++)
    {
        inst->buffer_desc.parsed_args[i] = NULL;
    }

    /* 要先跳过命令的名称 */
    i = 0;
    while(' ' != buffer_desc[i])
    {
        i++;
    }

    /* 开始解析 */
    for(; i<inst->buffer_desc.cur_input_len; i++)
    {
        /* 空格分隔 */
        if(
            (' ' == buffer_desc[i]) &&
            (' ' != buffer_desc[i+1]) &&
            ('\0' != buffer_desc[i+1])
        )
        {
            /* 此时找到了下一个参数开始位置 */
            buffer_desc[i] = '\0';
            if(inst->buffer_desc.arg_num < CATSH_MAX_CMD_ARGS)
            {
                inst->buffer_desc.parsed_args[inst->buffer_desc.arg_num++] = &(buffer_desc[i+1]);
            }
            else
            {
                CATSH_PRINTF("[catsh]ERROR: over max arg num!\r\n");
            }
        }/* if */
    }/* for */
}

static void _catsh_execute_cmd(catsh_instance_t *inst)
{
    catsh_cmd_t *target_cmd = CATSH_NULL;

    _catsh_parse_args(inst);

    target_cmd = _catsh_seek_cmd(inst);

    if(CATSH_NULL != target_cmd)
    {
        if(CATSH_NULL != target_cmd->do_cmd)
        {
            target_cmd->do_cmd(inst);
        }
    }
    else
    {
        CATSH_PRINTF("[catsh]ERROR: cmd %s not found!\r\n", inst->buffer_desc.buffer_space);
    }
}

/* keys */
void backspace_action(catsh_instance_t *inst)
{
	if(inst->buffer_desc.cur_input_len > 0)
    {
        CATSH_PRINTF("\b \b");//
        inst->buffer_desc.buffer_space[inst->buffer_desc.cur_input_len--] = '\0';
    }
}
CATSH_DECLARE_KEY(bs, CATSH_KEY_VAL_BACKSPACE, backspace_action);

void enter_action(catsh_instance_t *inst)
{
    CATSH_PRINTF("\r\n");
    inst->buffer_desc.buffer_space[inst->buffer_desc.cur_input_len] = '\0';

    if(inst->buffer_desc.cur_input_len != 0)
    {
        _catsh_execute_cmd(inst);

        inst->buffer_desc.buffer_space[0] = '\0';
        inst->buffer_desc.cur_input_len = 0;
    }
    
    CATSH_PRINTF("cat>");
}
CATSH_DECLARE_KEY(enter, CATSH_KEY_VAL_ENTER, enter_action);

/* basic cmds */
void *do_help(void *arg)
{
    (void)arg;
	
	catsh_u8 i = 0;
    catsh_cmd_t *temp;

	CATSH_FOREACH_CMD(temp)
	{
        /* 只打印命令 */
        CATSH_PRINTF("%2d %s\r\n", i, temp->name);
		i++;
	    
    }
		
	return NULL;
}
CATSH_DECLARE_CMD(help, print help msg, do_help);
