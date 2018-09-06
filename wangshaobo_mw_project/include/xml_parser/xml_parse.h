/*
 * xml_parse.h,声明配置文件解析存储结构以及各种操作函数。
 */
#ifndef MXML_H_
#define MXML_H_ 
#include "m_type.h"
#include "mxml.h"
typedef struct RTTransaction{
    uint_32 len;
    byte_8 dev_lid[DEV_LID_MAX_LEN][ATTR_LID_VALUE_MAX_LEN];
    byte_8 dev_map_subAddr[DEV_LID_MAX_LEN][RT_SUBADDR_VALUE_MAX_LEN];
    union info{
        byte_8 send_priority[DEV_LID_MAX_LEN][SEND_PRIORITY_VALUE_MAX_LEN];
        byte_8 receive_priority[DEV_LID_MAX_LEN][RECEIVE_PRIORITY_VALUE_MAX_LEN];
        byte_8 send_block_size[DEV_LID_MAX_LEN][SEND_BLOCK_VALUE_MAX_LEN];
        byte_8 receive_block_size[DEV_LID_MAX_LEN][RECEIVE_BLOCK_VALUE_MAX_LEN];
    }info[TRAN_DATA_ITEM_MAX_LEN];
}RTTransaction;
typedef struct attr{
    byte_8 attr_name[ATTR_MAX_LEN][ATTR_VALUE_MAX_LEN];
    byte_8 attr_value[ATTR_MAX_LEN][ATTR_VALUE_MAX_LEN];
}attribute;
typedef struct dev_description{
    byte_8 lid[ATTR_LID_VALUE_MAX_LEN];
    byte_8 type[ATTR_TYPE_VALUE_MAX_LEN];
    byte_8 interface[ATTR_INTERFACE_VALUE_MAX_LEN];
    uint_32 attr_len;
    attribute attrs;
}dev_descrip;
typedef union io_attributes{
    attribute attrs;
    RTTransaction RTTrans;
}io_attributes;
typedef struct io_description{
    byte_8 lid[ATTR_LID_VALUE_MAX_LEN];
    byte_8 type[ATTR_TYPE_VALUE_MAX_LEN];
    uint_32 io_attr_len;
    io_attributes io_attrs;
}io_descrip;
typedef struct IONode{
	uint_32 IO_id;
	io_descrip descrip;
	struct IONode* next;
}IONode;
typedef struct DeviceNode{
	uint_32 device_id;
	dev_descrip descrip;
	struct IONode* next;
}DeviceNode;
void parseXml(void);
byte_8* getInterface(const byte_8* dev_content,uint_32 dev_content_len);
mxml_node_t* findDependedNode(const byte_8* content,uint_32 content_len,mxml_node_t* root,bool isConfig);
void printList();
void* get_devices_descrip_item(void);
uint_32 get_devices_descrip_item_num(void);
byte_8* get_device_lid(void* descrip);
byte_8* get_device_type(void* descrip);
byte_8* get_device_interface(void* descrip);
///
void* get_devices_list_item(void);
void* get_device_list_item(uint_32 pos);
uint_32 get_device_list_item_num(void);
uint_32 get_device_attr_len(void* dev_item);
byte_8* get_device_attrs_name(void* dev_item);
byte_8* get_device_attrs_value(void* dev_item);
byte_8* get_device_data_size(void* dev_item);
byte_8* get_device_item_lid(void* dev_item);
byte_8* get_device_item_type(void* dev_item);
void* get_device_io_item(void* dev_item);
void* get_io_lid(void* io_item);
byte_8* get_io_type(void* io_item);
uint_32 get_io_attr_len(void* io_item);//
byte_8* get_io_attrs_name(void* io_tem);
byte_8* get_io_attrs_value(void* io_item);
uint_32 get_RT_trans_device_num(void* io_item);//
byte_8* get_RT_trans_devices_lid(void* io_item);
byte_8* get_io_attrs_name(void* io_item);
byte_8* get_io_attrs_value(void* io_item);
byte_8* get_RT_trans_device_send_priority(byte_8* dev_lid,void* io_item);
byte_8* get_RT_trans_device_send_block_size(byte_8* dev_lid,void* io_item);
byte_8* get_RT_trans_device_receive_priority(byte_8* dev_lid,void* io_item);
byte_8* get_RT_trans_device_receive_block_size(byte_8* dev_lid,void* io_item);
void* get_io_next_item(void* io_item);
byte_8* get_RT_trans_device_map_subAddr(byte_8*dev_lid,void* io_item);

//提供接口free申请的中间节点，因为后面不需要再使用
void free_device_list();
#endif

