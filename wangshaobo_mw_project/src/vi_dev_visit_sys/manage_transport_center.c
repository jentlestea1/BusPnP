#include "manage_transport_center.h"
#include "handle_data_package.h"
#include "handle_event.h"
#include "control_package.h"
#include "config_1553.h"
#include "control_traffic_light.h"
#include "relevant_struct_def.h"
#include "BC_socket.h"
#include "route_map.h"
#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"
#include "BC_socket.h"
#include "data_stream.h"

void init_vi_dev_visit_sys(void){
    printf("------开始初始化虚拟设备访问系统------\n");
    create_data_trans_rule_form();
    config_address_map_index_list();
    init_event_repos_array();
    uint_32 event_repos_id;
    get_one_event_repos(&event_repos_id);
    /*创建事件库扫描线程,事件库一个就够了*/
    create_evt_repos_scan_unit();
    config_1553();
    /*创建交通灯库扫描线程*/
    create_traffic_repos_scan_unit();
    initialize_BC();
    printf("------初始化虚拟设备访问系统完成------\n");
    printf("\n            准备开始测试......\n\n");
}


void vi_app_read_data(byte_8* dev_lid,ubyte_8* buffer,uint_32 buf_size,uint_32* size,void* time,AUTO_SET auto_set,uint_32 set_size){
    read_data(dev_lid,buffer,buf_size,size,time,auto_set,set_size);
}

void vi_app_write_data(byte_8* dev_lid,ubyte_8* buffer,uint_32 write_size,uint_32* size){
    write_data(dev_lid,buffer,write_size,size);
}

void vi_set_bus_enable(byte_8* bus_lid){
    uint_32 config_id_1553=get_1553_bus_config_id(bus_lid);
    void* p_tmp=get_config_node(config_id_1553);
    uint_32 traffic_repos_id=get_config_node_traffic_repos_id(p_tmp);
    set_traffic_repos_enable(traffic_repos_id);
}

void vi_set_bus_disable(byte_8* bus_lid){
    uint_32 config_id_1553=get_1553_bus_config_id(bus_lid);
    void* p_tmp=get_config_node(config_id_1553);
    uint_32 traffic_repos_id=get_config_node_traffic_repos_id(p_tmp);
    set_traffic_repos_disable(traffic_repos_id);
}

void vi_set_RT_enable(byte_8* bus_lid,byte_8* RT_lid){
    uint_32 config_id_1553=get_1553_bus_config_id(bus_lid);
    void* p_tmp=get_config_node(config_id_1553);
    uint_32 traffic_repos_id=get_config_node_traffic_repos_id(p_tmp);
    uint_32 RT_config_id=get_RT_config_id(p_tmp,RT_lid);
    uint_32 light_pos=get_config_node_light_pos(p_tmp,RT_config_id);
    set_traffic_light_list_enable(traffic_repos_id,light_pos);
}

void vi_set_RT_disable(byte_8* bus_lid,byte_8* RT_lid){
    uint_32 config_id_1553=get_1553_bus_config_id(bus_lid);
    void* p_tmp=get_config_node(config_id_1553);
    uint_32 traffic_repos_id=get_config_node_traffic_repos_id(p_tmp);
    uint_32 RT_config_id=get_RT_config_id(p_tmp,RT_lid);
    uint_32 light_pos=get_config_node_light_pos(p_tmp,RT_config_id);
    set_traffic_light_list_disable(traffic_repos_id,light_pos);
}

