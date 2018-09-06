
/**
* @file  m_type.h  
* @brief 常见类型定义以及常用宏定义
* @author       Shaobo Wang
* @date     2018-5-21
* @version  A001 
* @copyright Shaobo Wang                                                              
*/

#ifndef M_TYPE_H_
#define M_TYPE_H_
#include "compile_type.h"
#define bool int
#define true 1
#define false 0
#ifndef NULL
#define NULL 0
#endif

#define int_16 short
#define int_32 int
#define int_64 long
#define byte_8 char

#define uint_16 unsigned short
#define uint_32 unsigned int
#define uint_64 unsigned long
#define ubyte_8 unsigned char

#define float_32 float
#define float_64 double

#define ATTR_MAX_LEN 20
#define ATTR_VALUE_MAX_LEN 15
#define ATTR_LID_VALUE_MAX_LEN 15
#define ATTR_TYPE_VALUE_MAX_LEN 15
#define ATTR_INTERFACE_VALUE_MAX_LEN 15
#define RT_SUBADDR_VALUE_MAX_LEN 20
#define DEVICE_LIST_MAX_LEN 200
#define DEV_LID_MAX_LEN 32
#define SEND_PRIORITY_VALUE_MAX_LEN 10
#define SEND_BLOCK_VALUE_MAX_LEN 10
#define RECEIVE_PRIORITY_VALUE_MAX_LEN 10
#define RECEIVE_BLOCK_VALUE_MAX_LEN 10
#define TRAN_DATA_ITEM_MAX_LEN 5
#define INTERFACE_MAX_LEN 20
#define SEND_PRIORITY_POS 0
#define RECEIVE_PRIORITY_POS 1
#define SEND_BLOCK_POS 2
#define RECEIVE_BLOCK_POS 3
#endif
