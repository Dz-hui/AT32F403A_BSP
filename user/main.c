#include "main.h"
#include "diskio.h"
#include "ff.h"


FATFS fs; /* 文件系统对象 */
FIL fnew; /* 文件对象 */
FRESULT res_flash; /* 文件操作结果 */
UINT fnum; /* 文件成功读取数量 */
BYTE buffer[1024];
BYTE textfilebuffer[] = "Welcome here and wish you luck forever! This is the test program\r\n";
BYTE work[FF_MAX_SS];

int main(void){

    system_clock_config();
    bsp_uart_init();

    printf("hardware init complete\n");

    res_flash = f_mount(&fs,"0:",1); /* 挂载文件系统 */
    if(res_flash == FR_NO_FILESYSTEM){

        res_flash = f_mkfs("0:",0,work,sizeof(work));

        if(res_flash == FR_OK){
            f_mount(NULL,"0:",1);/* 取消挂载 */
            res_flash = f_mount(&fs,"0:",1);/* 重新挂载 */
        }
        else{
            printf("Format failed\n");
        }
    }
    else if(res_flash != FR_OK){
        printf("filesystem mount failed\n");
    }
    else{
        printf("file system mounted successfully\n");
    }

    res_flash = f_open(&fnew,"0:file.txt",FA_READ | FA_WRITE | FA_CREATE_ALWAYS);

    if(res_flash == FR_OK){
        res_flash = f_write(&fnew,textfilebuffer,sizeof(textfilebuffer),&fnum);

        if(res_flash == FR_OK) printf("write file successfully\n");
        else printf("write file failed\n");

        f_close(&fnew);
    }
    else{
        printf("Open file failed\n");
    }

    res_flash = f_open(&fnew,"0:file.txt",FA_OPEN_EXISTING|FA_READ);
    

    if(res_flash == FR_OK){
        res_flash = f_read(&fnew,buffer,sizeof(buffer),&fnum);

        if(res_flash == FR_OK){
            printf("read file successfully\n");
            printf("Read file data as: \n%s",buffer);
        } 
        else printf("read file failed\n");
    }
    else printf("Open file failed\n");

    f_close(&fnew);
    f_mount(NULL,"0:",1);

    while(1){	
		
    }
}

