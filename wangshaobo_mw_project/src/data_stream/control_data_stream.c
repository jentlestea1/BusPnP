/*
 *control_data_stream.c
 */
#include "control_data_stream.h"
#include "data_stream.h"
#include "control_traffic_light.h"
#include "config_1553.h"
#include "handle_event.h"
#include "address_map.h"
#include "route_map.h"
#include "string.h"
#include "stdio.h"
#include "sync_collect.h"

void ctrl_app_write_data(uint_32 traffic_repos_id,byte_8* dev_lid,ubyte_8* buffer,uint_32 write_size,uint_32* size){
    app_write_data_func(dev_lid,buffer,write_size,size);
    if(*size==0)return;
    uint_32 size_tmp=*size;
    while(write_size-size_tmp){
        app_write_data_func(dev_lid,buffer+size_tmp,write_size-size_tmp,size);
        size_tmp+=*size;
    }
    *size=size_tmp;
}


void ctrl_app_read_data(uint_32 traffic_repos_id,byte_8* dev_lid,ubyte_8* buffer,uint_32 read_size,uint_32* size,void* time){
    app_read_data_func(dev_lid,buffer,read_size,size,time);
    if(*size==0)return;
}


void ctrl_dev_write_data(uint_32 traffic_repos_id,byte_8* dev_lid,ubyte_8* buffer,uint_32 write_size,uint_32* size){
    if(write_size==0||buffer[0]=='\0'){
        *size=0;
        return;
    }
    dev_write_data_func(dev_lid,buffer,write_size,size);
    if(*size==0)return;
    uint_32 size_tmp=*size;
    while(write_size-size_tmp){
        dev_write_data_func(dev_lid,buffer+size_tmp,write_size-size_tmp,size);
        size_tmp+=*size;
    }
    *size=size_tmp;
}


void ctrl_dev_read_data(uint_32 traffic_repos_id,byte_8* dev_lid,ubyte_8* buffer,uint_32 read_size,uint_32* size,void* time){
    uint_32 light_pos=get_dev_light_pos(traffic_repos_id,dev_lid);
    route r;
    get_dev_route_map(dev_lid,&r);
    if(is_red_light(traffic_repos_id,light_pos,dev_lid)){
        dev_read_data_func(dev_lid,buffer,read_size,size,time);//红灯读出丢弃数据,抛出事件
        memset(buffer,0,read_size);
        if(*size==0){
            //抛出事件
            byte_8* RT_lid=get_route_RT_lid(r);
            throw_event(0,RT_lid,EVT_1553_RECV_COMMAND_ERR);
            return;
        }
        *size=0;
        return;
    }
    dev_read_data_func(dev_lid,buffer,read_size,size,time);
    if(*size==0)return;
}


/*读写都为读写block_size*/
void read_data(byte_8* dev_lid,ubyte_8* buffer,uint_32 buffer_size,uint_32* size,void* time,AUTO_SET auto_set,uint_32 set_size){
    route r;
    get_dev_route_map(dev_lid,&r);
    byte_8* bus_type=get_route_bus_type(r);
    byte_8* bus_lid=get_route_bus_lid(r);
    byte_8* RT_lid=get_route_RT_lid(r);
    uint_32 write_region_size=get_write_region_size(bus_type,bus_lid,RT_lid,dev_lid);
    void *p=get_sync_collect(HASH_CONTROL_APP_READ_FLAG,0,0,dev_lid);
    FLAG flag=FLAG1;
    if(write_region_size==0){
        flag=get_sync_collect_flag(p,false);
        if(flag==FLAG1){
            write_sync_collect_flag(p,FLAG2,false);   //FLAG1正常，FLAG2阻塞
            vi_wait(p);
            write_sync_collect_flag(p,FLAG1,false);
        }
        else {
            write_sync_collect_flag(p,FLAG1,false);
            throw_event(0,NULL,EVT_SYNC_FLAG_WRITE_ERR);
        }
    }
    uint_32 read_block_size=get_dev_trans_attr(bus_type,bus_lid,RT_lid,dev_lid,SEND_BLOCK_FLAG);
    if(read_block_size>buffer_size){
        *size=0;
        throw_event(0,RT_lid,EVT_APP_READ_BLOCK_OVERFLOW);
        return;//抛出事件
    }
    if(auto_set==AUTO){
        if(write_region_size<read_block_size){
            *size=0;
            return;
        }
    }
    else{
        if(set_size>0){
            read_block_size=set_size;
        }
    }
    uint_32 config_id=get_1553_bus_config_id(bus_lid);
    void* p_config_node=get_config_node(config_id);
    uint_32 traffic_repos_id=get_config_node_traffic_repos_id(p_config_node);
    ctrl_app_read_data(traffic_repos_id,dev_lid,buffer,read_block_size,size,time);
}


void write_data(byte_8* dev_lid,ubyte_8* buffer,uint_32 buffer_size,uint_32* size){
    route r;
    get_dev_route_map(dev_lid,&r);
    byte_8* bus_type=get_route_bus_type(r);
    byte_8* bus_lid=get_route_bus_lid(r);
    byte_8* RT_lid=get_route_RT_lid(r);
    uint_32 send_block_size=get_dev_trans_attr(bus_type,bus_lid,RT_lid,dev_lid,RECEIVE_BLOCK_FLAG);
    if(send_block_size>buffer_size){
        *size=0;
        throw_event(0,RT_lid,EVT_APP_WRITE_BLOCK_INVALID);
        return;//抛出事件
    }
    else if(send_block_size<buffer_size){
        throw_event(0,RT_lid,EVT_APP_WRITE_BLOCK_OVERFLOW);
        *size=0;
        return;
    }
    uint_32 config_id=get_1553_bus_config_id(bus_lid);
    void* p_config_node=get_config_node(config_id);
    uint_32 traffic_repos_id=get_config_node_traffic_repos_id(p_config_node);
    ctrl_app_write_data(traffic_repos_id,dev_lid,buffer,send_block_size,size);

}


