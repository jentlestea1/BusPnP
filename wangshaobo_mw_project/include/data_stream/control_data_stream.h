/** 
* @file        control_data_stream.h
* @brief       读写数据流中加入路径映射和信号灯控制
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/
#ifndef CONTROL_DATA_STREAM_H_
#define CONTROL_DATA_STREAM_H_
#include "m_type.h"
/** 获取设备对应的读取数据块大小类型 */
#define AUTO_SET uint_32
/** 自动获取设备对应读取数据块大小 */
#define AUTO 1
/** 手动设置设备对应的读取数据块大小 */
#define MANUAL 2

/** 
 * 应用层方向写入设备数据接口，写过程详见
 * @ref         data_stream.h
 * @param[in]   traffic_repos_id    交通灯库id
 * @param[in]   dev_lid     设备的逻辑标识符
 * @param[out]   buffer      写入数据缓冲区的地址
 * @param[in]   write_size    要写入的数据大小  
 * @param[out]   size    写入的数据大小 
 */ 
void ctrl_app_write_data(uint_32 traffic_repos_id,byte_8* dev_lid,ubyte_8* buffer,uint_32 write_size,uint_32* size);
/** 
 * 应用层方向读取设备数据接口，读取过程详见
 * @ref         data_stream.h
 * @param[in]   traffic_repos_id    交通灯库id
 * @param[in]   dev_lid     设备的逻辑标识符
 * @param[out]   buffer      写出数据缓冲区的地址
 * @param[in]   read_size    要读出的数据大小  
 * @param[out]   size    读出的数据大小 
 * @param[out]   time    读出数据带有的时间标签 
 */ 
void ctrl_app_read_data(uint_32 traffic_repos_id,byte_8* dev_lid,ubyte_8* buffer,uint_32 read_size,uint_32* size,void* time);
/** 
 * 设备方向写入设备数据接口，写过程详见
 * @ref         data_stream.h
 * @param[in]   traffic_repos_id    交通灯库id
 * @param[in]   dev_lid     设备的逻辑标识符
 * @param[out]   buffer      写入数据缓冲区的地址
 * @param[in]   write_size    要写入的数据大小  
 * @param[out]   size    写入的数据大小 
 */ 
void ctrl_dev_write_data(uint_32 traffic_repos_id,byte_8* dev_lid,ubyte_8* buffer,uint_32 write_size,uint_32* size);
/** 
 * 设备方向读取设备数据接口，写过程详见
 * @ref         data_stream.h
 * @param[in]   traffic_repos_id    交通灯库id
 * @param[in]   dev_lid     设备的逻辑标识符
 * @param[in]   buffer      写出数据缓冲区的地址
 * @param[in]   read_size    要读出的数据大小  
 * @param[out]   size    读出的数据大小 
 * @param[out]   time    读出数据带有的时间标签 
 */ 
void ctrl_dev_read_data(uint_32 traffic_repos_id,byte_8* dev_lid,ubyte_8* buffer,uint_32 read_size,uint_32* size,void* time);
/** 
 * 应用层方向读取设备数据接口，读取过程详见
 * @ref         data_stream.h
 * @param[in]   dev_lid     设备的逻辑标识符
 * @param[out]   buffer      写出数据缓冲区的地址
 * @param[in]   buf_size    缓冲区的最大容量
 * @param[out]   size    读出的数据大小
 * @param[out]   time    读出数据带有的时间标签 
 * @param[in]   auto_set    设置是否自动获取读出大小
 * @param[in]  set_size   手动时输入要读出的大小
 */ 
void read_data(byte_8* dev_lid,ubyte_8* buffer,uint_32 buf_size,uint_32* size,void* time,AUTO_SET auto_set,uint_32 set_size);
/** 
 * 应用层方向写设备数据接口，写过程详见
 * @ref         data_stream.h
 * @param[in]   dev_lid     设备的逻辑标识符
 * @param[in]   buffer      写入数据缓冲区的地址
 * @param[in]   buffer_size    buffer的最大容量
 * @param[out]   size    写入的数据大小
 */ 
void write_data(byte_8* dev_lid,ubyte_8* buffer,uint_32 buffer_size,uint_32* size);
#endif
