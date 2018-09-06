#include "compile_type.h"
#ifdef __VCAN_TRANSMIT
#include "m_type.h"
#include "vcan_frame.h"
#include "handle_event.h"


int frame_type_detect(struct can_frame frame,ubyte_8* w_buf,uint_32 *size,bool size_frame_mask){
    if(frame.can_dlc<=0||frame.can_dlc>8){
        *size=0;
        return VCAN_INVALID_FRAME_FLAG;
    }
    if(*(uint_32 *)frame.data==PACKAGE_HEADER&&size_frame_mask==false){
        if(frame.can_dlc!=8)
            return VCAN_INVALID_FRAME_FLAG;
        else{
            uint_32 a=*(int *)(frame.data+4);
            a=htonl(a);  //两端大小端不一样的时候需要转换
            *size=a;
            return VCAN_SIZE_FRAME_FLAG;
        }
    }
    else{
        if(w_buf==NULL)return VCAN_DATA_FRAME_FLAG;
        int i=0;
        for(i=0;i<frame.can_dlc;i++){
            w_buf[i]=*(frame.data+i);
        }
        *size=frame.can_dlc;
        return VCAN_DATA_FRAME_FLAG;
    }
}
struct can_frame serial_frame(int serial_type,ubyte_8* r_buf,uint_32 size){
    struct can_frame frame;
    frame.can_id=1;
    if(serial_type==VCAN_INIT_PORT_FRAME_FLAG){
        frame.data[0]=0x0;
        frame.data[1]=0xff;
        frame.can_dlc=0x2;
    }
    else if(serial_type==VCAN_DATA_FRAME_FLAG){
        if(size<0||size>8)
            return frame;
        else{
            if(r_buf==NULL){
                throw_event(0,NULL,EVT_INVALID_SERIAL_FRAME);
                return frame;
            }
            int i=0;
            for(i=0;i<size;i++){
                frame.data[i]=*(r_buf+i);
            }
            frame.can_dlc=size;
        }
    }
    else if(serial_type==VCAN_SIZE_FRAME_FLAG){
        *(uint_32 *)frame.data=PACKAGE_HEADER;
        *(uint_32 *)(frame.data+4)=size;
        frame.can_dlc=8;
    }
    else{
        throw_event(0,NULL,EVT_INVALID_SERIAL_FRAME);
    }
    return frame;
}
void clear_frame_data(ubyte_8* p){
    int i=0;
    for(i=0;i<8;i++){
        *(p+i)=0;
    }
}

#endif
