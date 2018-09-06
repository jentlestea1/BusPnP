/*
 *声明交通灯结构。
 */
#ifndef TRAFFIC_H_
#define TRAFFIC_H_
#include "m_type.h"
#define TRAFFIC_STATUS uint_32
#define UNCHECKED 4
#define GREEN 1
#define YELLOW 2
#define RED 3
#define IS_BACK uint_32
#define NOT_BACK 2
#define BACK 1
#define IS_LOADED uint_32
#define LOADED 1
#define NOT_LOADED 2
#define TRAFFIC_ENABLE 1
#define TRAFFIC_DISABLE 0
#define TRAFFIC_REPOS_MAX_LEN 10  //最多挂二十根总线
#define TRAFFIC_LIST_MAX_LEN 32
#define PRIORITY_LOW 1
#define PRIORITY_HIGH 2 
#define NOT_SET -2

typedef struct traffic_light{
    byte_8* dev_lid;
    TRAFFIC_STATUS traffic_status;
    IS_LOADED is_loaded;
    IS_BACK is_back;
}traffic_light;

typedef struct traffic_light_repos{
    traffic_light* p_traffic_light_list[TRAFFIC_LIST_MAX_LEN];
    uint_32 list_len;
    byte_8* RT_lid_array[TRAFFIC_LIST_MAX_LEN];
    uint_32 RT_section_priority[TRAFFIC_LIST_MAX_LEN];
    bool is_RT_section_enable[TRAFFIC_LIST_MAX_LEN];
    uint_32 dev_num[TRAFFIC_LIST_MAX_LEN];
    bool is_traffic_enable;
    uint_32 scan_pos;
}traffic_light_repos;
void* create_traffic_repos(byte_8* bus_type,byte_8* bus_lid);//模拟交通灯控制以总线为单位
#endif

