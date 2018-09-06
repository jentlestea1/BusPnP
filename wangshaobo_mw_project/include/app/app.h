/** 
* @file        app.h
* @brief    声明应用层的几个函数
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/
#ifndef APP_H
#define APP_H

/** 
 * 第一个应用层操作函数,接收8001,8002端口数据，计算结果发送给8003
 * @param[in]   argc    创建线程时传入的参数 
 */
void* app_read_pthread_func_f(void* argc);

/** 
 * 第二个应用层操作函数,接收8004端口数据，计算结果发送给8004
 * @param[in]   argc    创建线程时传入的参数 
 */
void* app_read_pthread_func_s(void* argc);

/** 
 * 第三个应用层操作函数,接收8005,8006,8007端口数据，计算结果发送给8007,8008
 * @param[in]   argc    创建线程时传入的参数 
 */
void* app_read_pthread_func_t(void* argc);

/** 
 * 应用层写操作函数
 * @param[in]   argc    创建线程时传入的参数 
 */
void* app_write_pthread_func(void* argc);

/** 
 * 用来创建前三个应用层操作线程的函数
 */
void app_read(void);
#endif
