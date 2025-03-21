/**
 * @file port.h
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-22
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-22 1.0    amoigus             内容
 */

#ifndef CAT_PORT_H
#define CAT_PORT_H

#include "catos_config.h"
#include "catos_types.h"

/**
 * @brief 关中断进临界区
 * 
 * @return uint32_t 
 */
uint32_t cat_enter_critical(void);

/**
 * @brief 开中断出临界区
 * 
 * @param status 
 */
void cat_exit_critical(uint32_t status);

#endif
