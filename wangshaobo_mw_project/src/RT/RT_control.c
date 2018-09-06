#include "RT_control.h"
#include "relevant_struct_def.h"
#include "RT_socket.h"
#include <unistd.h>
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
#include "file.h"
#include "handle_event.h"
#ifdef __RT_VCAN_TRANSMIT
#include "vcan_frame.h"
#endif

static uint_32 s_pos[MOUNT_DEV_MAX_NUM]={0};
static uint_32 e_pos[MOUNT_DEV_MAX_NUM]={0};
static ubyte_8 write_buffer[MOUNT_DEV_MAX_NUM][CACHE_MAX_SIZE]={0};
static uint_32 child_port_array[MOUNT_DEV_MAX_NUM]={0};
static uint_32 len=0;
void clear_write_buffer(){
    int i=0;
    for(;i<MOUNT_DEV_MAX_NUM;i++){
        memset(write_buffer[i],0,CACHE_MAX_SIZE);
    }
}

void write_write_buffer(uint_32 child_port,ubyte_8* buffer,uint_32 write_size,uint_32* size){
    int i=0;
    uint_32 size_tmp=0;
    int buffer_pos;
    buffer_pos=get_child_port_pos(child_port);
    if(buffer_pos==-1)return;
    uint_32 write_size_tmp=write_size;
    if(write_size_tmp!=0){
        while(s_pos[buffer_pos]!=(e_pos[buffer_pos]+1)%CACHE_MAX_SIZE){
            memcpy(write_buffer[buffer_pos]+e_pos[buffer_pos],buffer+size_tmp,sizeof(byte_8));
            size_tmp++;
            write_size_tmp--;
            e_pos[buffer_pos]=(e_pos[buffer_pos]+1)%CACHE_MAX_SIZE;
            if(write_size_tmp==0)break;
        }
    }
    *size=size_tmp;
}

void read_write_buffer(uint_32 pos,ubyte_8* buffer,uint_32 read_size,uint_32* size){
    uint_32 size_tmp=0;
    if(pos<0||pos>=len)return;
    while(s_pos[pos]!=e_pos[pos]){
        if(size_tmp==read_size)break;
        memcpy(buffer+size_tmp,write_buffer[pos]+s_pos[pos],sizeof(byte_8));
        size_tmp++;
        s_pos[pos]=(s_pos[pos]+1)%CACHE_MAX_SIZE;
    }
    *size=size_tmp;
}

//void init_port_array(uint_32 RT_physical_addr){
    /*
     * 根据RT_physical_addr找到RT_lid
     *依次按优先级找到其下的dev_lid,
     *再把每个dev相应的subaddr放到
     *child_port_array中
     */
/*
    printf("正在为地址为%d的RT分配子地址...\n",RT_physical_addr);
    uint_32 config_1553_id=get_device_num_1553_bus_config_id(BC_device_num);
    if(config_1553_id==-1)return;
    void* p_confgi_node=get_config_node(config_1553_id);
    uint_32 pos=get_config_light_pos(config_1553_id,RT_physical_addr);
    byte_8* RT_lid=get_config_node_RT_lid(p_confgi_node,pos);
    void* p_route=get_route_node();
    get_RT_route_map(RT_lid,&p_route);
    uint_32 cur_prio=0;
    uint_32 prev_priority=MAX_PRIORITY; 
    uint_32 anchor=0;
    uint_32 child_port;
    byte_8* dev_lid=get_priority_deterio_dev_lid(get_route_bus_type(p_route),get_route_bus_lid(p_route),get_route_RT_lid(p_route),SEND_PRIORITY_FLAG,prev_priority,&cur_prio,&anchor);
    prev_priority=cur_prio;
    while(strcmp(dev_lid,"")!=0){
        child_port=get_RT_physical_sub_addr(get_route_bus_type(p_route),get_route_bus_lid(p_route),get_route_RT_lid(p_route),dev_lid);
        child_port_array[len++]=child_port;
        dev_lid=get_priority_deterio_dev_lid(get_route_bus_type(p_route),get_route_bus_lid(p_route),get_route_RT_lid(p_route),SEND_PRIORITY_FLAG,prev_priority,&cur_prio,&anchor);
        prev_priority=cur_prio;
    }
    free_route_node(&p_route);
    int i=0;
    for(;i<len;i++){
        printf("port:%d ",child_port_array[i]);
    }
    printf("完成子地址分配...\n");
}
*/

void init_port_array(uint_32 *RT_sub_addr_array,uint_32 size){
    int i=0;
    if(size>=MOUNT_DEV_MAX_NUM){
        throw_event(0,NULL,RT_EVT_INITPORT_ERR);
        return;
    }
    len=size;
    for(i=0;i<size;i++){
        uint_32 sub_addr_value=*(RT_sub_addr_array+i);
#ifdef __RT_VCAN_TRANSMIT
        sub_addr_value=htonl(sub_addr_value);
#endif
        child_port_array[i]=sub_addr_value;
        printf("child_port[%d]:%d\n",i,child_port_array[i]);
    }
}

void pack_package(ubyte_8* buffer,uint_32 buffer_len,uint_32* buffer_size){
    //获取数据
    ubyte_8*p_buffer_data;
#ifdef __RT_TCPIP_TRANSMIT
    p_buffer_data=buffer+RT_PACKAGE_HEADER_SIZE_LEN;
#elif __RT_VCAN_TRANSMIT
    p_buffer_data=buffer;
#endif
    uint_32 read_size=READ_MAX_SIZE_RT;
    byte_8 read_buffer_tmp[CACHE_MAX_SIZE]={0};
    uint_32 size;
    uint_32 buffer_pos=0;
    uint_32 pos=0;
    bool is_send_valid=false;
    uint_32 tmp;
    for(;pos<len;pos++){
        read_write_buffer(pos,read_buffer_tmp,read_size,&size);
        if(size!=0)is_send_valid=true;
        tmp=size;
        tmp|=RT_DATA_BLOCK_VALID_PREFIX;
        *(p_buffer_data+buffer_pos)=tmp;
        buffer_pos++;
        if(buffer_pos>=buffer_len){   //如果这里出错，可以适当增大发送频率或者缓冲区大小
            is_send_valid=false;
            break;
        }
        memcpy(p_buffer_data+buffer_pos,read_buffer_tmp,size);
        buffer_pos+=size;
        *(p_buffer_data+buffer_pos)='\0';
        memset(read_buffer_tmp,0,CACHE_MAX_SIZE);
    }
    if(is_send_valid==true){
#ifdef __RT_TCPIP_TRANSMIT
        *(uint_32 *)buffer=buffer_pos;
        *buffer_size=buffer_pos+RT_PACKAGE_HEADER_SIZE_LEN;
#elif __RT_VCAN_TRANSMIT
        *buffer_size=buffer_pos;
#endif
    }
    else{
        *buffer_size=0;
#ifdef __RT_TCPIP_TRANSMIT
        memset(buffer,0,buffer_pos+RT_PACKAGE_HEADER_SIZE_LEN);
#elif __RT_VCAN_TRANSMIT
        memset(buffer,0,buffer_pos);
#endif
    }
}

void RT_handle_package(ubyte_8 *buffer,uint_32 n){
    if(n==0)return;
#ifdef __RT_TCPIP_TRANSMIT
    uint_32 size=*(uint_32 *)buffer;
    buffer+=RT_PACKAGE_HEADER_SIZE_LEN;
    n-=RT_PACKAGE_HEADER_SIZE_LEN;
#endif
 	if(n!=0&&n!=-1){
            /*解包过程*/
            ubyte_8 send_buffer[4096]={0};
            uint_32 size;
            uint_32 is_valid;
            uint_32 pos=0;
            uint_32 port_pos=0;
	        uint_32 child_port;
            while(pos<n){
                size=buffer[pos]%0x80;
                is_valid=buffer[pos]/0x80;
                if(is_valid==0){
                    if(size==0)pos++;
                    else pos+=size;
                    port_pos++;
                    continue;
                }
                if(size==0){
                    pos++;
                    port_pos++;
                    continue;
                }
                pos++;
                memcpy(send_buffer,buffer+pos,size);
                pos+=size;
                send_buffer[size]='\0';
                if(port_pos<get_child_port_array_len()){
                    child_port=get_child_port(port_pos);
                    port_pos++;
                }
                else{
                    throw_event(0,NULL,RT_EVT_HANDLE_PACKAGE_ERR);
                }
                float_64 d;
                /*测试用*/
                if(child_port==8003){
                    float_64 d1=send_buffer[0];
                    float_64 d2=send_buffer[1];
                    float_64 d3=send_buffer[2];
                    d=d1+pow(0.1,d3)*d2;
                }
                else {
                    d=send_buffer[0];
                }
                printf("位置：RT；类型：->接收；数据：%lf；大小：%d；端口：%d    ",d,size,child_port);
                add_string(RECEIVE,d,child_port);
                timeStamp t;
                get_current_time(&t);
                print_time(&t);
                memset(send_buffer,0,4096);
            }
            if(pos!=n){
                throw_event(0,NULL,RT_EVT_HANDLE_PACKAGE_ERR);
            }
	}
}

int get_child_port_pos(uint_32 child_port){
    int pos=-1;
    int i=0;
    for(;i<len;i++){
        if(child_port_array[i]==child_port){
            pos=i;
            break;
        }
    }
    return pos;
}

uint_32 get_child_port(int pos){
    uint_32 port_tmp=-1;
    if(pos>=len||pos<0)return 0;
    else port_tmp=child_port_array[pos];
    return port_tmp;
}

uint_32 get_child_port_array_len(){
    return len;
}

