/*
 *使用互斥同步锁来完成带阻塞类型的接口
 */

//vcan接口目前只支持在qemu上运行

#include "compile_type.h"
#ifdef __VCAN_TRANSMIT
#include "can_transmit.h"
#include "can_sys.h"
#include <fsu_pthread.h>
#include "m_type.h"
#include "interval.h"
#include "handle_event.h"
#include "vcan_frame.h"

//can_dev *device_recv;
//can_dev *device_send;

//can_dev *device;//
can_dev *device_send;
can_dev *device_recv;
volatile int init_flag=0;


void canIrq_handler(){
	uint8_t temp;
    //发中断不做操作
	temp = can_read8(device_recv->mem_base + SJA_INT_REG); 
	
	if (temp & RI) {
		if((device_recv->rx_in+1)%BUFFERSIZE == device_recv->rx_out) {
   			can_write8(0x04, device_recv->mem_base + SJA_COMMAND_REG);// Release receive buffer.
            throw_event(0,NULL,EVT_VCAN_RECV_BUF_OVERFLOW);
            return;   	 // No buffer, ignore this one.
		}
		receive_message(device_recv->mem_base, &(device_recv->rx_buf[device_recv->rx_in]));
		device_recv->rx_in++;
		if (device_recv->rx_in >= BUFFERSIZE)
			device_recv->rx_in = 0;
	}

	if (temp & DOI) {
        throw_event(0,NULL,EVT_VCAN_IRQ_OVERRUN);
		can_write8(CDO, device_recv->mem_base + SJA_COMMAND_REG); // Clear data overrun.
	}

}

void init_device(){
    if(init_flag==1) return;
    else {
        device_send = (can_dev *)malloc(sizeof(can_dev));
        device_recv = (can_dev *)malloc(sizeof(can_dev));
	    memset(device_send, 0, sizeof(can_dev));
	    memset(device_recv, 0, sizeof(can_dev));
	    device_send->mem_base = (addrptr)0x80005000;
	    device_recv->mem_base = (addrptr)0x80005000;
        device_open(device_send);
        device_open(device_recv);
        catch_interrupt(canIrq_handler,CANSYS_IRQ_NO);  //捕获中断
        enable_irq(CANSYS_IRQ_NO);
        init_flag=1;
    }
}

struct can_frame vcan_receive_frame(){
    can_frame *frame;
    while(device_recv->rx_in==device_recv->rx_out){
        sleep_ms(20);
    }
    frame = device_read(device_recv);
    return *frame;
}


void vcan_receive_package(ubyte_8 *buf,uint_32 r_size,uint_32 *recv_size){
    init_device();
    int n;
    uint_32 pack_size;
    uint_32 size;
    uint_32 recv_bytes=0;
    struct can_frame frame;
    frame = vcan_receive_frame();
    while(frame_type_detect(frame,NULL,&size,false)!=VCAN_SIZE_FRAME_FLAG){
        frame = vcan_receive_frame();
        throw_event(0,NULL,EVT_VCAN_RECV_SIZE_FRAME_ERR);
    }
    pack_size=size;
    while(recv_bytes<pack_size){
        frame = vcan_receive_frame();
        frame_type_detect(frame,buf+recv_bytes,&size,true);
        recv_bytes+=size;
        if(recv_bytes>=r_size){
            throw_event(0,NULL,EVT_VCAN_RECV_BUF_LIMIT_ERR);
            *recv_size=recv_bytes;
            return;
        }
    }
    if(recv_bytes>pack_size){
        throw_event(0,NULL,EVT_VCAN_RECV_PACKAGE_ERR);
    }
    *recv_size=recv_bytes;
}

int vcan_send_package(ubyte_8 *buf,uint_32 size){
    init_device();
    struct can_frame frame;
	frame=serial_frame(VCAN_SIZE_FRAME_FLAG,NULL,size);
    device_write(device_send,frame);
    int i=0;
    for(i=0;i<size;){
	    uint_32 frame_size_tmp = (size-i)>8?8:(size-i);
        frame=serial_frame(VCAN_DATA_FRAME_FLAG,buf+i,frame_size_tmp);
        i+=frame_size_tmp;
        int last_cnt=device_write(device_send,frame);
        if(last_cnt==0&&i>=size){
            throw_event(0,NULL,EVT_VCAN_SEND_PACKAGE_ERR);
            return -1;
        }
    }
    return 1;
}
#endif

