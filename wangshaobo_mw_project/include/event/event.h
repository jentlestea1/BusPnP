/** 
* @file        event.h
* @brief    声明事件库结构和基础的操作函数
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/
#ifndef EVENT_H_
#define EVENT_H_
#include "compile_type.h"
#include "m_type.h"
#include "relevant_struct_def.h"
#ifndef __RT_INCLUDE
#ifdef __SPARC_GCC_MMU
#include <fsu_pthread.h>
#elif __GCC_C99
#include <pthread.h>
#endif
#else
#include "pthread.h"
#endif
#define EVT_TYPE uint_32
#define EVT_LIST_MAX_LEN 10
#define REPOS_TYPE_1553_RET 1
#define REPOS_TYPE_1553 2
#define REPPOS_TYPE_RT 3
#define REPOS_TYPE uint_32
#define ENABLE 1
#define DISABLE 0

/** 事件结构 */
typedef struct event{
    timeStamp time_stamp;
    EVT_TYPE event_type;
    byte_8* sel_RT_lid;
}evt;
/** 事件库结构 */
typedef struct event_repository{
    evt* p_evt_list;    //<指向事件列表的指针
    uint_32 list_size;
    uint_32 counter;
    uint_32 global_enable;
    uint_32 start_pos;
    uint_32 end_pos;
	pthread_mutex_t mutex;  //<插入事件时的互斥锁
	
}evt_repository;

/** 
 * 创建事件仓库,系统默认创建一个事件库
 */ 
void* create_evt_repos(void); //注册一个event_repository,系统默认一个事件库即可

/** 
 * 重置事件库仓库 
 * @return      true，删除成功，false，删除失败
 */
void reset_evt_repos(void* p_evt_repos);
/** 
 * 插入事件到事件库仓库
 * @param[in]   p_evt_repos     指向事件库仓库指针
 * @param[in]   RT_lid    RT_lid标签 
 * @param[in]   event_type      事件库类型
 * @return      true，插入成功，false，插入失败
 */ 
bool insert_one_event(void* p_evt_repos,byte_8* RT_lid,EVT_TYPE event_type);
/** 
 * 获取一个事件节点
 * @param[in]   p_evt_repos     指向事件库仓库指针
 * @param[out]   p_evt_node      写出事件节点到本地的事件节点指针 
 * @return      true，获取成功，false，获取失败
 */ 
bool get_one_event(void* p_evt_repos,void* p_evt_node);
/** 
 * 获取事件节点的时间标签
 * @param[in]   p_evt_node      本地事件节点指针 
 * @return      指向时间标签的指针
 */
void* get_evt_node_time(void* p_evt_node);
/** 
 * 获取事件节点的事件类型
 * @param[in]   p_evt_node      本地事件节点指针 
 * @return      事件类型
 */
uint_32 get_evt_node_event_type(void* p_evt_node);
/** 
 * 获取事件节点的RT逻辑标识符
 * @param[in]   p_evt_node      本地事件节点指针 
 * @return      RT的逻辑标识符
 */
byte_8* get_evt_node_sel_RT_lid(void* p_evt_node);
/** 
 * 设置事件库有效
 * @param[in]   p_evt_repos      事件仓库指针 
 */
void set_evt_repos_global_enable(void* p_evt_repos);
#endif
