#include "control_package.h"
#include "package.h"
#include "control_traffic_light.h"
#include "handle_event.h"
#include "config_1553.h"
#include "route_map.h"

void ctrl_unpack_package_to_1553(uint_32 traffic_repos_id,uint_32 port,ubyte_8* buffer,uint_32 buf_size){
    uint_32 bus_config_id=get_1553_bus_config_id_t(traffic_repos_id);
    if(bus_config_id==-1)return;
    uint_32 light_pos_tmp=get_config_light_pos(bus_config_id,port);
    byte_8* RT_lid_tmp=get_RT_section_RT_lid(traffic_repos_id,light_pos_tmp);
    route r;
    get_RT_route_map(RT_lid_tmp,&r);
    unpack_package_to_1553(traffic_repos_id,buffer,buf_size,get_route_bus_type(r),\
            get_route_bus_lid(r),get_route_RT_lid(r));
}

void ctrl_pack_package_to_1553(uint_32 traffic_repos_id,ubyte_8* buffer_1553,uint_32* buf_size,int* light_pos){
    int light_pos_tmp=get_scan_pos(traffic_repos_id);
    *light_pos=light_pos_tmp;
    if(light_pos_tmp==-1){
        *buf_size=0;
        return;
    }
    byte_8* RT_lid_tmp=get_RT_section_RT_lid(traffic_repos_id,light_pos_tmp);
    route r;
    get_RT_route_map(RT_lid_tmp,&r);
    pack_package_to_1553(traffic_repos_id,light_pos_tmp,get_route_bus_type(r),get_route_bus_lid(r),get_route_RT_lid(r),buffer_1553,buf_size);
}

