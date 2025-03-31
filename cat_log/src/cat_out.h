/**
 * @file cat_out.h
 * @author 文佳源 (648137125@qq.com)
 * @brief 输出字符串
 * @version 0.1
 * @date 2025-03-31
 * 
 * Copyright (c) 2025
 * 
 * @par 修订历史
 * <table>
 * <tr><th>版本 <th>作者 <th>日期 <th>修改内容
 * <tr><td>v1.0 <td>文佳源 <td>2025-03-31 <td>内容
 * </table>
 */
#include <stdio.h>

#define CAT_PRINTF(fmt, ...) printf(fmt, ##__VA_ARGS__)