/** 
* @file        handle_event_func.h
* @brief       申明处理具体类型事件的几个函数
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/
#ifndef HANDLE_EVENT_FUNC_H_
#define HANDLE_EVENT_FUNC_H_
#include "m_type.h"
/** 
 * 处理1553总线发送给RT时出现的异常
 * @param[in]   p_event    事件指针
 */ 
void handle_1553_ret_func(void* p_event);
/** 
 * 处理RT发送给1553总线时出现的异常
 * @param[in]   p_event    事件指针
 */
void handle_1553_func(void* p_event);
/** 
 * 处理RT本身出现的异常
 * @param[in]   p_event    事件指针
 */
void handle_RT_func(void* p_event);
#endif
