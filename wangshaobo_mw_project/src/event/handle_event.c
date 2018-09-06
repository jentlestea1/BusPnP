#include "compile_type.h"
#include "handle_event.h"
#include "handle_event_func.h"
#include "unistd.h"
#include "relevant_struct_def.h"
#include<stdlib.h>
#ifdef __GCC_C99
#include<pthread.h>
#elif __SPARC_GCC_MMU
#include<fsu_pthread.h>
#endif
#include<stdio.h>
#include "route_map.h"
#include "interval.h"

static void* p_repos_array[REPOS_MAX_LEN];
static uint_32 is_repos_avail[REPOS_MAX_LEN]={0};

void init_event_repos_array(){
    printf("初始化事件库仓库...\n");
    int i=0;
    for(;i<REPOS_MAX_LEN;i++){
        if(p_repos_array[i]!=NULL){
            free(p_repos_array[i]);
        }
        void* p_repos_tmp=create_evt_repos();
        p_repos_array[i]=p_repos_tmp;
        is_repos_avail[i]=1;
    }
    printf("初始化事件库仓库结束...\n");
}

void get_one_event_repos(uint_32* pos){
    int i=0;
    for(;i<REPOS_MAX_LEN;i++){
        if(is_repos_avail[i]==1){
            is_repos_avail[i]=0;
            *pos=i;
            printf("获取编号为%d的事件库...\n",i);
            return;
        }
    }
}

void handle_event(void* p_event){
    if(p_event==NULL)return;
    if(((evt*)p_event)->event_type==EVT_1553_RECV_COMMAND||((evt*)(p_event))->event_type==EVT_1553_RECV_COMMAND_ERR){
        handle_1553_func(p_event);
    }
    else if(((evt*)p_event)->event_type==EVT_1553_RETURN_DATA||((evt*)p_event)->event_type==EVT_1553_RETURN_DATA_ERR){
        handle_1553_ret_func(p_event);
    }
    else if(((evt*)p_event)->event_type==EVT_RT_RECV_DATA||((evt*)p_event)->event_type==EVT_RT_RECV_DATA_ERR){
        handle_RT_func(p_event);
    }
    else{
        byte_8* sel_RT_lid_tmp=((evt*)p_event)->sel_RT_lid;
        if(sel_RT_lid_tmp!=NULL){
            route r;
            get_RT_route_map(sel_RT_lid_tmp,&r);
            printf("route:%s %s %s   event_type:%d\n",get_route_bus_type(r),get_route_bus_lid(r),get_route_RT_lid(r),((evt*)p_event)->event_type);
        }
        else{
            printf("route:null null null   event_type:%d\n",((evt*)p_event)->event_type);
        }
    }
}

void* get_evt_repos(uint_32 pos){
    return (void*)p_repos_array[pos];
}

void* evt_repos_scan_pthread_func(void* argc){
#ifdef PTHREAD_RECYCLE
    pthread_detach(pthread_self());
#endif
    uint_32 i=0;
    uint_32 pos=-1;
    for(;i<REPOS_MAX_LEN;i++){
        if(is_repos_avail[i]==0){//已经被注册
            pos=i;
            break;
        }
    }
    if(pos!=-1){
		evt evt_node;
		void *p_evt_node=(void *)&evt_node;
        while(true){
            void* p_repos=get_evt_repos(i);
            bool is_true=get_one_event(p_repos,p_evt_node);
            if(is_true==true){
                handle_event(p_evt_node);
            }
            sleep_ms(200);
        }
    }
}

void create_evt_repos_scan_unit(void){
    pthread_t tid;
#ifdef __GCC_C99
    pthread_create(&tid,NULL,evt_repos_scan_pthread_func,NULL);
#elif __SPARC_GCC_MMU
    pthread_create(&tid,NULL,(pthread_func_t)evt_repos_scan_pthread_func,NULL);
#endif
    printf("成功创建事件库扫描线程...\n");
}

void throw_event(uint_32 repos_id,byte_8* RT_lid,EVT_TYPE event_type){
    bool is_throw_succeed=false;
    void* p_repos_tmp=get_evt_repos(repos_id);
    if(p_repos_tmp==NULL)return;
    is_throw_succeed=insert_one_event(p_repos_tmp,RT_lid,event_type);
    if(is_throw_succeed==false){
        printf("throw event error\n");
    }
}


