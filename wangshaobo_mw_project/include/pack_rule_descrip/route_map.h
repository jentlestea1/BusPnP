/** 
* @file        route_map.h
* @brief    声明数据包转发规则表结构和各种对表操作的函数
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/

#ifndef ROUTE_MAP_H_
#define ROUTE_MAP_H_
#include "m_type.h"
#include "handle_data_package.h"


/** 路径结构 */
typedef struct route{
    byte_8 *bus_type;
    byte_8 *bus_lid;
    byte_8 *RT_lid;
}route;

/** 
 * 获取路径中的总线类型
 * @param[in]    r    路径
 */ 
byte_8* get_route_bus_type(route r);
/** 
 * 获取路径中的总线逻辑标识符
 * @param[in]    r    路径
 */ 
byte_8* get_route_bus_lid(route r);
/** 
 * 获取路径中的RT逻辑标识符
 * @param[in]    r    路径
 */ 
byte_8* get_route_RT_lid(route r);
/** 
 * 获取路径
 * @param[in]    RT_lid    RT的逻辑标识符
 * @param[out]    r    路径指针
 */ 
void get_RT_route_map(byte_8* RT_lid,route *r);
/** 
 * 获取路径
 * @param[in]    dev_lid    设备的逻辑标识符
 * @param[out]    r    路径指针
 */ 
void get_dev_route_map(byte_8* dev_lid,route *r);
#endif
