/** 
* @file        control_package.h
* @brief    声明数据包转发规则表结构和各种对表操作的函数
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/

#ifndef CONTROL_PACKAGE_H_
#define CONTROL_PACKAGE_H_
#include "m_type.h"

/** 
 * 控制解包,解包过程详见@ref package.h
 * @param[in]    traffic_repos_id    交通灯库id
 * @param[in]    port    RT地址(端口)
 * @param[in]    buffer  读入数据包的缓存地址
 * @param[in]    buf_size    buffer的最大容量
 */ 
void ctrl_unpack_package_to_1553(uint_32 traffic_repos_id,uint_32 port,ubyte_8* buffer,uint_32 buf_size);

/** 
 * 控制打包,打包过程详见@ref package.h
 * @param[in]    traffic_repos_id    交通灯库id
 * @param[out]    buffer_1553  写出数据包的缓存地址
 * @param[out]    buf_size    写出的大小
 * @param[in]    light_pos    交通灯在交通灯库中的id
 */ 
void ctrl_pack_package_to_1553(uint_32 traffic_repos_id,ubyte_8* buffer_1553,uint_32* buf_size,int* light_pos); //最后一个参数返回-1为无效
#endif
