/** 
* @file        data_stream.h
* @brief       读写数据流
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/


#ifndef RELEVANT_STRUCT_DEF_H_
#define RELEVANT_STRUCT_DEF_H_
#include "m_type.h"
/** 一次读写的最大数据长度 */
#define CACHE_MAX_LEN 0x400

/** 时间戳 */
typedef struct timeStamp{
    ubyte_8 hour;
    ubyte_8 minute;
    ubyte_8 second;
}timeStamp;

/** 数据节点,在缓存中存储的基本单位 */
typedef struct dataNode{
    timeStamp time;    //存的是指针
    ubyte_8 dataPiece;
}dataNode;

void plugin_data_node_array(void* data_node_array,ubyte_8* buf,uint_32 buf_size,uint_32* size);
void* get_data_node_array(void);
void free_data_node_array(void* p_data_node_array);
void get_array_data_string(void* data_node_array,ubyte_8* buf,uint_32 read_size,uint_32* size,void* time);
void insert_node_to_data_node_array(void* dest_array,uint_32 pos,void* src_data_node);
void get_current_time(timeStamp* time_stamp);
void clear_data_node_array(void* p_data_node_array,uint_32 array_size);
void delay(uint_32 m_s);
void print_time(timeStamp* time_stamp);
#endif
