/** 
* @file     address_map.h
* @brief    三级索引表结构用来管理逻辑标示符与转发地址
* 之间的映射,第一级索引表使用设备逻辑标识符标识，第二级索引
* 表使用RT逻辑标识符标识，第三级索引表使用总线标识符标识
* @author       Shaobo Wang
* @date     2018-5-21 
* @version  A001 
* @par Copyright (c):  Shaobo Wang 
*/

#ifndef ADDRESS_MAP_H_
#define ADDRESS_MAP_H_
#include "m_type.h"
#include "relevant_struct_def.h"
/** 第一级索引表表项最大数量 */
#define F_INDEX_LIST_MAX_LEN 5
/** 第二级索引表表项最大数量 */
#define S_INDEX_LIST_MAX_LEN 32
/** 第三级索引表表项最大数量 */
#define T_INDEX_LIST_MAX_LEN 20
/** 第三级索引表最大数量 */
#define T_INDEX_LIST_MAX_NUM 160
/** 第二级索引表最大数量 */
#define S_INDEX_LIST_MAX_NUM 5
/** 设备对应读区域最大大小 */
#define READ_REGION_MAX_SIZE 0x400
/** 设备对应写区域最大大小 */
#define WRITE_REGION_MAX_SIZE 0x400

/** 第三级索引表节点结构 */
typedef struct t_index_node{
    byte_8 dev_lid[ATTR_LID_VALUE_MAX_LEN];
    dataNode* addr_read_s;
    dataNode* addr_read_e;
    dataNode* addr_work_r;
    dataNode* addr_write_s;
    dataNode* addr_write_e;
    dataNode* addr_work_w;
}t_index_node;
/** 第三级索引表结构 */
typedef struct t_index_list{
    uint_32 t_node_len;
    t_index_node* t_node;
}t_index_list;
/** 第二级索引表节点结构 */
typedef struct s_index_node{
    byte_8 RT_lid[ATTR_LID_VALUE_MAX_LEN];
    t_index_list* t_list_p;
}s_index_node;
/** 第二级索引表结构 */
typedef struct s_index_list{
    uint_32 s_node_len;
    s_index_node* s_node;
}s_index_list;
/** 第一级索引表节点结构 */
typedef struct f_index_node{
    byte_8 bus_type[ATTR_TYPE_VALUE_MAX_LEN];
    byte_8 bus_lid[ATTR_LID_VALUE_MAX_LEN];
    s_index_list* s_list_p;
}f_index_node;
/** 第一级索引表结构 */
typedef struct f_index_list{
    uint_32 f_node_len;
    f_index_node* f_node;
}f_index_list;

/** 
 * 创建索引表
 */ 
void create_index_list(void);
/** 
 * 判断设备对应写区域是否为空
 * @param[in]   bus_type    1553总线类型
 * @param[in]   bus_lid     1553总线的逻辑标识符
 * @param[in]   RT_lid      RT的逻辑标识符
 * @param[in]   dev_lid     设备的逻辑标识符
 * @return      true，有数据，false，无数据
 */ 
bool is_write_region_empty(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8* dev_lid);
/** 
 * 判断设备对应读区域是否为空
 * @param[in]   bus_type    1553总线类型
 * @param[in]   bus_lid     1553总线的逻辑标识符
 * @param[in]   RT_lid      RT的逻辑标识符
 * @param[in]   dev_lid     设备的逻辑标识符
 * @return      true，有数据，false，无数据
 */
bool is_read_region_empty(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8* dev_lid);
/** 
 * 获取设备对应写区域数据大小(Byte)
 * @param[in]   bus_type    1553总线类型
 * @param[in]   bus_lid     1553总线的逻辑标识符
 * @param[in]   RT_lid      RT的逻辑标识符
 * @param[in]   dev_lid     设备的逻辑标识符
 * @return      数据大小
 */
uint_32 get_write_region_size(byte_8* bus_type,byte_8*bus_lid,byte_8* RT_lid,byte_8* dev_lid);
/** 
 * 获取设备对应读区域数据大小(Byte)
 * @param[in]   bus_type    1553总线类型
 * @param[in]   bus_lid     1553总线的逻辑标识符
 * @param[in]   RT_lid      RT的逻辑标识符
 * @param[in]   dev_lid     设备的逻辑标识符
 * @return      数据大小
 */
uint_32 get_read_region_size(byte_8* bus_type,byte_8*bus_lid,byte_8* RT_lid,byte_8* dev_lid);
/** 
 * 往设备方向读数据
 * @param[in]   bus_type    1553总线类型
 * @param[in]   bus_lid     1553总线的逻辑标识符
 * @param[in]   RT_lid      RT的逻辑标识符
 * @param[in]   dev_lid     设备的逻辑标识符
 * @param[out]   buffer     读出数据缓冲区地址
 * @param[in]   readSize    要读出的数据大小
 * @param[out]   size       读出的数据大小
 */
void dev_read_data(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8* dev_lid,void* buffer,uint_32 readSize,uint_32* size);
/** 
 * 往应用方向写数据
 * @param[in]   bus_type    1553总线类型
 * @param[in]   bus_lid     1553总线的逻辑标识符
 * @param[in]   RT_lid      RT的逻辑标识符
 * @param[in]   dev_lid     设备的逻辑标识符
 * @param[out]   buffer     写入数据缓冲区地址
 * @param[in]   writeSize    要写入的数据大小
 * @param[out]   size       写入的数据大小
 */
void dev_write_data(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8*dev_lid,void* buffer,uint_32 writeSize,uint_32* size);
/** 
 * 往应用方向读数据
 * @param[in]   bus_type    1553总线类型
 * @param[in]   bus_lid     1553总线的逻辑标识符
 * @param[in]   RT_lid      RT的逻辑标识符
 * @param[in]   dev_lid     设备的逻辑标识符
 * @param[out]   buffer     读出数据缓冲区地址
 * @param[in]   readSize    要读出的数据大小
 * @param[out]   size       读出的数据大小
 */
void app_read_data(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8* dev_lid,void* buffer,uint_32 readSize,uint_32* size);
/** 
 * 往设备方向写数据
 * @param[in]   bus_type    1553总线类型
 * @param[in]   bus_lid     1553总线的逻辑标识符
 * @param[in]   RT_lid      RT的逻辑标识符
 * @param[in]   dev_lid     设备的逻辑标识符
 * @param[out]   buffer     读出数据缓冲区地址
 * @param[in]   readSize    要写入的数据大小
 * @param[out]   size       写入的数据大小
 */
void app_write_data(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8*dev_lid,void* dataBuffer,uint_32 writeSize,uint_32* size);
/** 
 * 获取三级索引表项的指针
 * @param[in]   bus_type    1553总线类型
 * @param[in]   bus_lid     1553总线的逻辑标识符
 * @param[in]   RT_lid      RT的逻辑标识符
 * @param[in]   dev_lid     设备的逻辑标识符
 * @return      表项指针
 */
void* get_t_index_node(byte_8* bus_type,byte_8* bus_lid,byte_8* RT_lid,byte_8* dev_lid);
/** 
 * 获取读区域(相较于设备)buffer首指针
 * @param[in]   t_index_node    三级索引表项指针
 * @return      首指针
 */
void* get_read_addr_s(void* p_t_index_node);
/** 
 * 获取读区域(相较于设备)buffer尾指针
 * @param[in]   t_index_node    三级索引表项指针
 * @return      尾指针
 */
void* get_read_addr_e(void* p_t_index_node);
/** 
 * 获取读区域(相较于设备)buffer活动指针
 * @param[in]   t_index_node    三级索引表项指针
 * @return      活动指针
 */
void* get_read_addr_work_p(void* p_t_index_node);
/** 
 * 获取写区域(相较于设备)buffer首指针
 * @param[in]   t_index_node    三级索引表项指针
 * @return      首指针
 */
void* get_write_addr_s(void* p_t_index_node);
/** 
 * 获取写区域(相较于设备)buffer尾指针
 * @param[in]   t_index_node    三级索引表项指针
 * @return      尾指针
 */
void* get_write_addr_e(void* p_t_index_node);
/** 
 * 获取写区域(相较于设备)buffer活动指针
 * @param[in]   t_index_node    三级索引表项指针
 * @return      活动指针
 */
void* get_write_addr_work_p(void* p_t_index_node);
/** 
 * 获取二级索引表项数
 * @param[in]   bus_type    1553总线类型
 * @param[in]   bus_lid     1553总线的逻辑标识符
 * @return      索引表项数
 */
uint_32 get_s_index_list_len(byte_8* bus_type,byte_8* bus_lid);
/** 
 * 获取三级索引表指针
 * @param[in]   bus_type    1553总线类型
 * @param[in]   bus_lid     1553总线的逻辑标识符
 * @param[in]   pos     二级索引表的表项号
 * @return      三级索引表指针
 */
void* get_t_index_list(byte_8* bus_type,byte_8* bus_lid,uint_32 pos);
/** 
 * 获取二级索引表指针
 * @param[in]   bus_type    1553总线类型
 * @param[in]   bus_lid     1553总线的逻辑标识符
 * @param[in]   pos     三级索引表的表项号
 * @return      二级索引表指针
 */
void* get_s_index_list(byte_8* bus_type,byte_8* bus_lid);
/** 
 * 获取三级索引表表项数量
 * @param[in]   p_t_index_list    三级索引表指针
 * @return      表项数量
 */
uint_32 get_t_index_list_len(void* p_t_index_list);
/** 
 * 获取三级索引表表项设备逻辑标识符
 * @param[in]   p_t_index_list    三级索引表指针
 * @param[in]   pos    三级索引表的表项号
 * @param[out]  p_dev_lid    存放设备逻辑标识符的地址
 */
void get_t_index_node_lid(void* p_t_index_list,uint_32 pos,byte_8** p_dev_lid);
/** 
 * 获取二级索引表表项RT逻辑标识符
 * @param[in]   p_t_index_list    二级索引表指针
 * @param[in]   pos    二级索引表的表项号
 * @param[out]  p_RT_lid    存放RT逻辑标识符的地址
 */
void get_s_index_node_lid(void* p_s_index_list,uint_32 pos,byte_8** p_RT_lid);
/** 
 * 设置三级索引表项操作缓冲区的指针
 * @param[in]   p_t_index_node    二级索引表项指针
 * @param[in]   pos    是否是读区域,true，读区域，false，写区域
 * @param[in]   e_p    尾指针
 * @param[in]   s_p    首指针
 * @param[in]   w_p    活动指针
 */
void set_index_node_pointer(void* p_t_index_node,bool isRead,void* e_p,void* s_p,void* w_p);
#endif

