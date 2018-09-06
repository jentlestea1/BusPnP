#include "BC_control.h"
#include "relevant_struct_def.h"
#include "handle_data_package.h"
#include "config_1553.h"
#include "route_map.h"
#include "stdio.h"
#include "string.h"
#define BC_DEVICE_NUM 1

static bool is_read_buf_1553_avail[READ_BUF_1553_MAX_SIZE][RT_MAX_NUM]={0};

/** 
 * 获取RT子地址列表，按优先级递减
 * @param[in]   port    RT对应的配置地址（端口号）
 * @param[in]   buf    用于获取对应配置地址RT的子地址(子端口号)列表的缓存指针
 * @return		子地址的数量 
 */ 
uint_32 get_RT_sub_addr_array(uint_32 port,uint_32 *buf){
    
    /*
     * 根据RT_physical_addr找到RT_lid
     *依次按优先级找到其下的dev_lid,
     *再把每个dev相应的subaddr放到
     *child_port_array中
     */

    printf("正在为地址为%d的RT分配子地址...\n",port);
    uint_32 len=0;
    uint_32 config_1553_id=get_device_num_1553_bus_config_id(BC_DEVICE_NUM);
    if(config_1553_id==-1)return 0;
    void* p_confgi_node=get_config_node(config_1553_id);
    uint_32 pos=get_config_light_pos(config_1553_id,port);
    byte_8* RT_lid=get_config_node_RT_lid(p_confgi_node,pos);
    route r;
    get_RT_route_map(RT_lid,&r);
    uint_32 cur_prio=0;
    uint_32 prev_priority=MAX_PRIORITY; 
    uint_32 anchor=0;
    uint_32 child_port;
    byte_8* dev_lid=get_priority_deterio_dev_lid(get_route_bus_type(r),get_route_bus_lid(r),get_route_RT_lid(r),SEND_PRIORITY_FLAG,prev_priority,&cur_prio,&anchor);
    prev_priority=cur_prio;
    while(strcmp(dev_lid,"")!=0){
        child_port=get_RT_physical_sub_addr(get_route_bus_type(r),get_route_bus_lid(r),get_route_RT_lid(r),dev_lid);
        buf[len++]=child_port;
        dev_lid=get_priority_deterio_dev_lid(get_route_bus_type(r),get_route_bus_lid(r),get_route_RT_lid(r),SEND_PRIORITY_FLAG,prev_priority,&cur_prio,&anchor);
        prev_priority=cur_prio;
    }
    int i=0;
    for(;i<len;i++){
        printf("port:%d ",buf[i]);
    }
    return len;
}


bool get_buffer_is_avail(uint_32 config_id,uint_32 RT_config_id){
    return is_read_buf_1553_avail[config_id][RT_config_id];
}

bool set_buffer_unavail(uint_32 config_id,uint_32 RT_config_id){
    is_read_buf_1553_avail[config_id][RT_config_id]=false;
}

bool set_buffer_avail(uint_32 config_id,uint_32 RT_config_id){
    is_read_buf_1553_avail[config_id][RT_config_id]=true;
}

uint_32 get_RT_addr_array(uint_32 * buf){
    int i=0;
    int count=0;
    uint_32 len=get_config_len();
    for(i=0;i<len;i++){
		void* p_config_node_tmp=get_config_node(i);
		uint_32 RT_num=get_config_node_len(p_config_node_tmp);
		int j=0;
		for(j=0;j<RT_num;j++){
            uint_32 port_tmp=get_config_node_port(p_config_node_tmp,j);
            buf[count++]=port_tmp;
		}
    }
    return count;
}
