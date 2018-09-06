/** 
* @file        handle_data_package.h
* @brief    声明数据包转发规则表结构和各种对表操作的函数
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/


#ifndef HANDLE_DATA_PACKAGE_H_
#define HANDLE_DATA_PACKAGE_H_
#include "m_type.h"
//#include "xml_parse.h"
///定义包转发规则表和接口
#define DATA_TRANS_RULE_FORM_MAX_LEN 1
#define RT_TRANS_RULE_MAX_LEN 32
#define DEV_TRANS_DATA_INFO_MAX_LEN 32
#define PRIORITY uint_32
#define BLOCK_SIZE uint_32
#define TRANS_DEV_MAX_SIZE 100
#define SEND_PRIORITY_FLAG 1
#define SEND_BLOCK_FLAG 2
#define RECEIVE_PRIORITY_FLAG 3
#define RECEIVE_BLOCK_FLAG 4
#define MAX_PRIORITY 1000
#define LOWEST_PRIORITY -1000
#define MIN_DEVIATION 2000

/** 设备转发信息结构 */
typedef struct dev_trans_data_info{
    byte_8 dev_lid[ATTR_LID_VALUE_MAX_LEN];
    uint_32 data_size;
    byte_8 RT_subAddr[RT_SUBADDR_VALUE_MAX_LEN];
    PRIORITY send_priority;
    PRIORITY receive_priority;
    BLOCK_SIZE send_block_size;
    BLOCK_SIZE receive_block_size;
}dev_trans_data_info;
/** RT转发规则结构 */
typedef struct RT_trans_rule{
    byte_8 RT_lid[ATTR_LID_VALUE_MAX_LEN];
    uint_32 info_section_len;
    dev_trans_data_info *info_section;
}RT_trans_rule;
/** 包转发规则表结构 */
typedef struct data_trans_rule_form{
    byte_8 bus_lid[ATTR_LID_VALUE_MAX_LEN];
    byte_8 bus_type[ATTR_TYPE_VALUE_MAX_LEN];
    uint_32 rule_section_len;
    RT_trans_rule *rule_section;
}data_trans_rule_form;

/** 
 * 创建包转发规则表，每条1553总线都有一个包转发规则表
 */ 
void create_data_trans_rule_form(void);
/** 
 * 打印包转发规则表
 */ 
void print_form();
/** 
 * 获取包转发规则表个数
 * @return     包转发规则表个数
 */ 
uint_32 get_form_num();
/** 
 * 获取包转发规则表指针，内部使用
 * @param[in]   item_pos     包转发规则表id
 * @return      包转发规则表指针
 */ 
void* get_forms_item(uint_32 item_pos);
/** 
 * 获取包转发规则表对应的总线逻辑标识符
 * @param[in]   form_item     包转发规则表指针
 * @return      总线逻辑标识符
 */ 
byte_8* get_form_bus_lid(void* form_item);
/** 
 * 获取包转发规则表对应的总线类型
 * @param[in]   form_item     包转发规则表指针
 * @return      总线类型
 */ 
byte_8* get_form_bus_type(void* form_item);
/** 
 * 获取包转发规则表中RT转发规则个数
 * @param[in]   form_item     包转发规则表指针
 * @return      RT转发规则个数
 */ 
uint_32 get_form_rule_section_len(void* form_item);
/** 
 * 获取RT转发规则指针
 * @param[in]   form_item     包转发规则表指针
 * @param[in]   item_pos      RT转发规则的id 
 * @return      RT转发规则的指针
 */ 
void* get_form_rule_section_item(void* form_item,uint_32 item_pos);
/** 
 * 获取RT转发规则对应的RT逻辑标识符
 * @param[in]   rule_item     指向RT转发规则的指针
 * @return      RT的逻辑标识符
 */ 
byte_8* get_form_rule_RT_lid(void* rule_item);
/** 
 * 获取设备转发信息个数
 * @param[in]   rule_item     RT转发规则指针
 * @return      设备转发信息个数
 */ 
uint_32 get_form_info_section_len(void* rule_item);
/** 
 * 获取设备转发信息
 * @param[in]   rule_item     RT转发规则指针
 * @param[in]   item_pos      设备转发信息id
 * @return      设备转发信息指针
 */ 
void* get_form_info_section_item(void* rule_item,uint_32 item_pos);
/** 
 * 获取设备转发信息中的设备逻辑标识符
 * @param[in]   info_item     设备转发信息指针
 * @return      设备逻辑标识符
 */ 
byte_8* get_form_info_dev_lid(void* info_item);
/** 
 * 获取设备转发信息中设备收发数据对应的数据粒度(默认1B)
 * @param[in]   info_item     设备转发信息指针
 * @return      数据粒度
 */ 
uint_32 get_form_info_dev_data_size(void* info_item);
/** 
 * 获取设备转发信息中设备发数据对应的优先级
 * @param[in]   info_item     设备转发信息指针
 * @return      优先级
 */ 
uint_32 get_form_info_dev_send_priority(void* info_item);
/** 
 * 获取设备转发信息中设备收数据对应的优先级
 * @param[in]   info_item     设备转发信息指针
 * @return      优先级
 */ 
uint_32 get_form_info_dev_receive_priority(void* info_item);
/** 
 * 获取设备转发信息中设备发数据对应的数据块大小
 * @param[in]   info_item     设备转发信息指针
 * @return      数据块大小
 */ 
uint_32 get_form_info_dev_send_block_size(void* info_item);
/** 
 * 获取设备转发信息中设备收数据对应的数据块大小
 * @param[in]   info_item     设备转发信息指针
 * @return      数据块大小
 */ 
uint_32 get_form_info_dev_receive_block_size(void* info_item);


/** 
 * 获取包转发规则表指针
 * @param[in]   bus_type     总线类型
 * @param[in]   bus_lid      总线逻辑标识符
 * @return      包转发规则表指针
 */ 
void* get_form_item(byte_8* bus_type,byte_8* bus_lid);

/** 
 * 获取RT转发规则指针
 * @param[in]   bus_type     总线类型
 * @param[in]   bus_lid      总线逻辑标识符
 * @param[in]   RT_lid       RT逻辑标识符
 * @return      RT转发规则指针
 */ 
void* get_form_rule_item(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid);

//byte_8* get_max_priority_dev_lid(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,uint_32 attr_flag,uint_32* current_priority,uint_32* anchor);

/** 
 * RT转发规则内按优先级递减获取设备逻辑标识符
 * @param[in]   bus_type     总线类型
 * @param[in]   bus_lid      总线逻辑标识符
 * @param[in]   RT_lid       RT逻辑标识符
 * @param[in]   attr_flag    标识是是设备收还是设备发
 * @param[in]   prev_priority      从当前的优先级开始递减
 * @param[out]  current_priority       当前的设备对应的收/发优先级
 * @param[out]  anchor      以当前位置(设备信息所在下标)开始查找，并重新复制为新的位置，一般设初值0传入  
 * @return      设备逻辑标识符
 */ 
byte_8* get_priority_deterio_dev_lid(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,\
        uint_32 attr_flag,uint_32 prev_priority,uint_32* current_priority,uint_32* anchor);

/** 
 * 获取设备转发信息中的设备属性
 * @param[in]   bus_type     总线类型
 * @param[in]   bus_lid      总线逻辑标识符
 * @param[in]   RT_lid       RT逻辑标识符
 * @param[in]   dev_lid      设备逻辑标识符
 * @param[in]   attr_flag    标识是设备属性@ref handle_data_package.h
 * @return      设备属性
 */ 
uint_32 get_dev_trans_attr(byte_8* bus_type,byte_8* bus_lid,byte_8*RT_lid,byte_8* dev_lid,uint_32 attr_flag);

/** 
 * 获取设备转发信息中设备粒度大小
 * @param[in]   bus_type     总线类型
 * @param[in]   bus_lid      总线逻辑标识符
 * @param[in]   RT_lid       RT逻辑标识符
 * @param[in]   dev_lid      设备逻辑标识符
 * @return      设备数据粒度大小
 */ 
uint_32 get_dev_data_size(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8* dev_lid);
/** 
 * 获取使用总线转发的设备在总线转发数组中的标号@ref irdirect_trans_dev_lid
 * @param[in]   dev_lid      设备逻辑标识符
 * @return      标号
 */ 
uint_32 find_irdirect_dev_pos(byte_8* dev_lid);
/** 
 * RT在总线转发数组中的标号@ref irdirect_trans_RT_lid
 * @param[in]   RT_lid      RT逻辑标识符
 * @return      标号
 */ 
uint_32 find_irdirect_RT_pos(byte_8* RT_lid);
/** 
 * 获取设备转发信息中设备对应的RT子地址
 * @param[in]   info_item      设备转发信息指针
 * @return      RT子地址
 */ 
byte_8* get_form_info_dev_RT_subAddr(void* info_item);
/** 
 * 获取设备转发信息中设备对应的RT子地址功能与,@ref get_form_info_dev_RT_subAddr一致
 * @param[in]   bus_type     总线类型
 * @param[in]   bus_lid      总线逻辑标识符
 * @param[in]   RT_lid       RT逻辑标识符
 * @param[in]   dev_lid      设备逻辑标识符
 * @return      RT子地址
 */ 
byte_8* get_dev_map_RT_subAddr(byte_8* bus_type,byte_8* bus_lid,byte_8*RT_lid,byte_8* dev_lid);
/** 
 * 获取总线设备号
 * @param[in]   bus_type     总线类型
 * @param[in]   bus_lid      总线逻辑标识符
 * @return      总线设备号
 */ 
uint_32 get_1553_device_num(byte_8* bus_type,byte_8* bus_lid);
/** 
 * 获取RT地址
 * @param[in]   bus_type     总线类型
 * @param[in]   bus_lid      总线逻辑标识符
 * @param[in]   RT_lid       RT逻辑标识符
 * @return      RT地址
 */ 
uint_32 get_RT_physical_addr(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid);
/** 
 * 获取RT子地址
 * @param[in]   bus_type     总线类型
 * @param[in]   bus_lid      总线逻辑标识符
 * @param[in]   RT_lid       RT逻辑标识符
 * @param[in]   dev_lid      设备逻辑标识符
 * @return   RT子地址   
 */ 
uint_32 get_RT_physical_sub_addr(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8* dev_lid);

/** 
 * 获取转发数组中的设备逻辑标识符@ref irdirect_trans_dev_lid
 * @param[in]   irdirect_pos      下标
 * @return   设备逻辑标识符  
 */ 
byte_8* get_irdirect_dev_lid(uint_32 irdirect_pos);
/** 
 * 获取转发数组中的RT逻辑标识符@ref irdirect_trans_RT_lid
 * @param[in]   irdirect_pos      下标
 * @return   设备逻辑标识符  
 */ 
byte_8* get_irdirect_RT_lid(uint_32 irdirect_pos);
/** 
 * 获取转发数组中的总线类型@ref irdirect_trans_bus_type
 * @param[in]   irdirect_pos      下标
 * @return   设备逻辑标识符  
 */ 
byte_8* get_irdirect_bus_type(uint_32 irdirect_pos);
/** 
 * 获取转发数组中的总线逻辑标识符@ref irdirect_trans_bus_lid
 * @param[in]   irdirect_pos      下标
 * @return   设备逻辑标识符  
 */ 
byte_8* get_irdirect_bus_lid(uint_32 irdirect_pos);
#endif
