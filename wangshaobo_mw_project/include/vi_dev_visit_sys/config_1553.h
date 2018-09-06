#ifndef CONFIG_1553_H_
#define CONFIG_1553_H_
#include "m_type.h"
#define READ_BUF_1553_MAX_SIZE 10
#define PORT_ARRAY_MAX_SIZE 32
#define RT_LID_ARRAY_MAX_SIZE 32
#define RT_MAX_NUM RT_LID_ARRAY_MAX_SIZE
#define LIGHT_POS_ARRAY_MAX_SIZE 32
#define BUF_MAX_LEN 1024
#define CONFIG_OVERFLOW 100000

typedef struct config_node{
    byte_8* bus_lid;
    uint_32 device_num_1553;
    uint_32 traffic_repos_id;
    uint_32 len;
    byte_8 RT_lid_array[RT_LID_ARRAY_MAX_SIZE][ATTR_LID_VALUE_MAX_LEN];
    uint_32 port_array[PORT_ARRAY_MAX_SIZE];
    uint_32 light_pos_array[LIGHT_POS_ARRAY_MAX_SIZE];
}config_node;
void config_1553(void);
void* get_config_list_entry(void);
void* get_config_node(uint_32 config_id);
byte_8* get_config_node_bus_lid(void* p_config_node);
uint_32  get_config_node_traffic_repos_id(void* p_config_node);
uint_32  get_config_node_len(void* p_config_node);
uint_32  get_config_node_port(void* p_config_node,uint_32 pos);
byte_8* get_config_node_RT_lid(void* p_config_node,uint_32 pos);
uint_32  get_config_node_light_pos(void* p_config_node,uint_32 pos);
uint_32 get_config_len(void);
uint_32 get_1553_bus_config_id(byte_8* bus_lid);
uint_32 get_RT_config_id(void* p_config_node,byte_8* RT_lid);
uint_32 get_device_num_1553_bus_config_id(uint_32 device_num_1553);
uint_32 get_config_light_pos(uint_32 config_id,uint_32 port);
uint_32 get_1553_bus_config_id_t(uint_32 traffic_repos_id);
#endif

