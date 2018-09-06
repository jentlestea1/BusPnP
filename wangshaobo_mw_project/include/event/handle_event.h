/** 
* @file        handle_event.h
* @brief       申明操作事件库的几个函数
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/
#ifndef HANDLE_EVENT_H_
#define HANDLE_EVENT_H_
#include "m_type.h"
#include "event.h"
#include "event_type.h"
/** 事件库最大数量 */
#define REPOS_MAX_LEN 2

/** 
 * 初始化事件库仓库
 */ 
void init_event_repos_array(void);
/** 
 * 插入事件到事件库仓库
 * @param[out]   pos    输出事件库编号
 */ 
void get_one_event_repos(uint_32* pos);
/** 
 * 处理事件
 * @param[in]   p_event   事件指针
 */ 
void handle_event(void* p_event);
/** 
 * 处理事件
 * @param[in]   pos   事件库编号
 */ 
void* get_evt_repos(uint_32 pos);
/** 
 * 事件库扫描线程函数
 * @param[in]   argc   线程参数地址
 */ 
void* evt_repos_scan_pthread_func(void* argc);
/** 
 * 创建事件库扫描单元
 */
void create_evt_repos_scan_unit(void);
/** 
 * 抛出事件到事件库仓库
 * @param[in]   repos_id   事件库编号，一般只用0号事件库
 * @param[in]   RT_lid     RT逻辑标识符，用以标识哪个RT线路的异常   
 * @param[in]   event_type   事件类型
 */
void throw_event(uint_32 repos_id,byte_8* RT_lid,EVT_TYPE event_type);
#endif

