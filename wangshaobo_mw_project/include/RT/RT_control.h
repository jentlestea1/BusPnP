/*
 * 定义几个接受字符串的缓存,假设RT下最多可以挂载31个设备
 */
#ifndef RT_CONTROL_H_
#define RT_CONTROL_H_
#include "compile_type.h"
#include "m_type.h"
//#define RT_physical_addr 9000//具体再改
#define MOUNT_DEV_MAX_NUM 31
#define CACHE_MAX_SIZE 0x400 //1k
#define READ_MAX_SIZE_RT 0xff
#define RT_DATA_BLOCK_VALID_PREFIX (ubyte_8)0x80
#ifdef __RT_TCPIP_TRANSMIT
#define RT_PACKAGE_HEADER_SIZE_LEN 0X4
#endif

void init_port_array(uint_32 *RT_sub_addr_array,uint_32 size);
void clear_write_buffer(void);
void write_write_buffer(uint_32 child_port,ubyte_8* buffer,uint_32 write_size,uint_32* size);
void read_write_buffer(uint_32 pos,ubyte_8* buffer,uint_32 read_size,uint_32* size);
void pack_package(ubyte_8* buffer,uint_32 buffer_len,uint_32* buffer_size);
void RT_handle_package(ubyte_8 *buffer,uint_32 n);
int get_child_port_pos(uint_32 child_port);
uint_32 get_child_port(int pos);
uint_32 get_child_port_array_len(void);
#endif
