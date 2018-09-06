#include "config_1553.h"
#include "handle_data_package.h"
#include "control_traffic_light.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define CAN1553 1  //支持1553和can

static config_node* p_config_node_list;
static uint_32 config_len;
void config_1553(){
    printf("注册1553...\n");
    uint_32 num_1553=get_form_num();
    config_node* p_node=(config_node*)malloc(sizeof(config_node)*num_1553);
    p_config_node_list=p_node;
    int i=0;
    for(;i<num_1553;i++){
        void* p_form_item= get_forms_item(i);
        byte_8* bus_type_tmp=get_form_bus_type(p_form_item);
#ifdef CAN1553
        if(strcmp(bus_type_tmp,"1553b")!=0&&strcmp(bus_type_tmp,"can")!=0)continue;
#else
        if(strcmp(bus_type_tmp,"1553b")!=0)continue;
#endif
        byte_8* bus_lid_tmp=get_form_bus_lid(p_form_item);
        uint_32 device_num_1553_tmp=get_1553_device_num(bus_type_tmp,bus_lid_tmp);
        uint_32 repos_pos_tmp=config_traffic_repos(bus_type_tmp,bus_lid_tmp);
        (p_node+i)->bus_lid=bus_lid_tmp;
        (p_node+i)->device_num_1553=device_num_1553_tmp;
        (p_node+i)->traffic_repos_id=repos_pos_tmp;
        (p_node+i)->len=0;
        uint_32 RT_num=get_form_rule_section_len(p_form_item);
        int j=0;
        for(;j<RT_num;j++){
            void* p_RT_item=get_form_rule_section_item(p_form_item,j);
            byte_8* RT_lid_tmp=get_form_rule_RT_lid(p_RT_item);
            uint_32 port_tmp=get_RT_physical_addr(bus_type_tmp,bus_lid_tmp,RT_lid_tmp);
            uint_32 light_pos_tmp=get_light_pos(repos_pos_tmp,RT_lid_tmp);
            strcpy((p_node+i)->RT_lid_array[j],RT_lid_tmp);
            (p_node+i)->port_array[j]=port_tmp;
            (p_node+i)->light_pos_array[j]=light_pos_tmp;
            (p_node+i)->len++;
        }
    }
    config_len=num_1553;
    printf("完成注册...\n");
}

uint_32 get_config_len(){
    return config_len;
}

void* get_config_list_entry(void){
    return (void*)p_config_node_list;
}

uint_32 get_1553_bus_config_id(byte_8* bus_lid){
    void* entry=get_config_list_entry();
    uint_32 config_len_tmp=get_config_len();
    int i=0;
    uint_32 pos_tmp=CONFIG_OVERFLOW;
    for(;i<config_len_tmp;i++){
        byte_8* bus_lid_tmp=((config_node*)(entry+i))->bus_lid;
        if(strcmp(bus_lid_tmp,bus_lid)==0){
            pos_tmp=i;
            break;
        }
    }
    return pos_tmp;
}

uint_32 get_RT_config_id(void* p_config_node,byte_8* RT_lid){
    if(p_config_node==NULL)return CONFIG_OVERFLOW;
    config_node* p_tmp=(config_node*)p_config_node;
    uint_32 len=get_config_node_len(p_tmp);
    int i=0;
    for(;i<len;i++){
        if(strcmp(p_tmp->RT_lid_array[i],RT_lid)==0){
            return i;
        }
    }
    return CONFIG_OVERFLOW;
}

uint_32 get_1553_bus_config_id_t(uint_32 traffic_repos_id){
    void* entry=get_config_list_entry();
    uint_32 config_len_tmp=get_config_len();
    int i=0;
    uint_32 pos_tmp=CONFIG_OVERFLOW;
    for(;i<config_len_tmp;i++){
        uint_32 traffic_repos_id_tmp=((config_node*)(entry+i))->traffic_repos_id;
        if(traffic_repos_id_tmp==traffic_repos_id){
            pos_tmp=i;
            break;
        }
    }
    return pos_tmp;
    
}

uint_32 get_device_num_1553_bus_config_id(uint_32 device_num_1553){
    void* entry=get_config_list_entry();
    uint_32 config_len_tmp=get_config_len();
    int i=0;
    uint_32 pos_tmp=CONFIG_OVERFLOW;
    for(;i<config_len_tmp;i++){
        uint_32 device_num_1553_tmp=((config_node*)(entry+i))->device_num_1553;
        if(device_num_1553_tmp==device_num_1553){
            pos_tmp=i;
            break;
        }
    }
    return pos_tmp;
    
}

void* get_config_node(uint_32 pos){
    if(pos>=get_form_num())return NULL;
    void* p=get_config_list_entry();
    return (void*)((config_node*)p+pos);
}

byte_8* get_config_node_bus_lid(void* p_config_node){
    return ((config_node*)p_config_node)->bus_lid;
}

uint_32  get_config_node_traffic_repos_id(void* p_config_node){
    
    return ((config_node*)p_config_node)->traffic_repos_id;
}

uint_32  get_config_node_len(void* p_config_node){
    
    return ((config_node*)p_config_node)->len;
}

uint_32  get_config_node_port(void* p_config_node,uint_32 pos){
    if(pos==CONFIG_OVERFLOW)return CONFIG_OVERFLOW;
    
    return ((config_node*)p_config_node)->port_array[pos];
}

byte_8* get_config_node_RT_lid(void* p_config_node,uint_32 pos){
    byte_8* RT_lid_tmp="";
    if(pos==CONFIG_OVERFLOW)return RT_lid_tmp;
    
    return ((config_node*)p_config_node)->RT_lid_array[pos];
}

uint_32  get_config_node_light_pos(void* p_config_node,uint_32 pos){
    if(pos>=CONFIG_OVERFLOW)return -1;
    return ((config_node*)p_config_node)->light_pos_array[pos];
}

uint_32 get_config_light_pos(uint_32 config_id,uint_32 port){
    void* p=get_config_node(config_id);
    uint_32 pos_tmp=-1;
    uint_32 len_tmp=get_config_node_len(p);
    int i=0;
    for(;i<len_tmp;i++){
        if(get_config_node_port(p,i)==port){
            pos_tmp=i;
            break;
        }
    }
    if(pos_tmp==-1)return -1;
    return get_config_node_light_pos(p,pos_tmp);
}
