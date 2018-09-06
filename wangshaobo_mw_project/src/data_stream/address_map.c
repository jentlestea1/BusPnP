/*
 *address_map.c,定义操作索引表的各种函数。
 */
#include "address_map.h"
#include "xml_parse.h"
#include "handle_data_package.h"
#include "relevant_struct_def.h"
#include "handle_event.h"

#define TRANS_SPACE_MAX_SIZE 0x200  //每个缓存区大小为0.5k*1dataNode

static f_index_list index_list_f;
static s_index_list *index_list_s;
static uint_32 index_list_s_len=0;
static t_index_list *index_list_t;
static uint_32 index_list_t_len=0;



void create_index_list(void){
    /*
     * 为每一个设备定制一个数据/指令转发存储区，以RT为
     * 单位，分配缓存区，每个设备占用其中一块区域，用收
     * 发地址指针管理初始化收发地址指针为两端，向中间靠
     * 拢。
     */
    int t_index_list_num_tmp=0;
    printf("正在创建三级索引表结构...\n");
    uint_32 form_num=get_form_num();
    int i=0;
    index_list_f.f_node_len=0;
    index_list_f.f_node=(f_index_node*)malloc(form_num*sizeof(f_index_node));
    index_list_s=(s_index_list*)malloc(sizeof(s_index_list)*form_num);
    for(;i<form_num;i++){
        void* form_item=get_forms_item(i);
        byte_8* bus_lid=get_form_bus_lid(form_item);
        byte_8* bus_type=get_form_bus_type(form_item);
        uint_32 f_node_len=index_list_f.f_node_len;
        index_list_f.f_node_len++;
        strcpy(index_list_f.f_node[f_node_len].bus_type,bus_type);
        strcpy(index_list_f.f_node[f_node_len].bus_lid,bus_lid);
        index_list_f.f_node[f_node_len].s_list_p=&index_list_s[index_list_s_len];
        s_index_list* s_p_tmp=index_list_f.f_node[f_node_len].s_list_p;
        uint_32 RT_num=get_form_rule_section_len(form_item);
        s_p_tmp->s_node_len=0;
        int j=0;
        index_list_s[index_list_s_len].s_node=(s_index_node*)malloc(sizeof(s_index_node)*RT_num);
        index_list_t=(t_index_list*)realloc(index_list_t,(t_index_list_num_tmp+RT_num)*sizeof(t_index_list));
        t_index_list_num_tmp+=RT_num;
        index_list_s_len++;

        for(;j<RT_num;j++){
            void* RT_item=get_form_rule_section_item(form_item,j);
            byte_8* RT_lid=get_form_rule_RT_lid(RT_item);
            strcpy(s_p_tmp->s_node[j].RT_lid,RT_lid);
            uint_32 s_node_len=s_p_tmp->s_node_len;
            s_p_tmp->s_node_len++;
            s_p_tmp->s_node[s_node_len].t_list_p=&index_list_t[index_list_t_len];
            t_index_list* t_p_tmp=s_p_tmp->s_node[s_node_len].t_list_p;
            uint_32 info_num=get_form_info_section_len(RT_item);
            void* p_s=(void*)malloc(sizeof(dataNode)*TRANS_SPACE_MAX_SIZE*info_num);
            if(p_s==NULL){
                   printf("malloc error\n");
                   return;
            }
            t_p_tmp->t_node_len=0;
            int k=0;
            index_list_t[index_list_t_len].t_node=(t_index_node*)malloc(sizeof(t_index_node)*info_num);
            index_list_t_len++;
            for(;k<info_num;k++){
               void* info_item=get_form_info_section_item(RT_item,k);
               byte_8* dev_lid=get_form_info_dev_lid(info_item);
               strcpy(t_p_tmp->t_node[k].dev_lid,dev_lid);
               uint_32 t_node_len=t_p_tmp->t_node_len;
               t_p_tmp->t_node_len++;
               t_p_tmp->t_node[k].addr_read_s=t_p_tmp->t_node[k].addr_read_e=t_p_tmp->t_node[k].addr_work_r=(dataNode*)p_s+k*TRANS_SPACE_MAX_SIZE;
               t_p_tmp->t_node[k].addr_write_s=t_p_tmp->t_node[k].addr_write_e=t_p_tmp->t_node[k].addr_work_w=(dataNode*)p_s+((k+1)*(TRANS_SPACE_MAX_SIZE)-1);
            }
        }
    }
    printf("创建三级索引表结构结束...\n");
}

bool is_read_region_empty(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8* dev_lid){
    void* p=get_t_index_node(bus_type,bus_lid,RT_lid,dev_lid);
    void* e_p=get_read_addr_e(p);
    void* w_p=get_read_addr_work_p(p);
    if(e_p==w_p)return true;
    else return false;
}

bool is_write_region_empty(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8* dev_lid){
    void* p=get_t_index_node(bus_type,bus_lid,RT_lid,dev_lid);
    void* e_p=get_write_addr_e(p);
    void* w_p=get_write_addr_work_p(p);
    if(e_p==w_p)return true;
    else return false;
}

uint_32 get_write_region_size(byte_8* bus_type,byte_8*bus_lid,byte_8* RT_lid,byte_8* dev_lid){
    void* p=get_t_index_node(bus_type,bus_lid,RT_lid,dev_lid);
    void* s_p=get_write_addr_s(p);
    void* e_p=get_write_addr_e(p);
    void* w_p=get_write_addr_work_p(p);
    uint_32 size_tmp=0;
    while(w_p!=e_p){
        size_tmp++;
        w_p=(void*)(((((dataNode*)w_p-1)-(dataNode*)s_p)%WRITE_REGION_MAX_SIZE)+(dataNode*)s_p);
    }
    return size_tmp;
}

uint_32 get_read_region_size(byte_8* bus_type,byte_8*bus_lid,byte_8* RT_lid,byte_8* dev_lid){
    void* p=get_t_index_node(bus_type,bus_lid,RT_lid,dev_lid);
    void* s_p=get_read_addr_s(p);
    void* e_p=get_read_addr_e(p);
    void* w_p=get_read_addr_work_p(p);
    uint_32 size_tmp=0;
    while(w_p!=e_p){
        size_tmp++;
        w_p=(void*)(((((dataNode*)w_p+1)-(dataNode*)s_p)%WRITE_REGION_MAX_SIZE)+(dataNode*)s_p);
    }
    return size_tmp;
}

void dev_read_data(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8* dev_lid,void* buffer,uint_32 readSize,uint_32* size){
    *size=0;
    void* p=get_t_index_node(bus_type,bus_lid,RT_lid,dev_lid);
    void* s_p=get_read_addr_s(p);
    void* e_p=get_read_addr_e(p);
    void* w_p=get_read_addr_work_p(p);
    if(readSize<=0)return;
    while(readSize--){
        if(w_p==e_p)break;
        const void* w_p_tmp=w_p;
        memcpy((dataNode*)buffer+*size,w_p_tmp,sizeof(dataNode));
        w_p=(void*)(((((dataNode*)w_p+1)-(dataNode*)s_p)%READ_REGION_MAX_SIZE)+(dataNode*)s_p);
        (*size)++;
    }
    set_index_node_pointer(p,true,e_p,s_p,w_p);
}

void dev_write_data(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8*dev_lid,void* dataBuffer,uint_32 writeSize,uint_32* size){ 
    *size=0;
    void* p=get_t_index_node(bus_type,bus_lid,RT_lid,dev_lid);
    void* s_p=get_write_addr_s(p);
    void* e_p=get_write_addr_e(p);
    void* w_p=get_write_addr_work_p(p);
    if(writeSize<=0)return;
    while(writeSize--){
        void* next=(void*)(((((dataNode*)e_p-1)-(dataNode*)s_p)%WRITE_REGION_MAX_SIZE)+(dataNode*)s_p);
        if(next==w_p)break;
        void* data_p_tmp=(dataNode*)dataBuffer+*size;
        memcpy(e_p,data_p_tmp,sizeof(dataNode));
        e_p=next;
        (*size)++;
    }
    set_index_node_pointer(p,false,e_p,s_p,w_p);
}

void app_read_data(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8* dev_lid,void* buffer,uint_32 readSize,uint_32* size){
    *size=0;
    void* p=get_t_index_node(bus_type,bus_lid,RT_lid,dev_lid);
    void* s_p=get_write_addr_s(p);
    void* e_p=get_write_addr_e(p);
    void* w_p=get_write_addr_work_p(p);
    if(readSize<=0)return;
    while(readSize--){
        if(w_p==e_p)break;
        const void* w_p_tmp=w_p;
        memcpy((dataNode*)buffer+*size,w_p_tmp,sizeof(dataNode));
        w_p=(void*)(((((dataNode*)w_p-1)-(dataNode*)s_p)%WRITE_REGION_MAX_SIZE)+(dataNode*)s_p);
        (*size)++;
    }
    set_index_node_pointer(p,false,e_p,s_p,w_p);
    
}

void app_write_data(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8*dev_lid,void* buffer,uint_32 writeSize,uint_32* size){
    *size=0;
    void* p=get_t_index_node(bus_type,bus_lid,RT_lid,dev_lid);
    void* s_p=get_read_addr_s(p);
    void* e_p=get_read_addr_e(p);
    void* w_p=get_read_addr_work_p(p);
    if(writeSize<=0)return;
    while(writeSize--){
        void* next=(void*)(((((dataNode*)e_p+1)-(dataNode*)s_p)%READ_REGION_MAX_SIZE)+(dataNode*)s_p);
        if(next==w_p){
            break;//抛出异常
            throw_event(0,RT_lid,EVT_APP_WRITE_ERR);
        }
        const void* data_p_tmp=(dataNode*)buffer+*size;
        memcpy(e_p,data_p_tmp,sizeof(dataNode));
        e_p=next;
        (*size)++;
    }
    set_index_node_pointer(p,true,e_p,s_p,w_p);
}

void* get_t_index_node(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8* dev_lid){
    uint_32 f_node_len=index_list_f.f_node_len;
    void* p=NULL;
    int i=0;
    for(;i<f_node_len;i++){
        if(strcmp(index_list_f.f_node[i].bus_type,bus_type)==0&&strcmp(index_list_f.f_node[i].bus_lid,bus_lid)==0){
            s_index_list* s_list_p_tmp=index_list_f.f_node[i].s_list_p;
            uint_32 s_node_len=s_list_p_tmp->s_node_len;
            int j=0;
            for(;j<s_node_len;j++){
                if(strcmp(s_list_p_tmp->s_node[j].RT_lid,RT_lid)==0){
                    t_index_list* t_list_p_tmp=s_list_p_tmp->s_node[j].t_list_p;
                    uint_32 t_node_len=t_list_p_tmp->t_node_len;
                    int k=0;
                    for(;k<t_node_len;k++){
                        if(strcmp(t_list_p_tmp->t_node[k].dev_lid,dev_lid)==0){
                            p=(void*)&(t_list_p_tmp->t_node[k]); 
                        }
                    }

                }
            }
        }
    }
    return p;
}

void* get_read_addr_s(void* p_t_index_node){
    void* p=NULL;
    if(p_t_index_node==NULL)return p;
    p=(void*)((t_index_node*)p_t_index_node)->addr_read_s; 
    return p;
}

void* get_read_addr_e(void* p_t_index_node){
    void* p=NULL;
    if(p_t_index_node==NULL)return p;
    p=(void*)((t_index_node*)p_t_index_node)->addr_read_e; 
    return p;
}

void* get_read_addr_work_p(void* p_t_index_node){
    void* p=NULL;
    if(p_t_index_node==NULL)return p;
    p=(void*)((t_index_node*)p_t_index_node)->addr_work_r; 
    return p;
}

void* get_write_addr_s(void* p_t_index_node){
    void* p=NULL;
    if(p_t_index_node==NULL)return p;
    p=(void*)((t_index_node*)p_t_index_node)->addr_write_s; 
    return p;
    
}

void* get_write_addr_e(void* p_t_index_node){
    void* p=NULL;
    if(p_t_index_node==NULL)return p;
    p=(void*)((t_index_node*)p_t_index_node)->addr_write_e; 
    return p;
    
}

void* get_write_addr_work_p(void* p_t_index_node){
    void* p=NULL;
    if(p_t_index_node==NULL)return p;
    p=(void*)((t_index_node*)p_t_index_node)->addr_work_w; 
    return p;
    
}

void set_index_node_pointer(void* p_t_index_node,bool isRead,void* e_p,void* s_p,void* w_p){
    if(isRead==true){
        ((t_index_node*)p_t_index_node)->addr_read_e=(dataNode*)e_p;
        ((t_index_node*)p_t_index_node)->addr_read_s=(dataNode*)s_p;
        ((t_index_node*)p_t_index_node)->addr_work_r=(dataNode*)w_p;
    }
    else{
        ((t_index_node*)p_t_index_node)->addr_write_e=(dataNode*)e_p;
        ((t_index_node*)p_t_index_node)->addr_write_s=(dataNode*)s_p;
        ((t_index_node*)p_t_index_node)->addr_work_w=(dataNode*)w_p;
    }
}

void* get_t_index_list(byte_8* bus_type,byte_8* bus_lid,uint_32 pos){
    uint_32 f_node_len=index_list_f.f_node_len;
    void* p=NULL;
    int i=0;
    for(;i<f_node_len;i++){
        if(strcmp(index_list_f.f_node[i].bus_type,bus_type)==0&&strcmp(index_list_f.f_node[i].bus_lid,bus_lid)==0){
            s_index_list* s_list_p_tmp=index_list_f.f_node[i].s_list_p;
			if(pos>=s_list_p_tmp->s_node_len)
				return NULL;
            p=(void*)s_list_p_tmp->s_node[pos].t_list_p;
        }
    }
    return p;
}

void* get_s_index_list(byte_8* bus_type,byte_8* bus_lid){
    uint_32 f_node_len=index_list_f.f_node_len;
    void* p=NULL;
    int i=0;
    for(;i<f_node_len;i++){
        if(strcmp(index_list_f.f_node[i].bus_type,bus_type)==0&&strcmp(index_list_f.f_node[i].bus_lid,bus_lid)==0){
            s_index_list* s_list_p_tmp=index_list_f.f_node[i].s_list_p;
            p=(void*)s_list_p_tmp;
        }
    }
    return p;
}

uint_32 get_s_index_list_len(byte_8* bus_type,byte_8* bus_lid){
    uint_32 f_node_len=index_list_f.f_node_len;
    uint_32 s_node_len=0;
    int i=0;
    for(;i<f_node_len;i++){
        if(strcmp(index_list_f.f_node[i].bus_type,bus_type)==0&&strcmp(index_list_f.f_node[i].bus_lid,bus_lid)==0){
            s_index_list* s_list_p_tmp=index_list_f.f_node[i].s_list_p;
            s_node_len=s_list_p_tmp->s_node_len;
        }
    }
    return s_node_len;
}

uint_32 get_t_index_list_len(void* p_t_index_list){
    return ((t_index_list*)p_t_index_list)->t_node_len;
}

void get_t_index_node_lid(void* p_t_index_list,uint_32 pos,byte_8** p_dev_lid){
    t_index_list* p_t_index_list_tmp=(t_index_list*)p_t_index_list;
    if(pos>p_t_index_list_tmp->t_node_len-1){
        *p_dev_lid="";
        return;
    }
    *p_dev_lid=p_t_index_list_tmp->t_node[pos].dev_lid;
}

void get_s_index_node_lid(void* p_s_index_list,uint_32 pos,byte_8** p_RT_lid){
    s_index_list* p_s_index_list_tmp=(s_index_list*)p_s_index_list;
    if(pos>p_s_index_list_tmp->s_node_len-1){
        *p_RT_lid="";
        return;
    }
    *p_RT_lid=p_s_index_list_tmp->s_node[pos].RT_lid;
    
}

