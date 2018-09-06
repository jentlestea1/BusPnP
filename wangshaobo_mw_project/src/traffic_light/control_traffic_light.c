/*
 *control_traffic_light.c,定义操作交通灯数据结构的各种操作结构。
 */
#include "compile_type.h"
#include "control_traffic_light.h"
#include "unistd.h"
#include "handle_event.h"
#include "route_map.h"
#include "address_map.h"
#ifdef __GCC_C99
#include <pthread.h>
#elif __SPARC_GCC_MMU
#include <fsu_pthread.h>
#endif
#include "stdio.h"
#include "string.h"
#include "interval.h"
#include "sync_collect.h"
#define MAX_MSEC 200
#define MIN_MSEC 10

static traffic_light_repos* p_traffic_light_repos_array[TRAFFIC_REPOS_ARRAY_MAX_LEN];
uint_32 config_traffic_repos(byte_8* bus_type,byte_8* bus_lid){
    int i=0;
    printf("正在为bus_type:%s,bus_lid:%s的总线创建交通灯库...\n",bus_type,bus_lid);
    for(;i<TRAFFIC_REPOS_ARRAY_MAX_LEN;i++){
        if(p_traffic_light_repos_array[i]==NULL){
            void* p_tmp=create_traffic_repos(bus_type,bus_lid);
            p_traffic_light_repos_array[i]=p_tmp;
            break;
        }
    }
    if(i>=TRAFFIC_REPOS_ARRAY_MAX_LEN||i<0)i=-1;
    return i;
}
void* get_traffic_repos_node(uint_32 traffic_repos_id){
    return p_traffic_light_repos_array[traffic_repos_id];
}
void config_traffic(){
    uint_32 form_num=get_form_num();
    int i=0;
    for(;i<form_num;i++){
        void* p_form_item=get_forms_item(i);
        byte_8* bus_type=get_form_bus_type(p_form_item);
        byte_8* bus_lid=get_form_bus_lid(p_form_item);
        config_traffic_repos(bus_type,bus_lid);
    }
}
void* traffic_repos_scan_pthread_func(void* argc){
#ifdef PTHREAD_RECYCLE
    pthread_detach(pthread_self());
#endif
    int i=0;
    uint_32 msec=10;
    for(;i<TRAFFIC_REPOS_ARRAY_MAX_LEN;i++){
        if(p_traffic_light_repos_array[i]!=NULL){
            while(true){
                bool scan_ret=false;
                scan_ret=traffic_repos_scan_func(i);
                if(scan_ret==false){
                    msec*=2;
                    msec=msec>MAX_MSEC?MAX_MSEC:msec;
                }
                else{
                    msec=MIN_MSEC;
                }
        sleep_ms(msec);
            }
        }
    }
}

void  set_traffic_light(uint_32 traffic_repos_id,uint_32 light_pos,TRAFFIC_STATUS traffic_status,IS_LOADED is_loaded,IS_BACK is_back){
    if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)return;  
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id];
    int i=0;
    for(;i<p_repos_tmp->list_len;i++){
        traffic_light* p_list_tmp=p_repos_tmp->p_traffic_light_list[i];
        traffic_light* p=p_list_tmp+light_pos;
        if(traffic_status!=NOT_SET){
            p->traffic_status=traffic_status;
        }
        if(is_loaded!=NOT_SET)
            p->is_loaded=is_loaded;
        if(is_back!=NOT_SET)
            p->is_back=is_back;
    }
}

//true代表扫描到数据
bool traffic_repos_scan_func(uint_32 traffic_repos_id){
    /*
     * 刷新，10ms间隔
     */
    bool ret=false;
    if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)\
        return false;
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id];
        if(p_repos_tmp->is_traffic_enable==TRAFFIC_ENABLE){
            int i=0;
            for(;i<p_repos_tmp->list_len;i++){
                int j=0;
                traffic_light* p_light_tmp=p_repos_tmp->p_traffic_light_list[i];
                route r;
                for(;j<p_repos_tmp->dev_num[i];j++){
                        if(p_light_tmp->is_loaded==LOADED){
                            ret=true;
                            if(p_light_tmp->traffic_status==UNCHECKED){
                                p_light_tmp->traffic_status=GREEN;
                            }
                            else if(p_light_tmp->traffic_status==GREEN){
                                p_light_tmp->traffic_status=YELLOW;
                                p_repos_tmp->RT_section_priority[i]=PRIORITY_HIGH;
                            }
                            else if(p_light_tmp->traffic_status==YELLOW){
                                p_light_tmp->traffic_status=RED;
                            }
                        }
                        byte_8* dev_lid=p_light_tmp->dev_lid;
                        get_dev_route_map(dev_lid,&r);
                        byte_8* bus_type=get_route_bus_type(r);
                        byte_8* bus_lid=get_route_bus_lid(r);
                        byte_8* RT_lid=get_route_RT_lid(r);
                        uint_32 dev_read_block_size_tmp=get_dev_trans_attr(bus_type,bus_lid,RT_lid,dev_lid,RECEIVE_BLOCK_FLAG);
                        uint_32 dev_read_buffer_size;
                        if(!is_read_region_empty(bus_type,bus_lid,RT_lid,dev_lid)){
                            ret=true;
                            p_light_tmp->traffic_status=GREEN;
                            p_light_tmp->is_loaded=LOADED;
                            if(dev_read_buffer_size=get_read_region_size(bus_type,bus_lid,RT_lid,dev_lid)>=READ_REGION_MAX_SIZE/2){
                                p_repos_tmp->RT_section_priority[j]=PRIORITY_HIGH;
                                if(dev_read_buffer_size%dev_read_block_size_tmp!=0){
                                    throw_event(0,RT_lid,EVT_APP_DATA_SIZE_ERR);
                                }
                            }
                            else{
                                p_repos_tmp->RT_section_priority[j]=PRIORITY_LOW;
                            }

                        }
                        else{
                            p_light_tmp->traffic_status=UNCHECKED;
                        }
                        if(!is_write_region_empty(bus_type,bus_lid,RT_lid,dev_lid)){
                            p_light_tmp->is_back=BACK;
                            void *p=get_sync_collect(HASH_CONTROL_APP_READ_FLAG,0,0,dev_lid);
                            FLAG flag=get_sync_collect_flag(p,false);
                            if(flag==FLAG2){                    //FLAG2阻塞 FLAG1正常
                                vi_signal(p);
                            }
                        }
                        else{
                            p_light_tmp->is_back=NOT_BACK;
                        }
                        p_light_tmp++;
                }
            }
        }
        return ret;
}

void set_traffic_repos_disable(uint_32 traffic_repos_id){
  if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)return;
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id];
    p_repos_tmp->is_traffic_enable=TRAFFIC_DISABLE;
}

void set_traffic_repos_enable(uint_32 traffic_repos_id){
  if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)return;
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id];
    p_repos_tmp->is_traffic_enable=TRAFFIC_ENABLE;
}

void set_traffic_light_list_enable(uint_32 traffic_repos_id,uint_32 light_pos){
    if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)return;
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id]; 
    if(light_pos>=TRAFFIC_LIST_MAX_LEN)return;
    p_repos_tmp->is_RT_section_enable[light_pos]=true;
}

void set_traffic_light_list_disable(uint_32 traffic_repos_id,uint_32 light_pos){
    if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)return;
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id]; 
    if(light_pos>=TRAFFIC_LIST_MAX_LEN)return;
    p_repos_tmp->is_RT_section_enable[light_pos]=false;
    
}

void reset_traffic_repos_light(uint_32 traffic_repos_id,uint_32 light_pos){
    if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)return;
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id];
    if(light_pos>=TRAFFIC_LIST_MAX_LEN)return;
    traffic_light* p_light_tmp=p_repos_tmp->p_traffic_light_list[light_pos];
    if(p_light_tmp==NULL)return;

    p_repos_tmp->RT_section_priority[light_pos]=PRIORITY_LOW;
    p_light_tmp->traffic_status=UNCHECKED;
    p_light_tmp->is_loaded=NOT_LOADED;
}

uint_32 get_dev_light_pos(uint_32 traffic_repos_id,byte_8* dev_lid){
    if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)return -1;  
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id];
    int i=0;
    uint_32 light_pos=-1;
    for(;i<p_repos_tmp->list_len;i++){
        traffic_light* p_list_tmp=p_repos_tmp->p_traffic_light_list[i];
        int j=0;
        for(;j<p_repos_tmp->dev_num[i];j++){
            traffic_light* p=p_list_tmp+j;
            if(strcmp(p->dev_lid,dev_lid)==0){
                light_pos=i;
                break;
            }
        }
        if(light_pos!=-1)break;
    }
    return light_pos;
}

uint_32 get_light_pos(uint_32 traffic_repos_id,byte_8* RT_lid){
    if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)return -1;  
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id]; 
    int i=0;
    uint_32 light_pos=-1;
    for(;i<p_repos_tmp->list_len;i++){
        byte_8* RT_lid_tmp=p_repos_tmp->RT_lid_array[i];
        if(strcmp(RT_lid,RT_lid_tmp)==0)light_pos=i;
    }
    return light_pos;
}

bool is_RT_section_readable(uint_32 traffic_repos_id,uint_32 light_pos){
    if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)return false;
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id]; 
    if(p_repos_tmp->is_traffic_enable==false)return false;
    if(light_pos>=TRAFFIC_LIST_MAX_LEN)return false;
    return p_repos_tmp->is_RT_section_enable[light_pos];
}

bool is_RT_section_writeable(uint_32 traffic_repos_id,uint_32 light_pos){        
    if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)return false;
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id]; 
    if(p_repos_tmp->is_traffic_enable==false)return false;
    if(light_pos>=TRAFFIC_LIST_MAX_LEN)return false;
    return p_repos_tmp->is_RT_section_enable[light_pos];
}

bool is_dev_readable(uint_32 traffic_repos_id,uint_32 light_pos,uint_32 dev_pos){
    if(is_RT_section_readable(traffic_repos_id,light_pos)==false)return false;
    if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)return false;
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id];
    if(light_pos>=TRAFFIC_LIST_MAX_LEN)return false;
    traffic_light* p_light_tmp=p_repos_tmp->p_traffic_light_list[light_pos];
    if(p_light_tmp==NULL)return false;
    if((p_light_tmp+dev_pos)->traffic_status==GREEN||(p_light_tmp+dev_pos)->traffic_status==YELLOW||(p_light_tmp+dev_pos)->traffic_status==RED){
        return true;//红灯先保留
    }
    else return false;
}

bool is_red_light(uint_32 traffic_repos_id,uint_32 light_pos,byte_8* dev_lid){
    if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)return false;
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id];
    if(light_pos>=TRAFFIC_LIST_MAX_LEN)return false;
    traffic_light* p_light_tmp=p_repos_tmp->p_traffic_light_list[light_pos];
    if(p_light_tmp==NULL)return false;
    int i=0;
    for(;i<p_repos_tmp->dev_num[light_pos];i++){
        if(strcmp(dev_lid,(p_light_tmp+i)->dev_lid)==0){
            if((p_light_tmp+i)->traffic_status==RED){
                return true;
            }   
        }
    }
    return false;
    
}

uint_32 get_scan_pos(uint_32 traffic_repos_id){ 
    uint_32 next_pos=-1; if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)return -1;
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id];
    uint_32 pos_tmp=p_repos_tmp->scan_pos;
    int i=pos_tmp==-1?0:pos_tmp;
    uint_32 list_len_tmp=p_repos_tmp->list_len;
    int j=(i+1)%list_len_tmp;
    uint_32 count_tmp=list_len_tmp;
    while(count_tmp--){
        uint_32 light_pos_tmp=j;
        if(p_repos_tmp->RT_section_priority[light_pos_tmp]==PRIORITY_HIGH){
            traffic_light* p_light_tmp=p_repos_tmp->p_traffic_light_list[light_pos_tmp];
            uint_32 dev_num_tmp=p_repos_tmp->dev_num[light_pos_tmp];
            uint_32 k=0;
            for(;k<dev_num_tmp;k++){
                if(is_dev_readable(traffic_repos_id,light_pos_tmp,k)){
                    next_pos=light_pos_tmp;
                    break;
                }
            }
        }
        if(next_pos!=-1)break;
        j=(j+1)%list_len_tmp;
    }
    count_tmp=list_len_tmp;
    j=(i+1)%list_len_tmp;
    if(next_pos==-1){
        while(count_tmp--){
            uint_32 light_pos_tmp=j;
            if(p_repos_tmp->RT_section_priority[light_pos_tmp]==PRIORITY_LOW){
                    traffic_light* p_light_tmp=p_repos_tmp->p_traffic_light_list[light_pos_tmp];
                    uint_32 dev_num_tmp=p_repos_tmp->dev_num[light_pos_tmp];
                    uint_32 k=0;
                    for(;k<dev_num_tmp;k++){
                        if(is_dev_readable(traffic_repos_id,light_pos_tmp,k)){
                        next_pos=light_pos_tmp;
                        break;
                    }
                }
            }
        if(next_pos!=-1)break;
        j=(j+1)%list_len_tmp;
        }   
    }
    p_repos_tmp->scan_pos=next_pos;
    return pos_tmp;
}

byte_8* get_RT_section_RT_lid(uint_32 traffic_repos_id,uint_32 light_pos){
    byte_8* RT_lid="";
    if(traffic_repos_id>=TRAFFIC_REPOS_ARRAY_MAX_LEN||p_traffic_light_repos_array[traffic_repos_id]==NULL)return RT_lid;
    traffic_light_repos* p_repos_tmp=(traffic_light_repos*)p_traffic_light_repos_array[traffic_repos_id];
    if(light_pos>=TRAFFIC_LIST_MAX_LEN)return RT_lid;
    RT_lid=p_repos_tmp->RT_lid_array[light_pos];
    return RT_lid;
}

void create_traffic_repos_scan_unit(void){
    pthread_t tid;
#ifdef __GCC_C99
    int err=pthread_create(&tid,NULL,traffic_repos_scan_pthread_func,NULL);
#elif __SPARC_GCC_MMU
    int err=pthread_create(&tid,NULL,(pthread_func_t)traffic_repos_scan_pthread_func,NULL);
#endif
    if(err!=0)printf("创建交通等库扫描线程失败...\n");
    else printf("成功创建交通灯库扫描线程,本扫描进程每50ms扫描一次所有的交通灯库...\n");
}


