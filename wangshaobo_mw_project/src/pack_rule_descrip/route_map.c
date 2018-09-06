#include "route_map.h"
#include "stdlib.h"
#include "string.h"

//将堆换成栈

void get_RT_route_map(byte_8* RT_lid,/*void** pp_route*/route *r){
    if(strcmp(RT_lid,"")==0)return;
    uint_32 irdirect_pos=find_irdirect_RT_pos(RT_lid);
    r->bus_type=get_irdirect_bus_type(irdirect_pos);
    r->bus_lid=get_irdirect_bus_lid(irdirect_pos);
    r->RT_lid=get_irdirect_RT_lid(irdirect_pos);
}

void get_dev_route_map(byte_8* dev_lid,/*void** pp_route*/route *r){
    if(strcmp(dev_lid,"")==0)return;
    uint_32 irdirect_pos=find_irdirect_dev_pos(dev_lid);
    r->bus_type=get_irdirect_bus_type(irdirect_pos);
    r->bus_lid=get_irdirect_bus_lid(irdirect_pos);
    r->RT_lid=get_irdirect_RT_lid(irdirect_pos);
}

byte_8* get_route_bus_type(route r){
    return r.bus_type;
}

byte_8* get_route_bus_lid(route r){

    return r.bus_lid;
}

byte_8* get_route_RT_lid(route r){
    return r.RT_lid;
    
}

