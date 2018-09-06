/** 
* @file        dev.h
* @brief    模拟设备产生数据
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/
#ifndef DEV_H
#define DEV_H
/** 
 * 创建6种产生数据的线程
 */ 
void generate_data(void);
/** 
 * 产生数据给端口8001，时间1~10秒内随机
 * @param[in]    argc    线程的参数地址
 */ 
void* generate_data_1(void* argc);
/** 
 * 产生数据给端口8002，时间1~10秒内随机
 * @param[in]    argc    线程的参数地址
 */ 
void* generate_data_2(void* argc);
/** 
 * 产生数据给端口8004，时间1~10秒内随机
 * @param[in]    argc    线程的参数地址
 */ 
void* generate_data_4(void* argc);
/** 
 * 产生数据给端口8005，时间1~10秒内随机
 * @param[in]    argc    线程的参数地址
 */ 
void* generate_data_5(void* argc);
/** 
 * 产生数据给端口8006，时间1~10秒内随机
 * @param[in]    argc    线程的参数地址
 */ 
void* generate_data_6(void* argc);
/** 
 * 产生数据给端口8007，时间1~10秒内随机
 * @param[in]    argc    线程的参数地址
 */ 
void* generate_data_7(void* argc);
#endif
