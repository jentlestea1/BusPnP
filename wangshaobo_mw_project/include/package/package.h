/** 
* @file        package.h
* @brief    声明数据包转发规则表结构和各种对表操作的函数
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/

#ifndef PACKAGE_H_
#define PACKAGE_H_
#include "m_type.h"

/** 打包数据包的最大数据容量 */
#define READ_BUFFER_MAX_SIZE 0x200

/** 设备对应数据段无效标志  */
#define DATA_BLOCK_INVALID_PREFIX (ubyte_8)0x00

/** 设备对应数据段有效标志  */
#define DATA_BLOCK_VALID_PREFIX (ubyte_8)0x80
/** 设备有效位掩码 */
#define DATA_BLOCK_ISVALID_MASK (ubyte_8)0x80
/** 数据包头部大小位为4个字节 */
#define PACKAGE_HEADER_SIZE_LEN 0x4

/** 
 * 解包,控制解包过程详见@ref control_package.h
 * @param[in]    traffic_repos_id    交通灯库id
 * @param[in]    buffer  读入数据包的缓存地址
 * @param[in]    buf_size    buffer的最大容量
 * @param[in]    bus_type    总线类型
 * @param[in]    bus_lid     总线逻辑标识符
 * @param[in]    RT_lid      RT逻辑标识符
 */ 
void unpack_package_to_1553(uint_32 traffic_repos_id,ubyte_8* buffer,uint_32 buf_size,byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid);

/** 
 * 打包,控制打包过程详见@ref control_package.h
 * @param[in]    traffic_repos_id    交通灯库id
 * @param[in]    light_pos    交通灯在交通灯库中的id
 * @param[in]    bus_type    总线类型
 * @param[in]    bus_lid     总线逻辑标识符
 * @param[in]    RT_lid      RT逻辑标识符
 * @param[out]    buffer_1553  写出数据包的缓存地址
 * @param[out]    buffer_size    写出的大小
 */ 
void pack_package_to_1553(uint_32 traffic_repos_id,uint_32 light_pos,byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,ubyte_8* buffer_1553,uint_32* buffer_size);
#endif
