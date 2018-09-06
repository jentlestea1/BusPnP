/** 
* @file        file.h
* @brief    测试数据转发的正确性
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/
#ifndef FILE_H_
#define FILE_H_
#include "m_type.h"
#define SEND 1
#define RECEIVE 2
#define STR_MAX_SIZE 0x10000 
/** 处理数据时的结构 */
typedef struct t{
    uint_32 send_or_rec;
    float_64 data;
    uint_32 port;
}t;
/** 结束标识 */
static uint_32 is_end=0;
/** 输出到文件的字符串 */
static ubyte_8 out_put_str[STR_MAX_SIZE]={0};
/** 
 * 向out_put_str中添加字符串
 * @param[in]   send_or_recv    收或发标识
 * @param[in]   data    数据
 * @param[in]   port    端口号
 */
void add_string(uint_32 send_or_recv,float_64 data,uint_32 port);
/** 
 * 将out_put_str写入到文件data.txt
 */
void out_put(void);
/** 
 * is_end++
 */
void set_is_end(void);
/** 
 * is_end?=6
 * @return    6,end
 */
uint_32 is_end_t(void);
/** 
 * 读入data.txt数据并判定结果正确性
 */
void judge_result(void);
#endif
