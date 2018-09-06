#include "compile_type.h"
#include "event.h"
#include "relevant_struct_def.h"
#include "event_type.h"
#include<stdlib.h>
#include<string.h>



void* create_evt_repos(void){
    evt_repository* p_evt_repos=(evt_repository*)malloc(sizeof(evt_repository));
    evt* p_list=(evt*)malloc((EVT_LIST_MAX_LEN)*sizeof(evt));
    p_evt_repos->p_evt_list=p_list;
    p_evt_repos->list_size=EVT_LIST_MAX_LEN;
    p_evt_repos->counter=0;
    p_evt_repos->global_enable=ENABLE;
    p_evt_repos->start_pos=0;
    p_evt_repos->end_pos=0;
	pthread_mutex_init(&(p_evt_repos->mutex),NULL);
    return(void*)p_evt_repos;
}

void reset_evt_repos(void* p_evt_repos){
    if(p_evt_repos==NULL)return;
    evt_repository* p_evt_repos_tmp=(evt_repository*)p_evt_repos;
    p_evt_repos_tmp->start_pos=0;
    p_evt_repos_tmp->end_pos=0;
    p_evt_repos_tmp->counter=0;
    p_evt_repos_tmp->global_enable=ENABLE;
}

bool insert_one_event(void* p_evt_repos,byte_8* sel_RT_lid,EVT_TYPE event_type){
	evt_repository* p_evt_repos_tmp=(evt_repository*)p_evt_repos;
    if(p_evt_repos_tmp==NULL){
        return false;
    }
	pthread_mutex_lock(&(p_evt_repos_tmp->mutex));
    if((p_evt_repos_tmp->end_pos+1)%p_evt_repos_tmp->list_size==p_evt_repos_tmp->start_pos){
        return false;
    }
    else{
        uint_32 insert_pos=p_evt_repos_tmp->end_pos;
        evt* p_evt_list_tmp=p_evt_repos_tmp->p_evt_list;
        get_current_time((void*)&((p_evt_list_tmp+insert_pos)->time_stamp));
        (p_evt_list_tmp+insert_pos)->event_type=event_type;
        (p_evt_list_tmp+insert_pos)->sel_RT_lid=sel_RT_lid;
        p_evt_repos_tmp->counter++;
        p_evt_repos_tmp->end_pos=(insert_pos+1)%p_evt_repos_tmp->list_size;
    }
	pthread_mutex_unlock(&(p_evt_repos_tmp->mutex));
    return true;
}

bool get_one_event(void* p_evt_repos,void* p_evt_node){
	evt_repository* p_evt_repos_tmp=(evt_repository*)p_evt_repos;
    if(p_evt_repos_tmp->counter==0)return false;
    evt* p_evt_list_tmp=p_evt_repos_tmp->p_evt_list;
    uint_32 delete_pos=p_evt_repos_tmp->start_pos;
    if(p_evt_repos_tmp==NULL)return false;
    if(p_evt_repos_tmp->start_pos==p_evt_repos_tmp->end_pos)return false;
    else{
        memcpy(p_evt_node,(void*)(p_evt_list_tmp+delete_pos),sizeof(evt));
        p_evt_repos_tmp->counter--;
        p_evt_repos_tmp->start_pos=(p_evt_repos_tmp->start_pos+1)%p_evt_repos_tmp->list_size;
    }
    return true;
}

void set_evt_repos_global_enable(void* p_evt_repos){
    evt_repository* p_evt_repos_tmp=(evt_repository*)p_evt_repos;
    p_evt_repos_tmp->global_enable=ENABLE;
}

void* get_evt_node_time(void* p_evt_node){
    return (void*)(&((evt*)p_evt_node)->time_stamp);
}

uint_32 get_evt_node_event_type(void* p_evt_node){
    return ((evt*)p_evt_node)->event_type;
    
}

byte_8* get_evt_node_sel_RT_lid(void* p_evt_node){
    return ((evt*)p_evt_node)->sel_RT_lid;

}

