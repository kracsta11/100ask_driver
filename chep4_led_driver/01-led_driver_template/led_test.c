#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define  HELLO_DEV  "/dev/my_led"
int main(int argc, char **argv)
{
    int  fd;
    char status;
    /*
    * 参数判断
    */
    if( argc != 3){
        printf("error num of args!\r\n");
        return -1;
    }

    /*
    * 打开文件
    */
    fd = open(argv[1], O_RDWR);
    if(fd == -1){
        printf("open dev error!\r\n");
        return -1;
    }

    /*
    * 开始读写
    */
    if((0 == strcmp(argv[2], "on"))){
        status = 1;
        write(fd, &status, 1);
        printf("led on!\r\n");
    }else{
        status = 0;
        write(fd, &status, 1);
        printf("led off!\r\n");
    }

    close(fd);
    return 0;
}