/** 
* @file        BC_control.h
* @brief    申明控制BC转发数据的几个函数
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/
#ifndef BC_CONTROL_H
#define BC_CONTROL_H
#include "m_type.h"

/** 
 * 获取RT子地址列表，按优先级递减
 * @param[in]   port    RT对应的配置地址（端口号）
 * @param[in]   buf    用于获取对应配置地址RT的子地址(子端口号)列表的缓存指针,一个子地址使用4个字节存储
 * @return		子地址的数量 
 */ 
uint_32 get_RT_sub_addr_array(uint_32 port,uint_32 *buf);

/** 
 * 获取对应buffer是否已准备好数据
 * @param[in]   config_id    1553总线（BC）注册的id
 * @param[in]    RT_config_id   1553总线下的RT注册的id
 * @return        true，准备好，false，没准备好
 */ 
bool get_buffer_is_avail(uint_32 config_id,uint_32 RT_config_id);

/** 
 * 设置对应buffer标志位为不可用
 * @param[in]   config_id    1553总线（BC）注册的id
 * @param[in]    RT_config_id   1553总线下的RT注册的id
 * @return        true，设置成功，false，设置不成功
 */
bool set_buffer_unavail(uint_32 config_id,uint_32 RT_config_id);

/** 
 * 设置对应buffer标志位为可用
 * @param[in]   config_id    1553总线（BC）注册的id
 * @param[in]    RT_config_id   1553总线下的RT注册的id
 * @return        true，设置成功，false，设置不成功
 */
bool set_buffer_avail(uint_32 config_id,uint_32 RT_config_id);

/** 
 * 获取对应buffer内的内容
 * @param[in]   config_id    1553总线（BC）注册的id
 * @param[out]    buf   将数据写出的缓冲区
 */ 
void get_buffer_content(byte_8* buf,uint_32 config_id);

/** 
 * 获取1553总线下RT的地址(端口号)列表
 * @param[out]    buf   写出RT地址的缓冲区地址,一个地址用4个字节存储
 */ 
uint_32 get_RT_addr_array(uint_32 * buf);
#endif

