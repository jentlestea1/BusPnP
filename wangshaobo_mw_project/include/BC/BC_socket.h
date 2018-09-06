/** 
* @file        BC_socket.h
* @brief    申明BC转发数据的几个函数,支持VCAN和TCPIP两种数据传输协议
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/
#ifndef MY_SOCKET_H
#define MY_SOCKET_H
#include "m_type.h"
#include "config_1553.h"
/** socket一次最大发送/接收数据量(Byte) */
#define MAXLINE 4096

/** 创建扫描线程时传入的参数类型 */
typedef struct scan_config{
    uint_32 config_id;
}scan_config;

/** 创建转发线程时传入的参数类型 */
typedef struct socket_config{
    uint_32 config_id;
    uint_32 RT_config_id;
    uint_32 port;
    uint_32 sub_port;
}socket_config;

/** 
 * 创建总线客户端服务，用来给RT服务端(socket server)发送数据
 * @param[in]   config_id    1553总线（BC）注册的id
 * @param[in]   RT_config_id   1553总线下的RT注册的id
 * @param[in]   port  RT地址
 */ 
void create_bus_socket_client(uint_32 config_id,uint_32 RT_config_id,uint_32 port);

/** 
 * 创建总线服务端服务，用来接收RT客户端(socket client)发送的数据
 * @param[in]   config_id    1553总线（BC）注册的id
 * @param[in]   port  RT地址
 */ 
void create_bus_socket_server(uint_32 config_id,uint_32 port);
/** 
 * 扫描RT section的线程函数，获取要发送给RT的数据,相关结构详见
 * @ref traffic_light.h
 * @param[in]   p_scan_config   传入线程的参数地址
 */ 
void* scan_1553_RT_section_pthread_func(void* p_scan_config);
/** 
 * 创建扫描线程单元，每一条1553总线都有一个扫描线程单元
 * @param[in]   p_scan_config    传入线程的参数地址
 */ 
void create_scan_1553_RT_section_unit(void* p_scan_config);
/** 
 * 创建总线服务端线程函数
 * @param[in]   p_socket_config    传入线程的参数地址
 */ 
void* bus_socket_pthread_func(void* p_socket_config);
/** 
 * 创建总线客户端线程函数
 * @param[in]   p_socket_config    传入线程的参数地址
 */ 
void* bus_ret_socket_pthread_func(void* p_socket_config);

/** 
 * 创建总线转发数据单元，包含一个总线服务端和一个总线客户端服务线程
 * @param[in]   p_scan_config    传入线程的参数地址
 */  
void create_1553_bus_unit(void* p_socket_config);
/** 
 * 创建总线转发数据单元之前的初始化操作，与RT获取连接信息
 */ 
void initialize_BC();
#endif
