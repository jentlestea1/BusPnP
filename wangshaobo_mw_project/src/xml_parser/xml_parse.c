/*
 * xml_parse.c,定义各种对配置文件解析存储的操作函数,只需要使用parseXml()就可以完成配置该存储结构。
 */
#include "compile_type.h"
#include "xml_parse.h"
#include<stdio.h>
#include<stdlib.h>
#include "relevant_struct_def.h"
#ifdef __SPARC_GCC_MMU
#include "dev_string_xml.h"
#endif
#define ROUTE_MAX_LEN 5
#define ROUTE_VALUE_MAX_LEN 10
#define CONTENT_MAX_LEN 30
#define ROUTE_IO_NAME_POS 0
#define ROUTE_IO_LID_VALUE 1
#define ROUTE_CHANNEL_NAME_POS 2
#define ROUTE_CHANNEL_LID_VALUE_POS 3
#define DESCRIPTION_ITEM_MAX_LEN 20
#define TAG_NAME_MAX_LEN 30

static DeviceNode* DeviceList[DEVICE_LIST_MAX_LEN]={NULL};
static uint_32 DeviceListLen=0;         
static uint_32 DeviceListPos=0;         
static uint_32 DescripEntryPos=0;
static uint_32 dev_attr_name_pos=0;
static uint_32 dev_attr_value_pos=0;
static uint_32 io_attr_name_pos=0;
static uint_32 io_attr_value_pos=0;
static uint_32 RT_trans_device_lid_pos=0;
static uint_32 RT_trans_device_s_p_pos=0;
static uint_32 RT_trans_device_s_b_pos=0;
static uint_32 RT_trans_device_r_p_pos=0;
static uint_32 RT_trans_device_r_b_pos=0;
static byte_8 interface[INTERFACE_MAX_LEN]={0};

mxml_node_t* root=NULL;

/** 
 * 解析XML配置文件，生成配置信息中间存储结构
 * @ref            m_type.h 
 */ 
void parseXml(){
    printf("正在生成配置文件存储结构...\n");

#ifdef __GCC_C99
    FILE* fp=fopen("dev_new.xml","r");
    root=mxmlLoadFile(NULL,fp,MXML_NO_CALLBACK);
#elif __SPARC_GCC_MMU
    root=mxmlLoadString(NULL,xml_string,MXML_NO_CALLBACK);
#endif

    byte_8* device_tag="device_entity";
    mxml_node_t* dev_node_p=NULL;
    mxml_node_t* dev_attr_node_p=NULL;
    mxml_node_t* io_node_p=NULL;
    mxml_node_t* io_attr_node_p=NULL;
    mxml_node_t* io_config_node_p=NULL;
    mxml_node_t* io_descrip_tmp_p=NULL;
    byte_8 dev_lid[DESCRIPTION_ITEM_MAX_LEN];
    byte_8 dev_type[DESCRIPTION_ITEM_MAX_LEN];
    byte_8 dev_attr_name[DESCRIPTION_ITEM_MAX_LEN];
    byte_8 dev_attr_value[DESCRIPTION_ITEM_MAX_LEN];
    uint_32 device_id_tmp=0;
    uint_32 io_id_tmp=0;
    for(dev_node_p=mxmlFindElement(root,root,device_tag,NULL,NULL,MXML_DESCEND);dev_node_p!=NULL;dev_node_p=mxmlFindElement(dev_node_p,root,device_tag,NULL,NULL,MXML_NO_DESCEND)){
        /*
         * 对每一个device_entity节点,提取lid，type，interface，attr，然后再找到io_entity依次迭代
         * 创建一个DeviceNode节点
         */
        DeviceNode* dev_node=(DeviceNode*)malloc(sizeof(DeviceNode));
        mxml_node_t* dev_dependence=mxmlFindElement(dev_node_p,root,"dependence",NULL,NULL,MXML_DESCEND);
        strcpy(dev_lid,mxmlElementGetAttr(dev_node_p,"lid"));
        strcpy(dev_type,mxmlElementGetAttr(dev_node_p,"type"));
        strcpy(dev_node->descrip.lid,dev_lid);
        strcpy(dev_node->descrip.type,dev_type);
        strcpy(dev_node->descrip.interface,getInterface(mxmlGetText(dev_dependence,NULL),strlen(mxmlGetText(dev_dependence,NULL))));
        dev_node->device_id=device_id_tmp++;
        /*添加设备属性并找到第一个io_entity和io_config*/
        dev_node->descrip.attr_len=0;
        mxml_node_t* descrip=mxmlFindElement(dev_node_p,root,"description",NULL,NULL,MXML_DESCEND);
        for(dev_attr_node_p=mxmlFindElement(descrip,root,NULL,NULL,NULL,MXML_DESCEND);dev_attr_node_p!=NULL;dev_attr_node_p=mxmlFindElement(dev_attr_node_p,descrip,NULL,NULL,NULL,MXML_NO_DESCEND)){
                strcpy(dev_node->descrip.attrs.attr_name[dev_node->descrip.attr_len],dev_attr_node_p->value.element.name);
                strcpy(dev_node->descrip.attrs.attr_value[dev_node->descrip.attr_len],mxmlGetText(dev_attr_node_p,NULL));
                dev_node->descrip.attr_len++;
        }
        const byte_8 *dev_content_tmp=mxmlGetText(dev_dependence,NULL);
        uint_32 dev_content_tmp_len=strlen(dev_content_tmp);
        io_node_p=findDependedNode(dev_content_tmp,dev_content_tmp_len,root,false);
        io_config_node_p=findDependedNode(dev_content_tmp,dev_content_tmp_len,root,true);
        /*赋值并迭代找io*/
        const byte_8* io_lid=NULL;
        const byte_8* io_type=NULL;
        byte_8 io_attr_name[DESCRIPTION_ITEM_MAX_LEN];
        byte_8 io_attr_value[DESCRIPTION_ITEM_MAX_LEN];
        byte_8 dev_lid[DESCRIPTION_ITEM_MAX_LEN];
        byte_8 rt_send_priority[DESCRIPTION_ITEM_MAX_LEN];
        byte_8 rt_receive_priority[DESCRIPTION_ITEM_MAX_LEN];
        byte_8 rt_send_block_size[DESCRIPTION_ITEM_MAX_LEN];
        byte_8 rt_receive_block_size[DESCRIPTION_ITEM_MAX_LEN];
        io_id_tmp=0;
        IONode* prev_p=NULL;
        for(;io_node_p!=NULL;){
            IONode* io_node=(IONode*)malloc(sizeof(IONode));
            io_node->next=NULL;
            if(io_id_tmp==0){
                dev_node->next=io_node;
                DeviceList[DeviceListPos++]=dev_node;
            }
            else{
                prev_p->next=io_node;
            }
            prev_p=io_node;
            io_node->IO_id=io_id_tmp++;
            if(strcmp(io_node_p->value.element.name,"channel")==0){
                io_lid=mxmlElementGetAttr(io_node_p->parent,"lid");
                io_type=mxmlElementGetAttr(io_node_p->parent,"type");
            }
            else{
                io_lid=mxmlElementGetAttr(io_node_p,"lid");
                io_type=mxmlElementGetAttr(io_node_p,"type");
            }
            strcpy(io_node->descrip.lid,io_lid);
            strcpy(io_node->descrip.type,io_type);
            io_node->descrip.io_attr_len=0;
            if(strcmp(io_type,"RT")==0){
                io_descrip_tmp_p=mxmlFindElement(io_node_p,root,"tranData_description",NULL,NULL,MXML_DESCEND);
                io_attr_node_p=mxmlFindElement(io_descrip_tmp_p,root,NULL,NULL,NULL,MXML_DESCEND_FIRST);
                uint_32 dev_map_pos=0;
                mxml_node_t* io_dev_map_tmp_p=mxmlFindElement(io_node_p,root,"dev_map_subAddr_list",NULL,NULL,MXML_DESCEND);
                mxml_node_t* dev_map_subAddr_node_p=mxmlFindElement(io_dev_map_tmp_p,root,NULL,NULL,NULL,MXML_DESCEND_FIRST);
                for(;dev_map_subAddr_node_p!=NULL;dev_map_subAddr_node_p=mxmlFindElement(dev_map_subAddr_node_p,io_dev_map_tmp_p,NULL,NULL,NULL,MXML_NO_DESCEND)){
                            strcpy(io_node->descrip.io_attrs.RTTrans.dev_map_subAddr[dev_map_pos++],mxmlGetText(dev_map_subAddr_node_p,NULL));
                }
            }
            else{
                io_descrip_tmp_p=mxmlFindElement(io_node_p,root,"description",NULL,NULL,MXML_DESCEND);
                io_attr_node_p=mxmlFindElement(io_descrip_tmp_p,root,NULL,NULL,NULL,MXML_DESCEND_FIRST);
            }
            for(;io_attr_node_p!=NULL;io_attr_node_p=mxmlFindElement(io_attr_node_p,io_descrip_tmp_p,NULL,NULL,NULL,MXML_NO_DESCEND)){
                    io_node->descrip.io_attr_len=0;
                    mxml_node_t* p_tmp=mxmlFindElement(io_attr_node_p,root,NULL,NULL,NULL,MXML_DESCEND);
                    if(strcmp(io_type,"RT")==0){

                        uint_32 RTTransPos=0;
                        byte_8* name_tmp=io_attr_node_p->value.element.name;
                        if(strcmp(name_tmp,"send_priority")==0){
                            RTTransPos=0;
                        }
                        else if(strcmp(name_tmp,"send_blockSize")==0){
                            RTTransPos=2;
                        }
                        else if(strcmp(name_tmp,"receive_priority")==0){
                            RTTransPos=1;
                        }
                        else if(strcmp(name_tmp,"receive_blockSize")==0){
                            RTTransPos=3;
                        }
                        else{
                            printf("RTTransPos wrong!\n");
                            return;
                        }
                        io_node->descrip.io_attrs.RTTrans.len=0;
                        uint_32 pos_tmp=0;
                        for(;p_tmp!=NULL;p_tmp=mxmlFindElement(p_tmp,io_attr_node_p,NULL,NULL,NULL,MXML_NO_DESCEND)){
                            strcpy(io_node->descrip.io_attrs.RTTrans.dev_lid[pos_tmp],mxmlElementGetAttr(p_tmp,"lid"));
                            if(RTTransPos==0)strcpy(io_node->descrip.io_attrs.RTTrans.info[RTTransPos].send_priority[pos_tmp++],mxmlGetText(p_tmp,NULL));

                            else if(RTTransPos==1)strcpy(io_node->descrip.io_attrs.RTTrans.info[RTTransPos].receive_priority[pos_tmp++],mxmlGetText(p_tmp,NULL));

                            else if(RTTransPos==2)strcpy(io_node->descrip.io_attrs.RTTrans.info[RTTransPos].send_block_size[pos_tmp++],mxmlGetText(p_tmp,NULL));

                            else if(RTTransPos==3)strcpy(io_node->descrip.io_attrs.RTTrans.info[RTTransPos].receive_block_size[pos_tmp++],mxmlGetText(p_tmp,NULL));
                           io_node->descrip.io_attrs.RTTrans.len++;
                            }

                    }
                    else{
                        /*如果不是子地址映射，直接放入io属性*/
                        if(strcmp(io_attr_node_p->value.element.name,"dev_map_subAddr_list")!=0){
                            strcpy(io_node->descrip.io_attrs.attrs.attr_name[io_node->descrip.io_attr_len],io_attr_node_p->value.element.name);
                            strcpy(io_node->descrip.io_attrs.attrs.attr_value[io_node->descrip.io_attr_len],mxmlGetText(io_attr_node_p,NULL));
                            io_node->descrip.io_attr_len++;
                        }
                    }

            }
            mxml_node_t* io_config_parent=mxmlFindElement(io_config_node_p,root,"parent",NULL,NULL,MXML_DESCEND);
            const byte_8* io_config_content=mxmlGetText(io_config_parent,NULL);
            io_node_p=findDependedNode(io_config_content,strlen(io_config_content),root,false);
            io_config_node_p=findDependedNode(io_config_content,strlen(io_config_content),root,true);
        }
    }
    DeviceListLen=DeviceListPos;
    DeviceListPos=0; //初始化DeviceList入口为第一个DeviceNode
    printf("生成配置文件解析存储结构完成...\n");
}

/** 
 * 获取接口名
 * @param[in]   dev_content    配置文件中设备属性字段dependency节点的text
 * @param[in]   dev_content_len    配置文件中设备属性字段dependency节点的text字段长度
 * @return      接口名
 * @ref         xml_parse.h 
 */ 
byte_8* getInterface(const byte_8* dev_content,uint_32 dev_content_len){
    byte_8* t=interface;
    int i=0;
    for(i=0;i<dev_content_len;i++){
        interface[i]=dev_content[i];
        if(dev_content[i]==':'){
            interface[i]='\0';
            break;
        }
    }
    return t;
}

mxml_node_t* findDependedNode(const byte_8* content,uint_32 content_len,mxml_node_t* root,bool isConfig){

    /*处理字符串*/
    mxml_node_t* t=NULL;
    byte_8 content_tmp[CONTENT_MAX_LEN]={0};
    byte_8 ROUTE[ROUTE_MAX_LEN][ROUTE_VALUE_MAX_LEN]={{0}};
    memset(ROUTE[ROUTE_CHANNEL_NAME_POS],0,ROUTE_VALUE_MAX_LEN);
    memset(ROUTE[ROUTE_CHANNEL_LID_VALUE_POS],0,ROUTE_VALUE_MAX_LEN);
    int j=0;
    int k=0;
    uint_32 content_tmp_len=0;
    uint_32 ROUTE_pos=0;
    uint_32 ROUTE_VALUE_pos=0;
    bool isHaveChannel=false;
    byte_8 io_tag_name_tmp[TAG_NAME_MAX_LEN]="io_";
    byte_8 io_tag_type[TAG_NAME_MAX_LEN]={0};
    byte_8 io_tag_entity_suffix[TAG_NAME_MAX_LEN]="_entity";
    byte_8 io_tag_config_suffix[TAG_NAME_MAX_LEN]="_config";
    byte_8 io_lid[TAG_NAME_MAX_LEN]={0};
    byte_8 channel_tag_name[TAG_NAME_MAX_LEN]={0};
    byte_8 channel_lid[TAG_NAME_MAX_LEN]={0};

    /*删掉content的前后空字符*/
    int i=0;
    for(i=0;i<content_len;i++){
        if(j==0){
            if(content[i]==' '){
                continue;
            }
            else{
                content_tmp[j++]=content[i];
                content_tmp_len++;
                continue;
            }
        }
        else{
                k=i;
                bool isLastWordsEmpty=true;
                int l;
                for(l=k;l<content_len;l++){
                    if(content[l]!=' '){
                        isLastWordsEmpty=false;
                    }
                 }
                if(isLastWordsEmpty==true){
                    break;
                }
                else{
                    content_tmp[j++]=content[i];
                    content_tmp_len++;
                    continue;
            }
        }
      content_tmp[j]='\0';
    }
    /*处理完前后空字符*/

    if(strcmp(content_tmp,"ROOT")==0)return NULL;
    /*放入ROUTE*/
    j=0;
    k=0;
    for(i=0;i<content_tmp_len;i++){
        k=i;
        if(content_tmp[i]==':'||content_tmp[i]==';'||i==content_tmp_len-1){
            if(i==content_tmp_len-1)k++;
            int l;
            for(l=j;l<k;l++){
                *(ROUTE[ROUTE_pos]+ROUTE_VALUE_pos++)=*(content_tmp+l);
            }
            *(ROUTE[ROUTE_pos]+ROUTE_VALUE_pos)='\0';
            ROUTE_pos++;
            ROUTE_VALUE_pos=0;
            j=i+1;
        }
        else{
            continue;
        }
    }
    /*查找Node*/
    if(strlen(ROUTE[ROUTE_CHANNEL_NAME_POS])>0&&strlen(ROUTE[ROUTE_CHANNEL_LID_VALUE_POS])>0){
        isHaveChannel=true;
    }
    strcat(io_tag_name_tmp,ROUTE[ROUTE_IO_NAME_POS]);
    if(isConfig==false)strcat(io_tag_name_tmp,io_tag_entity_suffix);
    else strcat(io_tag_name_tmp,io_tag_config_suffix);
    strcpy(io_lid,ROUTE[ROUTE_IO_LID_VALUE]);
    strcpy(channel_tag_name,ROUTE[ROUTE_CHANNEL_NAME_POS]);
    strcpy(channel_lid,ROUTE[ROUTE_CHANNEL_LID_VALUE_POS]);
    if(isConfig==false){
         t=mxmlFindElement(root,root,io_tag_name_tmp,"lid",io_lid,MXML_DESCEND);
         if(isHaveChannel==true){
             t=mxmlFindElement(t,root,channel_tag_name,"lid",channel_lid,MXML_DESCEND);
         }
    }
    else{
        mxml_node_t* t_tmp=NULL;
        for(t_tmp=mxmlFindElement(root,root,io_tag_name_tmp,"lid",io_lid,MXML_DESCEND);t_tmp!=NULL;t_tmp=mxmlFindElement(t,root,io_tag_name_tmp,"lid",io_lid,MXML_DESCEND)){
            t=t_tmp;
            if(isHaveChannel==true){
                if(strcmp(mxmlElementGetAttr(t,"channel"),channel_lid)==0){
                    break;
                }
            }
        }
    }
    return t;
}

/*
 *打印配置文件解析存储结构 
 */
void printList(){
    printf("enter print...\n");
    printf("list len:%d\n",get_devices_descrip_item_num());
    int i=0;
    for(i=0;i<get_devices_descrip_item_num();i++){
    printf("____________________________\n");
    printf("enterpos: %d\n",i);
        void* dev_p=get_device_list_item(i);
        printf("device lid:%s\n",get_device_lid(dev_p));
        printf("device type:%s\n",get_device_type(dev_p));
        printf("device interface:%s\n",get_device_interface(dev_p));
        int j=0;
        for(j=0;j<DeviceList[i]->descrip.attr_len;j++){
            printf("%s: ",DeviceList[i]->descrip.attrs.attr_name[j]);
            printf("%s\n",DeviceList[i]->descrip.attrs.attr_value[j]);
        }
        printf("\n");
        IONode* io_tmp=DeviceList[i]->next;
        for(;io_tmp!=NULL;io_tmp=io_tmp->next){
            printf("io type:%s\n",io_tmp->descrip.type);
            printf("io lid:%s\n",io_tmp->descrip.lid);
            if(strcmp(io_tmp->descrip.type,"RT")==0){
                //RT_trans
                int k=0;
                for(k=0;k<io_tmp->descrip.io_attrs.RTTrans.len;k++){
                    printf("_____addr_map_____\n");
                    printf("device lid:%s->",io_tmp->descrip.io_attrs.RTTrans.dev_lid[k]);
                    printf("addr:%s\n",io_tmp->descrip.io_attrs.RTTrans.dev_map_subAddr[k]);
                }
                for(k=0;k<io_tmp->descrip.io_attrs.RTTrans.len;k++){
                    printf("_____send_prio_____\n");
                    printf("device lid:%s",io_tmp->descrip.io_attrs.RTTrans.dev_lid[k]);
                    printf("weight:%s\n",io_tmp->descrip.io_attrs.RTTrans.info[SEND_PRIORITY_POS].send_priority[k]);
                }
                for(k=0;k<io_tmp->descrip.io_attrs.RTTrans.len;k++){
                    printf("_____send_block_____\n");
                    printf("device lid:%s",io_tmp->descrip.io_attrs.RTTrans.dev_lid[k]);
                    printf("weight:%s\n",io_tmp->descrip.io_attrs.RTTrans.info[SEND_BLOCK_POS].send_priority[k]);
                }
                for(k=0;k<io_tmp->descrip.io_attrs.RTTrans.len;k++){
                    printf("_____receive_prio_____\n");
                    printf("device lid:%s",io_tmp->descrip.io_attrs.RTTrans.dev_lid[k]);
                    printf("weight:%s\n",io_tmp->descrip.io_attrs.RTTrans.info[RECEIVE_PRIORITY_POS].send_priority[k]);
                }
                for(k=0;k<io_tmp->descrip.io_attrs.RTTrans.len;k++){
                    printf("_____receive_block_____\n");
                    printf("device lid:%s",io_tmp->descrip.io_attrs.RTTrans.dev_lid[k]);
                    printf("weight:%s\n",io_tmp->descrip.io_attrs.RTTrans.info[RECEIVE_BLOCK_POS].send_priority[k]);
                }

            }
            else if(io_tmp->descrip.io_attr_len!=0){
                //io_attr
                printf("___io_attr______\n");
                int k=0;
                for(k=0;k<io_tmp->descrip.io_attr_len;k++){
                    printf("%s:",io_tmp->descrip.io_attrs.attrs.attr_name[k]);
                    printf("%s\n",io_tmp->descrip.io_attrs.attrs.attr_value[k]);
                }
            }
            printf("\n\n");
        }
    }
}

/*以下几个接口用于不需要通过1553总线转发数据的设备*/
void* get_devices_descrip_item(void){
    DescripEntryPos=DescripEntryPos%DeviceListLen;
    return (void*)(&DeviceList[DescripEntryPos++]->descrip);
}

uint_32 get_devices_descrip_item_num(void){
    return DeviceListLen;
}

byte_8* get_device_lid(void* descrip){
    return ((dev_descrip*)descrip)->lid;
}

byte_8* get_device_type(void* descrip){
    return ((dev_descrip*)descrip)->type;
}

byte_8* get_device_interface(void* descrip){
    return ((dev_descrip*)descrip)->interface;
}

/*以下接口用于需要通过1553总线转发数据的设备*/
void* get_devices_list_item(void){
    DeviceListPos=DeviceListPos%DeviceListLen;
    return (void*)(DeviceList[DeviceListPos++]);
}

void* get_device_list_item(uint_32 pos){
    if(pos>=DeviceListLen)return NULL;
    else return (void*)(DeviceList[pos]);
}

uint_32 get_device_attr_len(void* dev_item){
    return ((DeviceNode*)dev_item)->descrip.attr_len;
}

byte_8* get_device_attrs_name(void* dev_item){
    dev_attr_name_pos=dev_attr_name_pos%((DeviceNode*)dev_item)->descrip.attr_len;
    return ((DeviceNode*)dev_item)->descrip.attrs.attr_name[dev_attr_name_pos++];
}

byte_8* get_device_attrs_value(void* dev_item){
    dev_attr_value_pos=dev_attr_value_pos%((DeviceNode*)dev_item)->descrip.attr_len;
    return ((DeviceNode*)dev_item)->descrip.attrs.attr_value[dev_attr_value_pos++];
}

void* get_device_io_item(void* dev_item){
    return ((DeviceNode*)dev_item)->next;
}

void* get_io_lid(void* io_item){
    return ((IONode*)io_item)->descrip.lid;
}

byte_8* get_io_type(void* io_item){
    return ((IONode*)io_item)->descrip.type;
}

uint_32 get_io_attr_len(void* io_item){
    return ((IONode*)io_item)->descrip.io_attr_len;
}

byte_8* get_io_attrs_name(void* io_item){
    io_attr_name_pos=io_attr_name_pos%((IONode*)io_item)->descrip.io_attr_len;
    return ((IONode*)io_item)->descrip.io_attrs.attrs.attr_name[io_attr_name_pos++];
}

byte_8* get_io_attrs_value(void* io_item){
    io_attr_value_pos=io_attr_value_pos%((IONode*)io_item)->descrip.io_attr_len;
    return ((IONode*)io_item)->descrip.io_attrs.attrs.attr_value[io_attr_value_pos++];
}

uint_32 get_RT_trans_device_num(void* io_item){
    return ((IONode*)io_item)->descrip.io_attrs.RTTrans.len;
}

byte_8* get_RT_trans_devices_lid(void* io_item){
    RT_trans_device_lid_pos=RT_trans_device_lid_pos%((IONode*)io_item)->descrip.io_attrs.RTTrans.len;
    return ((IONode*)io_item)->descrip.io_attrs.RTTrans.dev_lid[RT_trans_device_lid_pos++];
}

byte_8* get_RT_trans_device_send_priority(byte_8* dev_lid,void* io_item){
    byte_8* t="";
    if(strcmp(get_io_type(io_item),"RT")!=0)return t;
    int i=0;
    int pos_tmp=-1;
    for(;i<get_RT_trans_device_num(io_item);i++){
        if(strcmp(((IONode*)io_item)->descrip.io_attrs.RTTrans.dev_lid[i],dev_lid)==0){
            pos_tmp=i;
        }
    }
    if(pos_tmp==-1)return t;
    else{
        t=((IONode*)io_item)->descrip.io_attrs.RTTrans.info[SEND_PRIORITY_POS].send_priority[pos_tmp];
    }
    if(strcmp(t,"null")==0)t="";
    return t;
}

byte_8* get_RT_trans_device_send_block_size(byte_8* dev_lid,void* io_item){
    byte_8* t="";
    if(strcmp(get_io_type(io_item),"RT")!=0)return t;
    int i=0;
    int pos_tmp=-1;
    for(;i<get_RT_trans_device_num(io_item);i++){
        if(strcmp(((IONode*)io_item)->descrip.io_attrs.RTTrans.dev_lid[i],dev_lid)==0){
            pos_tmp=i;
        }
    }
    if(pos_tmp==-1)return t;
    else{
        t=((IONode*)io_item)->descrip.io_attrs.RTTrans.info[SEND_BLOCK_POS].send_block_size[pos_tmp];
    }
    if(strcmp(t,"null")==0)t="";
    return t;
}

byte_8* get_RT_trans_device_receive_priority(byte_8* dev_lid,void* io_item){
    byte_8* t="";
    if(strcmp(get_io_type(io_item),"RT")!=0)return t;
    int i=0;
    int pos_tmp=-1;
    for(;i<get_RT_trans_device_num(io_item);i++){
        if(strcmp(((IONode*)io_item)->descrip.io_attrs.RTTrans.dev_lid[i],dev_lid)==0){
            pos_tmp=i;
        }
    }
    if(pos_tmp==-1)return t;
    else{
        t=((IONode*)io_item)->descrip.io_attrs.RTTrans.info[RECEIVE_PRIORITY_POS].send_block_size[pos_tmp];
    }
    if(strcmp(t,"null")==0)t="";
    return t; 
}

byte_8* get_RT_trans_device_receive_block_size(byte_8*dev_lid,void* io_item){
    byte_8* t="";
    if(strcmp(get_io_type(io_item),"RT")!=0)return t;
    int i=0;
    int pos_tmp=-1;
    for(;i<get_RT_trans_device_num(io_item);i++){
        if(strcmp(((IONode*)io_item)->descrip.io_attrs.RTTrans.dev_lid[i],dev_lid)==0){
            pos_tmp=i;
        }
    }
    if(pos_tmp==-1)return t;
    else{
        t=((IONode*)io_item)->descrip.io_attrs.RTTrans.info[RECEIVE_BLOCK_POS].receive_block_size[pos_tmp];
    }
    if(strcmp(t,"null")==0)t="";
    return t; 
}
byte_8* get_RT_trans_device_map_subAddr(byte_8*dev_lid,void* io_item){
    byte_8* t="";
    if(strcmp(get_io_type(io_item),"RT")!=0)return t;
    int pos_tmp=-1;
    int i=0;
    for(;i<get_RT_trans_device_num(io_item);i++){
        if(strcmp(((IONode*)io_item)->descrip.io_attrs.RTTrans.dev_lid[i],dev_lid)==0){
            pos_tmp=i;
            break;
        }
    }
    if(pos_tmp==-1)return t;
    else{
        t=((IONode*)io_item)->descrip.io_attrs.RTTrans.dev_map_subAddr[pos_tmp];
    }
    if(strcmp(t,"null")==0)t="";
    return t; 
}

void* get_io_next_item(void* io_item){
    return (void*)(((IONode*)io_item)->next);
}

byte_8* get_device_data_size(void* dev_item){
    byte_8* attr_name="";
    byte_8* attr_value="";
    int i=0;
    for(i=0;i<get_device_attr_len(dev_item);i++){
        attr_name=get_device_attrs_name(dev_item);
        attr_value=get_device_attrs_value(dev_item);
        if(strcmp(attr_name,"dataSize")==0){
         break; 
        }
    }
    if(strcmp(attr_name,"dataSize")!=0)attr_value="";
    return attr_value;
}

uint_32 get_device_list_item_num(void){
    return DeviceListLen;    
}

byte_8* get_device_item_type(void* dev_item){
    return ((DeviceNode*)dev_item)->descrip.type;
}

byte_8* get_device_item_lid(void* dev_item){
    return ((DeviceNode*)dev_item)->descrip.lid;
}

void free_device_list(){
    DeviceNode** pp=DeviceList;
    int cnt=0;
    int i=0;
    for(i=0;i<get_device_list_item_num();i++){
        DeviceNode* d_p=DeviceList[i];
        if(d_p!=NULL){
            IONode* i_p=d_p->next;        
            free(d_p);
            cnt++;
            while(i_p!=NULL){
                IONode* i_p_tmp=i_p;
                i_p=i_p->next;
                free(i_p_tmp);
                cnt++;
            }
        }
    }
    for(i=0;i<get_devices_descrip_item_num();i++){
        DeviceList[i]=NULL;
    }
    DeviceListLen=0;
    mxmlDelete(root);
}
