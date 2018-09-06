#ifndef MY_RT_SOCKET_H_
#define MY_RT_SOCKET_H_
#include "compile_type.h"
#include "m_type.h"
#include "RT_control.h"
#define m_random() rand()%5
#define TIMES 60        //TIMES为数据源发送数据次数，不要超过200
typedef struct port_con{
    uint_32 port;
}port_con;
void* get_one_port_con();
void create_RT_unit(void* p_RT_config);
void* create_RT_socket_server(void* port_con);
void create_RT_socket_client(ubyte_8* send_buffer,uint_32 port);
void* create_RT_ret_socket_client(void* port_con);
void set_RT_port(void* p_port_con,uint_32 port);
void initialize_RT(void);
#endif
