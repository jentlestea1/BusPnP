#include "compile_type.h"
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "manage_transport_center.h"
#include "app.h"
#include "RT_socket.h"
#include "file.h"
#ifdef __GCC_C99
#include <pthread.h>
#elif __SPARC_GCC_MMU
#include <fsu_pthread.h>
#include "sync_collect.h"
#endif
/** 
 * 第一个应用层操作函数,接收8001,8002端口数据，计算结果发送给8003
 * @param[in]   argc    创建线程时传入的参数 
 */
int main(){
#ifdef __GCC_C99
#elif __SPARC_GCC_MMU
    pthread_init();
#endif
    init_vi_dev_visit_sys();
    app_read();
    while(1)
        sleep(30);
    return 0;
}
