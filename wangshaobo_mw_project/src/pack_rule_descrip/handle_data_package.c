/*
 *handle_data_package.c,定义各种对数据包转发规则表的操作函数，使用create_data_rule_form()
 *函数可以创建一个包转发规则表。
 */
#include "handle_data_package.h"
#include "xml_parse.h"
#include "handle_event.h"

//static data_trans_rule_form form[DATA_TRANS_RULE_FORM_MAX_LEN];
static data_trans_rule_form *form=NULL;
//以下相当于字典映射
static byte_8 direct_trans_dev_lid[TRANS_DEV_MAX_SIZE][ATTR_LID_VALUE_MAX_LEN]={{0}};
static byte_8 irdirect_trans_dev_lid[TRANS_DEV_MAX_SIZE][ATTR_LID_VALUE_MAX_LEN]={{0}};
static byte_8 irdirect_trans_bus_type[TRANS_DEV_MAX_SIZE][ATTR_TYPE_VALUE_MAX_LEN]={{0}};
static byte_8 irdirect_trans_bus_lid[TRANS_DEV_MAX_SIZE][ATTR_LID_VALUE_MAX_LEN]={{0}};
static byte_8 irdirect_trans_RT_lid[TRANS_DEV_MAX_SIZE][ATTR_LID_VALUE_MAX_LEN]={{0}};
static uint_32 RT_trans_rule_pos=0;
static uint_32 data_trans_rule_form_num=0;
static uint_32 direct_trans_dev_num=0;
static uint_32 irdirect_trans_dev_num=0;
void create_data_trans_rule_form(void){
    parseXml();
    printf("正在创建包转发规则表...\n");
    uint_32 dev_num=get_device_list_item_num();
    int i=0;
    for(;i<dev_num;i++){                     
        void* dev_p=get_devices_list_item();
        uint_32 data_size=atoi(get_device_data_size(dev_p));
        byte_8* dev_lid=get_device_item_lid(dev_p);
        void* io_p=get_device_io_item(dev_p);
        bool isHaveRT=false;
        while(io_p!=NULL){
            if(strcmp(get_io_type(io_p),"RT")==0){
                void* io_p_tmp=io_p;
                void* io_p_tmp_next=get_io_next_item(io_p_tmp);
                byte_8* RT_lid_tmp=get_io_lid(io_p_tmp);
                byte_8* bus_type_tmp=get_io_type(io_p_tmp_next);
                byte_8* bus_lid_tmp=get_io_lid(io_p_tmp_next);
                strcpy(irdirect_trans_RT_lid[irdirect_trans_dev_num],RT_lid_tmp);
                strcpy(irdirect_trans_bus_lid[irdirect_trans_dev_num],bus_lid_tmp);
                strcpy(irdirect_trans_bus_type[irdirect_trans_dev_num],bus_type_tmp);
                isHaveRT=true;
                int k=0;
                bool isHaveCreate=false;
                for(;k<data_trans_rule_form_num;k++){
                    if(strcmp(form[k].bus_lid,get_io_lid(get_io_next_item(io_p)))==0&&strcmp(form[k].bus_type,get_io_type(get_io_next_item(io_p)))==0){
                        isHaveCreate=true;
                        break;
                    }
                }
                if(isHaveCreate==false)break;
            }
            io_p=get_io_next_item(io_p);
        }
        /*接下来写入此设备的info*/
        if(isHaveRT==false){
            strcpy(direct_trans_dev_lid[direct_trans_dev_num++],dev_lid);//将此设备push到direct trans列表
            continue;
        }
        else if(io_p!=NULL){//form未被创建
            /*需要新建表*/
            if(form==NULL){
                form=(data_trans_rule_form*)malloc(sizeof(data_trans_rule_form));
                memset(form,0,sizeof(data_trans_rule_form));
                if(form==NULL)return;
            }
            else{
                form=(data_trans_rule_form*)realloc(form,sizeof(data_trans_rule_form)\
                        *(data_trans_rule_form_num+1));
                memset(form+sizeof(data_trans_rule_form),0,sizeof(data_trans_rule_form));
            }

            void* next_io_p=get_io_next_item(io_p);
            byte_8* bus_lid=get_io_lid(next_io_p);
            byte_8* bus_type=get_io_type(next_io_p);
            /*写入设备转发info*/
            strcpy(form[data_trans_rule_form_num].bus_lid,bus_lid);
            strcpy(form[data_trans_rule_form_num].bus_type,bus_type);
            form[data_trans_rule_form_num].rule_section_len=0;
            uint_32 rule_len=form[data_trans_rule_form_num].rule_section_len;
            form[data_trans_rule_form_num].rule_section=(RT_trans_rule*)realloc(form[data_trans_rule_form_num].rule_section\
                    ,(rule_len+1)*sizeof(RT_trans_rule));
            memset(form[data_trans_rule_form_num].rule_section+rule_len*sizeof(RT_trans_rule),\
                    0,sizeof(RT_trans_rule));
            form[data_trans_rule_form_num].rule_section[rule_len].info_section_len=0;
            uint_32 info_len=form[data_trans_rule_form_num].rule_section[rule_len].info_section_len;
            form[data_trans_rule_form_num].rule_section[rule_len].info_section=(dev_trans_data_info*)realloc\
                    (form[data_trans_rule_form_num].rule_section[rule_len].info_section,(info_len+1)*sizeof(dev_trans_data_info));
            memset(form[data_trans_rule_form_num].rule_section[rule_len].info_section+info_len*\
                    sizeof(dev_trans_data_info),0,sizeof(dev_trans_data_info));
            strcpy(form[data_trans_rule_form_num].rule_section[rule_len].RT_lid,get_io_lid(io_p));
            byte_8* dev_lid_tmp=get_device_item_lid(dev_p);
            byte_8* dev_map_subAddr_tmp=get_RT_trans_device_map_subAddr(dev_lid_tmp,io_p);
            uint_32 data_size_tmp=atoi(get_device_data_size(dev_p));
            uint_32 send_priority=atoi(get_RT_trans_device_send_priority(dev_lid_tmp,io_p));
            uint_32 receive_priority=atoi(get_RT_trans_device_receive_priority(dev_lid_tmp,io_p));
            uint_32 send_block_size=atoi(get_RT_trans_device_send_block_size(dev_lid_tmp,io_p));
            uint_32 receive_block_size=atoi(get_RT_trans_device_receive_block_size(dev_lid_tmp,io_p));
            strcpy(form[data_trans_rule_form_num].rule_section[rule_len].info_section[info_len].dev_lid,dev_lid_tmp);
            strcpy(form[data_trans_rule_form_num].rule_section[rule_len].info_section[info_len].RT_subAddr,dev_map_subAddr_tmp);
            form[data_trans_rule_form_num].rule_section[rule_len].info_section[info_len].data_size=data_size_tmp;
            form[data_trans_rule_form_num].rule_section[rule_len].info_section[info_len].send_priority=send_priority;
            form[data_trans_rule_form_num].rule_section[rule_len].info_section[info_len].receive_priority=receive_priority;
            form[data_trans_rule_form_num].rule_section[rule_len].info_section[info_len].send_block_size=send_block_size;
            form[data_trans_rule_form_num].rule_section[rule_len].info_section[info_len].receive_block_size=receive_block_size;
            form[data_trans_rule_form_num].rule_section_len++;
            form[data_trans_rule_form_num].rule_section[rule_len].info_section_len++;
            /*找出所有挂载在此总线下的设备*/
            int j=i+1;
            for(;j<dev_num;j++){
                void* dev_p_tmp=get_device_list_item(j);
                void* io_p_tmp=get_device_io_item(dev_p_tmp);
                while(io_p_tmp!=NULL){
                if(strcmp(get_io_type(io_p_tmp),"RT")==0){
                    /*分两种情况，RT_RULE已经创建和未创建*/
                    void* io_p_tmp_next=get_io_next_item(io_p_tmp);
                    if(strcmp(bus_lid,get_io_lid(io_p_tmp_next))==0&&strcmp(bus_type,get_io_type(io_p_tmp_next))==0){
                    int l=0;
                    uint_32 pos_tmp=-1;//标号
                    for(;l<form[data_trans_rule_form_num].rule_section_len;l++){
                        /*当前form的当前RT_rule已经创建，取标号*/
                        if(strcmp(get_io_lid(io_p_tmp),form[data_trans_rule_form_num].rule_section[l].RT_lid)==0){
                            pos_tmp=l;
                            break;
                        }
                    }
                    rule_len=form[data_trans_rule_form_num].rule_section_len;
                    pos_tmp=pos_tmp==-1?rule_len:pos_tmp;
                    if(pos_tmp==rule_len){
                        //创建新的rule_len
                        form[data_trans_rule_form_num].rule_section=(RT_trans_rule*)realloc(\
                                form[data_trans_rule_form_num].rule_section,(rule_len+1)*sizeof(RT_trans_rule));
                        memset(form[data_trans_rule_form_num].rule_section+rule_len*sizeof(RT_trans_rule),\
                                0,sizeof(RT_trans_rule));
                        strcpy(form[data_trans_rule_form_num].rule_section[pos_tmp].RT_lid,get_io_lid(io_p_tmp));
                        form[data_trans_rule_form_num].rule_section[pos_tmp].info_section_len=0;
                        form[data_trans_rule_form_num].rule_section_len++;
                    }
                    info_len=form[data_trans_rule_form_num].rule_section[pos_tmp].info_section_len;
                    form[data_trans_rule_form_num].rule_section[pos_tmp].info_section=(dev_trans_data_info*)realloc\
                        (form[data_trans_rule_form_num].rule_section[pos_tmp].info_section,(info_len+1)*sizeof(dev_trans_data_info));
                    dev_lid_tmp=get_device_item_lid(dev_p_tmp);
                    dev_map_subAddr_tmp=get_RT_trans_device_map_subAddr(dev_lid_tmp,io_p_tmp);
                    int len=get_RT_trans_device_num(io_p_tmp);
                    data_size_tmp=atoi(get_device_data_size(dev_p_tmp));
                    send_priority=atoi(get_RT_trans_device_send_priority(dev_lid_tmp,io_p_tmp));
                    receive_priority=atoi(get_RT_trans_device_receive_priority(dev_lid_tmp,io_p_tmp));
                    send_block_size=atoi(get_RT_trans_device_send_block_size(dev_lid_tmp,io_p_tmp));
                    receive_block_size=atoi(get_RT_trans_device_receive_block_size(dev_lid_tmp,io_p_tmp));
                    strcpy(form[data_trans_rule_form_num].rule_section[pos_tmp].info_section[info_len].dev_lid,dev_lid_tmp);
                    strcpy(form[data_trans_rule_form_num].rule_section[pos_tmp].info_section[info_len].RT_subAddr,dev_map_subAddr_tmp);
                    form[data_trans_rule_form_num].rule_section[pos_tmp].info_section[info_len].data_size=data_size_tmp;
                    form[data_trans_rule_form_num].rule_section[pos_tmp].info_section[info_len].send_priority=send_priority;
                    form[data_trans_rule_form_num].rule_section[pos_tmp].info_section[info_len].receive_priority=receive_priority;
                    form[data_trans_rule_form_num].rule_section[pos_tmp].info_section[info_len].send_block_size=send_block_size;
                    form[data_trans_rule_form_num].rule_section[pos_tmp].info_section[info_len].receive_block_size=receive_block_size;
                    form[data_trans_rule_form_num].rule_section[pos_tmp].info_section_len++;
                    break;
                    }
                }
            io_p_tmp=get_io_next_item(io_p_tmp);
           }
        }
        data_trans_rule_form_num++;
     }
      strcpy(irdirect_trans_dev_lid[irdirect_trans_dev_num++],dev_lid);
   }
    printf("创建包转发规则表结束...\n");
    
    //释放中间结构
    free_device_list();
}

/*
 *打印包转发规则表
 */
void print_form(){
    printf("正在打印包转发规则表...\n");
    uint_32 form_num=get_form_num();
    int i=0;
    printf("form num:%d\n",form_num);
    for(;i<form_num;i++){
        printf("_____________________\n");
        void* form_p=get_forms_item(i);
        printf("bus type:%s\n",get_form_bus_type(form_p));
        printf("bus lid:%s\n\n",get_form_bus_lid(form_p));
        int j=0;
        uint_32 rule_num=get_form_rule_section_len(form_p);
        for(;j<rule_num;j++){
            void* rule_p=get_form_rule_section_item(form_p,j);
            printf("___RT lid___:%s\n\n",get_form_rule_RT_lid(rule_p));
            int k=0;
            uint_32 info_num=get_form_info_section_len(rule_p);
            for(;k<info_num;k++){
                void* info_p=get_form_info_section_item(rule_p,k);
                printf("dev_lid:%s\n",get_form_info_dev_lid(info_p));
                printf("RT_subAddr:%s\n",get_form_info_dev_RT_subAddr(info_p));
                printf("data_size:%d\n",get_form_info_dev_data_size(info_p));
                printf("send_priority:%d\n",get_form_info_dev_send_priority(info_p));
                printf("receive_priority:%d\n",get_form_info_dev_receive_priority(info_p));
                printf("send_block_size:%d\n",get_form_info_dev_send_block_size(info_p));
                printf("receive_block_size:%d\n\n",get_form_info_dev_receive_block_size(info_p));
            }
        }
    }
    printf("打印包转发规则表结束...\n");
}

uint_32 get_form_num(){
    return data_trans_rule_form_num;
}

void* get_forms_item(uint_32 item_pos){
    return (void*)&form[item_pos];
}

byte_8* get_form_bus_lid(void* form_item){
    return ((data_trans_rule_form*)form_item)->bus_lid;
}

byte_8* get_form_bus_type(void* form_item){
    return ((data_trans_rule_form*)form_item)->bus_type;
}

uint_32 get_form_rule_section_len(void* form_item){
    return ((data_trans_rule_form*)form_item)->rule_section_len;
}

void* get_form_rule_section_item(void* form_item,uint_32 item_pos){
    return (void*)&(((data_trans_rule_form*)form_item)->rule_section[item_pos]);
}

byte_8* get_form_rule_RT_lid(void* rule_item){
    return ((RT_trans_rule*)rule_item)->RT_lid;
}

uint_32 get_form_info_section_len(void* rule_item){
    return ((RT_trans_rule*)rule_item)->info_section_len;
}

void* get_form_info_section_item(void* rule_item,uint_32 item_pos){
    return (void*)&(((RT_trans_rule*)rule_item)->info_section[item_pos]);
}

byte_8* get_form_info_dev_lid(void* info_item){
    return ((dev_trans_data_info*)info_item)->dev_lid;
}

uint_32 get_form_info_dev_data_size(void* info_item){
    return ((dev_trans_data_info*)info_item)->data_size;
}

uint_32 get_form_info_dev_send_priority(void* info_item){
    return ((dev_trans_data_info*)info_item)->send_priority;
}

uint_32 get_form_info_dev_receive_priority(void* info_item){
    return ((dev_trans_data_info*)info_item)->receive_priority;
}

uint_32 get_form_info_dev_send_block_size(void* info_item){
    return ((dev_trans_data_info*)info_item)->send_block_size;
}

uint_32 get_form_info_dev_receive_block_size(void* info_item){
    return ((dev_trans_data_info*)info_item)->receive_block_size;
}

byte_8* get_form_info_dev_RT_subAddr(void* info_item){
    if(info_item==NULL)return "";
    return ((dev_trans_data_info*)info_item)->RT_subAddr;
}

void* get_form_rule_item(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid){
    int i=0;
    void* p=NULL;
    for(;i<get_form_num();i++){
        void* f_p=get_forms_item(i);
        if(strcmp(get_form_bus_type(f_p),bus_type)==0&&strcmp(get_form_bus_lid(f_p),bus_lid)==0){
            int j=0;
            uint_32 rule_num=get_form_rule_section_len(f_p);
            for(;j<rule_num;j++){
                void* rule_p=get_form_rule_section_item(f_p,j);
                if(strcmp(get_form_rule_RT_lid(rule_p),RT_lid)==0){
                    p=rule_p;
                    break;
                }
            }
            if(p!=NULL)break;
        }
    }
    return p;
}

void* get_form_item(byte_8* bus_type,byte_8*bus_lid){
    int i=0;
    void* p=NULL;
    for(;i<get_form_num();i++){
        void* f_p=get_forms_item(i);
        if(strcmp(get_form_bus_type(f_p),bus_type)==0&&strcmp(get_form_bus_lid(f_p),bus_lid)==0){
            p=f_p;
            break;
        }
    }
    return p;
}

/*byte_8* get_max_priority_dev_lid(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,uint_32 attr_flag,uint_32* current_priority,uint_32* anchor){
    int k=0;
    byte_8* dev_lid="";
    int max_priority=-100;
    int priority_t=0;
    void* rule_p=get_form_rule_item(bus_type,bus_lid,RT_lid);
    if(rule_p==NULL)return dev_lid;
    uint_32 info_num=get_form_info_section_len(rule_p);
    for(;k<info_num;k++){
        void* info_p=get_form_info_section_item(rule_p,k);
        if(attr_flag==SEND_PRIORITY_FLAG)priority_t=get_form_info_dev_send_priority(info_p);
        else if(attr_flag==RECEIVE_PRIORITY_FLAG)priority_t=get_form_info_dev_receive_priority(info_p);
        if(priority_t==0)continue;
        if(max_priority==priority_t)continue;
        uint_32 max_priority_tmp=max_priority;
        max_priority=max_priority<priority_t?priority_t:max_priority;
        if(max_priority==priority_t){
            if(max_priority_tmp!=max_priority)*anchor=k;//max_priority改变了则下锚
            dev_lid=get_form_info_dev_lid(info_p);
            if(attr_flag==SEND_PRIORITY_FLAG)*current_priority=get_form_info_dev_send_priority(info_p);
            if(attr_flag==RECEIVE_PRIORITY_FLAG)*current_priority=get_form_info_dev_receive_priority(info_p);
        }
    }
    return dev_lid;
}*/

byte_8* get_priority_deterio_dev_lid(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,uint_32 attr_flag,uint_32 prev_priority,uint_32* current_priority,uint_32* anchor){
    void* rule_p=get_form_rule_item(bus_type,bus_lid,RT_lid);
    uint_32 info_num=get_form_info_section_len(rule_p);
    int k=*anchor==0?0:(*anchor+1);
    byte_8* dev_lid="";
    uint_32 max_priority=prev_priority;
    uint_32 priority_t=0;
    for(;k<info_num;k++){
        void* info_p=get_form_info_section_item(rule_p,k);
        if(attr_flag==SEND_PRIORITY_FLAG)priority_t=get_form_info_dev_send_priority(info_p);
        else if(attr_flag==RECEIVE_PRIORITY_FLAG)priority_t=get_form_info_dev_receive_priority(info_p);
        if(*anchor!=0){//找出相等的
            if(max_priority==priority_t){
                dev_lid=get_form_info_dev_lid(info_p);
                if(attr_flag==SEND_PRIORITY_FLAG)*current_priority=get_form_info_dev_send_priority(info_p);
                if(attr_flag==RECEIVE_PRIORITY_FLAG)*current_priority=get_form_info_dev_receive_priority(info_p);
                *anchor=k;
                break;
            }
        }
    }
    if(strcmp(dev_lid,"")!=0){
        return dev_lid;
    }//没有找到相等的，找比prev_priority小一级的
    k=0;
    uint_32 min_deviation=MIN_DEVIATION;
    for(;k<info_num;k++){
        void* info_p=get_form_info_section_item(rule_p,k);
        if(attr_flag==SEND_PRIORITY_FLAG)priority_t=get_form_info_dev_send_priority(info_p);
        else if(attr_flag==RECEIVE_PRIORITY_FLAG)priority_t=get_form_info_dev_receive_priority(info_p);
        if(priority_t>=max_priority)continue;
        min_deviation=(max_priority-priority_t)<min_deviation?(max_priority-priority_t):min_deviation;
        if(min_deviation==(max_priority-priority_t)){//
            dev_lid=get_form_info_dev_lid(info_p);
            if(attr_flag==SEND_PRIORITY_FLAG)*current_priority=get_form_info_dev_send_priority(info_p);
            if(attr_flag==RECEIVE_PRIORITY_FLAG)*current_priority=get_form_info_dev_receive_priority(info_p);
            *anchor=k;
        }
    }
    if(k==info_num)*anchor=0;
    return dev_lid;
}

uint_32 get_dev_trans_attr(byte_8* bus_type,byte_8* bus_lid,byte_8*RT_lid,byte_8* dev_lid,uint_32 attr_flag){
    void* rule_p=get_form_rule_item(bus_type,bus_lid,RT_lid);
    if(rule_p==NULL)return 0;
    uint_32 info_num=get_form_info_section_len(rule_p);
    int k=0;
    uint_32 attr_value;
    for(;k<info_num;k++){
        void* info_p=get_form_info_section_item(rule_p,k);
        if(strcmp(get_form_info_dev_lid(info_p),dev_lid)==0){
            if(attr_flag==SEND_BLOCK_FLAG)attr_value=get_form_info_dev_send_block_size(info_p);
            if(attr_flag==RECEIVE_BLOCK_FLAG)attr_value=get_form_info_dev_receive_block_size(info_p);
        }
    }
    return attr_value;
}

uint_32 get_dev_data_size(byte_8* bus_type,byte_8* bus_lid,byte_8*RT_lid,byte_8* dev_lid){
    void* rule_p=get_form_rule_item(bus_type,bus_lid,RT_lid);
    uint_32 info_num=get_form_info_section_len(rule_p);
    int k=0;
    for(;k<info_num;k++){
        void* info_p=get_form_info_section_item(rule_p,k);
        if(strcmp(get_form_info_dev_lid(info_p),dev_lid)==0){
            return get_form_info_dev_data_size(info_p);
        }
    }
}

byte_8* get_dev_map_RT_subAddr(byte_8* bus_type,byte_8* bus_lid,byte_8*RT_lid,byte_8* dev_lid){
    if(strcmp(RT_lid,"")==0){
        void* form_p=get_form_item(bus_type,bus_lid);
        uint_32 form_rule_len=get_form_info_section_len(form_p);
        if(form_rule_len>0){
            void* rule_p=get_form_rule_section_item(form_p,0);
            uint_32 info_len=get_form_info_section_len(rule_p);
            if(info_len>0){
                void* info_p=get_form_info_section_item(rule_p,0);
                return get_form_info_dev_RT_subAddr(info_p);
            }
        }
    }
    void* rule_p=get_form_rule_item(bus_type,bus_lid,RT_lid);
    uint_32 info_num=get_form_info_section_len(rule_p);
    int k=0;
    byte_8* sub_addr_tmp="0:0";
    if(strcmp(dev_lid,"")==0){
        if(info_num>0){
            void* info_p=get_form_info_section_item(rule_p,0);
            return get_form_info_dev_RT_subAddr(info_p);
        }
    }
    for(;k<info_num;k++){
        void* info_p=get_form_info_section_item(rule_p,k);
        if(strcmp(get_form_info_dev_lid(info_p),dev_lid)==0){
            sub_addr_tmp=get_form_info_dev_RT_subAddr(info_p);
        }
    }
    return sub_addr_tmp;
}

uint_32 find_irdirect_dev_pos(byte_8* dev_lid){
    uint_32 i=0;
    for(;i<irdirect_trans_dev_num;i++){
        if(strcmp(irdirect_trans_dev_lid[i],dev_lid)==0)
            return i;
    }
}

uint_32 find_irdirect_RT_pos(byte_8* RT_lid){
    uint_32 i=0;
    for(;i<irdirect_trans_dev_num;i++){
        if(strcmp(irdirect_trans_RT_lid[i],RT_lid)==0)
            return i;
    }
}

uint_32 get_1553_device_num(byte_8* bus_type,byte_8* bus_lid){
    byte_8 addr_buffer[RT_SUBADDR_VALUE_MAX_LEN]={0};
    byte_8 device_num_string[RT_SUBADDR_VALUE_MAX_LEN]={0};
    strcpy(addr_buffer,get_dev_map_RT_subAddr(bus_type,bus_lid,"",""));
    uint_32 i=0;
    for(;i<RT_SUBADDR_VALUE_MAX_LEN;i++){
        if(addr_buffer[i]==':'){
            memcpy(device_num_string,addr_buffer,i);
            break;
        }
    }
    return atoi(device_num_string);

}

uint_32 get_RT_physical_addr(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid){
    byte_8 addr_buffer[RT_SUBADDR_VALUE_MAX_LEN]={0};
    byte_8 physical_addr[RT_SUBADDR_VALUE_MAX_LEN]={0};
    strcpy(addr_buffer,get_dev_map_RT_subAddr(bus_type,bus_lid,RT_lid,""));
    int i=0;
    uint_32 fir_pos=-1;
    for(;i<RT_SUBADDR_VALUE_MAX_LEN;i++){
        if(addr_buffer[i]==':'){
            int j=i+1;
            fir_pos=i+1;
            uint_32 sec_pos=-1;
            for(;j<RT_SUBADDR_VALUE_MAX_LEN;j++){
                if(addr_buffer[j]==':'){
                    sec_pos=j-1;
                    break;
                }
            }
            if(sec_pos==-1)return -1;
            memcpy(physical_addr,addr_buffer+fir_pos,sec_pos-fir_pos+1);
            break;
        }
    }
    return atoi(physical_addr);
}

uint_32 get_RT_physical_sub_addr(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8* dev_lid){
    byte_8 addr_buffer[RT_SUBADDR_VALUE_MAX_LEN]={0};
    byte_8 physical_sub_addr[RT_SUBADDR_VALUE_MAX_LEN]={0};
    strcpy(addr_buffer,get_dev_map_RT_subAddr(bus_type,bus_lid,RT_lid,dev_lid));
    int i=0;
    for(;i<RT_SUBADDR_VALUE_MAX_LEN;i++){
        if(addr_buffer[i]==':'){
            int j=i+1;
            for(;j<RT_SUBADDR_VALUE_MAX_LEN;j++){
                if(addr_buffer[j]==':')break;
            }
            memcpy(physical_sub_addr,addr_buffer+j+1,RT_SUBADDR_VALUE_MAX_LEN-j-1);
            break;
        }
    }
    return atoi(physical_sub_addr);
}

byte_8* get_irdirect_dev_lid(uint_32 irdirect_pos){
    return irdirect_trans_dev_lid[irdirect_pos];
}

byte_8* get_irdirect_RT_lid(uint_32 irdirect_pos){
    return irdirect_trans_RT_lid[irdirect_pos];

}

byte_8* get_irdirect_bus_type(uint_32 irdirect_pos){
    return irdirect_trans_bus_type[irdirect_pos];
    
}

byte_8* get_irdirect_bus_lid(uint_32 irdirect_pos){
    return irdirect_trans_bus_lid[irdirect_pos];

}


