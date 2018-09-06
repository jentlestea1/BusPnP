#ifndef CAN_TRANSMIT_H
#define CAN_TRANSMIT_H
#include "compile_type.h"
#ifdef __VCAN_TRANSMIT
#include "m_type.h"
void vcan_receive_package(ubyte_8 *buf,uint_32 r_size,uint_32 *size);
int vcan_send_package(ubyte_8 *buf,uint_32 size);
#endif
#endif

