#include "compile_type.h"
#include "app.h"
#include "manage_transport_center.h"
#include<string.h>
#include <unistd.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include "relevant_struct_def.h"
#ifdef __GCC_C99
#include<pthread.h>
#elif __SPARC_GCC_MMU
#include<fsu_pthread.h>
#endif
#include "interval.h"

void* app_write_pthread_func(void* argc){
    uint_32 times=10;
    while(times--){
        byte_8 write_buf[200]="wang ke wei,ni de jia zai na li?";
        byte_8 write_buf_s[200]="yu mei jie,ni xi huan chi shen me?";
        uint_32 write_size=0;
        write_data("001",write_buf,200,&write_size);  //大小重设
        printf("\n-------写数据---------\n");
        printf("write_size:%d\n",write_size);
        if(write_size==strlen(write_buf))printf("write success!\ndev_lid:%s   data:%s\n","001",write_buf);
        else printf("write error!\n");
        printf("----------------------\n");
        
        write_data("003",write_buf_s,200,&write_size);//大小重设
        printf("\n-------写数据---------\n");
        printf("write_size:%d\n",write_size);
        if(write_size==strlen(write_buf_s))printf("write success!\ndev_lid:%s   data:%s\n","003",write_buf_s);
        else printf("write error!\n");
        printf("----------------------\n");
        sleep_ms(1000);
    }
}
void str_to_float_64(float_64* data,byte_8* str){
    float_64 d1=str[0];
    float_64 d2=str[1];
    float_64 d3=str[2];
    float_64 d=d1+pow(0.1,d3)*d2;
    *data = d;
}
void* app_read_pthread_func_f(void* argc){
#ifdef PTHREAD_RECYCLE
    pthread_detach(pthread_self());
#endif
    ubyte_8 read_buf[200];
    ubyte_8 write_buf[200];
    uint_32 read_size;
    uint_32 write_size;
    float_64 sum;
    float_64 d1;
    float_64 d2;
    timeStamp t;
    while(true){
        memset(read_buf,0,200);
        while(true){
            vi_app_read_data("001",read_buf,200,&read_size,&t,AUTO,-1);
            if(read_size!=0)break;
        }
        str_to_float_64(&d1,read_buf);
        memset(read_buf,0,200);
        while(true){
            vi_app_read_data("002",read_buf,200,&read_size,&t,AUTO,-1);
            if(read_size!=0)break;
        }
        str_to_float_64(&d2,read_buf);
        sum=d1+d2;
        uint_32 s1=(uint_32)sum;
        uint_32 s2=(uint_32)((sum-s1)*1000);
        uint_32 s3=3; 
        write_buf[0]=s1;
        write_buf[1]=s2;
        write_buf[2]=s3;
        write_buf[3]='\0';
        vi_app_write_data("003",write_buf,3,&write_size);
        if(write_size==0)printf("写错误\n");
        printf("位置：APP；类型：发送；数据：%lf；大小：%d；端口：%d  ",sum,write_size,8003);
        print_time(&t);
    }
}

void* app_read_pthread_func_s(void* argc){
#ifdef PTHREAD_RECYCLE
    pthread_detach(pthread_self());
#endif
    ubyte_8 read_buf[200];
    ubyte_8 write_buf[200];
    uint_32 read_size;
    uint_32 write_size;
    uint_32 sum;
    timeStamp t;
    while(true){
        memset(read_buf,0,200);
        while(true){
            vi_app_read_data("004",read_buf,200,&read_size,&t,AUTO,-1);
            if(read_size!=0)break;
        }
        sum=read_buf[0]+1;
        write_buf[0]=sum;
        write_buf[1]='\0';
        vi_app_write_data("004",write_buf,1,&write_size);
        if(write_size==0)printf("写错误\n");
        printf("位置：APP；类型：发送；数据：%d；大小：%d；端口：%d  ",sum,write_size,8004);
        print_time(&t);
    }
}

void* app_read_pthread_func_t(void* argc){
#ifdef PTHREAD_RECYCLE
    pthread_detach(pthread_self());
#endif
    ubyte_8 read_buf[200];
    ubyte_8 write_buf[200];
    uint_32 read_size;
    uint_32 write_size;
    uint_32 sum;
    timeStamp t;
    while(true){
        memset(read_buf,0,200);
        while(true){
            vi_app_read_data("005",read_buf,200,&read_size,&t,AUTO,-1);
            if(read_size!=0)break;
        }
        sum=read_buf[0];
        memset(read_buf,0,200);
        while(true){
            vi_app_read_data("006",read_buf,200,&read_size,&t,AUTO,-1);
            if(read_size!=0)break;
        }
        sum+=read_buf[0];
        memset(read_buf,0,200);
        while(true){
            vi_app_read_data("007",read_buf,200,&read_size,&t,AUTO,-1);
            if(read_size!=0)break;
        }
        sum+=read_buf[0];
        write_buf[0]=sum;
        write_buf[1]='\0';
        vi_app_write_data("007",write_buf,1,&write_size);
        if(write_size==0)printf("写错误\n");
        vi_app_write_data("008",write_buf,1,&write_size);
        if(write_size==0)printf("写错误\n");
        printf("位置：APP；类型：发送；数据：%d；大小：%d；端口：%d  ",sum,write_size,8007);
        print_time(&t);
        printf("位置：APP；类型：发送；数据：%d；大小：%d；端口：%d  ",sum,write_size,8008);
        print_time(&t);
    }
    
}
void app_read(void){
    vi_set_RT_disable("001","02");
    vi_set_RT_enable("001","02");
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
#ifdef __GCC_C99
    pthread_create(&tid1,NULL,app_read_pthread_func_f,NULL);
    pthread_create(&tid2,NULL,app_read_pthread_func_s,NULL);
    pthread_create(&tid3,NULL,app_read_pthread_func_t,NULL);
#elif __SPARC_GCC_MMU
    pthread_create(&tid1,NULL,(pthread_func_t)app_read_pthread_func_f,NULL);
    pthread_create(&tid2,NULL,(pthread_func_t)app_read_pthread_func_s,NULL);
    pthread_create(&tid3,NULL,(pthread_func_t)app_read_pthread_func_t,NULL);
#endif
}
