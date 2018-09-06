/** 
* @file        data_stream.h
* @brief       读写数据流
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/
#ifndef DATA_STREAM_H_
#define DATA_STREAM_H_
#include "m_type.h"
#include "address_map.h"
/** 写区域最大容量 */
#define WRITE_MAX_SIZE WRITE_REGION_MAX_SIZE
/** 读区域最大容量 */
#define READ_MAX_SIZE READ_REGION_MAX_SIZE

/** 
 * 配置索引表，详见
 * @ref   address_map.h
 */ 
void config_address_map_index_list(void);
/** 
 * 应用层方向写入设备数据接口，写过程详见
 * @ref         address_map.h
 * @param[in]   dev_lid     设备的逻辑标识符
 * @param[in]   buffer      写入数据缓冲区的地址
 * @param[in]   write_size    要写入的数据大小  
 * @param[out]   size    写入的数据大小 
 */ 
void app_write_data_func(byte_8* dev_lid,ubyte_8* buffer,uint_32 write_size,uint_32* size);
/** 
 * 应用层方向读出设备数据接口，写过程详见
 * @ref         address_map.h
 * @param[in]   dev_lid     设备的逻辑标识符
 * @param[out]   buffer      读出数据缓冲区的地址
 * @param[in]   read_size    要读出的数据大小  
 * @param[out]   size    写入的数据大小 
 * @param[out]   time    读出数据带有的时间标签 
 */ 
void app_read_data_func(byte_8* dev_lid,ubyte_8* buffer,uint_32 read_size,uint_32* size,void* time);
/** 
 * 设备方向写入设备数据接口，写过程详见
 * @ref         address_map.h
 * @param[in]   dev_lid     设备的逻辑标识符
 * @param[in]   buffer      写入数据缓冲区的地址
 * @param[in]   write_size    要写入的数据大小  
 * @param[out]   size    写入的数据大小 
 */ 
void dev_write_data_func(byte_8* dev_lid,ubyte_8* buffer,uint_32 write_size,uint_32* size);
/** 
 * 设备方向读出设备数据接口，写过程详见
 * @ref         address_map.h
 * @param[in]   dev_lid     设备的逻辑标识符
 * @param[out]   buffer      读出数据缓冲区的地址
 * @param[in]   read_size    要读出的数据大小  
 * @param[out]   size    写入的数据大小 
 * @param[out]   time    读出数据带有的时间标签 
 */ 
void dev_read_data_func(byte_8* dev_lid,ubyte_8* buffer,uint_32 read_size,uint_32* size,void* time);
#endif

