/*
 *control_traffic_light.h,声明操作交通灯数据结构的各种操作函数。
 */
#ifndef CONTROL_TRAFFIC_LIGHT_H_
#define CONTROL_TRAFFIC_LIGHT_H_
#include "m_type.h"
#include "traffic_light.h"
#define TRAFFIC_REPOS_ARRAY_MAX_LEN 10
void* get_traffic_repos_node(uint_32 traffic_repos_id);
uint_32  config_traffic_repos(byte_8* bus_type,byte_8* bus_lid);
void config_traffic(void);
bool  traffic_repos_scan_func(uint_32 traffic_repos_id);
void* traffic_repos_scan_pthread_func(void* argc);
void create_traffic_repos_scan_unit(void);
void  set_traffic_light(uint_32 traffic_repos_id,uint_32 light_pos,TRAFFIC_STATUS traffic_status,IS_LOADED is_loaded,IS_BACK is_back);

void set_traffic_repos_disable(uint_32 traffic_repos_id);
void set_traffic_repos_enable(uint_32 traffic_repos_id);
void set_traffic_light_list_enable(uint_32 traffic_repos_id,uint_32 light_pos);
void set_traffic_light_list_disable(uint_32 traffic_repos_id,uint_32 light_pos);
uint_32 get_dev_light_pos(uint_32 traffic_repos_id,byte_8* dev_lid);
uint_32 get_light_pos(uint_32 traffic_repos_id,byte_8* RT_lid);
void reset_traffic_repos_light(uint_32 traffic_repos_id,uint_32 light_pos);
bool is_RT_section_readable(uint_32 traffic_repos_id,uint_32 light_pos);
bool is_RT_section_writeable(uint_32 traffic_repos_id,uint_32 light_pos);
bool is_dev_readable(uint_32 traffic_repos_id,uint_32 light_pos,uint_32 dev_pos);
bool is_red_light(uint_32 traffic_repos_id,uint_32 light_pos,byte_8* dev_lid);
uint_32 scan_RT_section(uint_32 traffic_repos_id);
uint_32 get_scan_pos(uint_32 traffic_repos_id);
byte_8* get_RT_section_RT_lid(uint_32 traffic_repos_id,uint_32 light_pos);
#endif
