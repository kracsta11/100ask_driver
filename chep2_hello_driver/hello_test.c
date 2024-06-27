#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define  HELLO_DEV  /dev/hello
int main(int argc, char **argv)
{
    int  fd;
    char str[1024];
    int  length;
    /*
    * 参数判断
    */
    if(argc != 2 && argc != 3){
        printf("error num of args!\r\n");
        return -1;
    }

    /*
    * 打开文件
    */
    fd = open("HELLO_DEV", O_RDWR);
    if(fd == -1){
        printf("open dev error!\r\n");
        return -1;
    }

    /*
    * 开始读写
    */
    if((0 == strcmp(argv[1], "-w")) && argc == 3){
        length = strlen(argv[2]) + 1;
        length = length < 1024 ? length : 1024;
        write(fd, argv[2], length);
    }else{
        length = read(fd, str, 1024);		
		str[1023] = '\0';
		printf("APP read : %s\n", str);
    }

    close(fd);
    return 0;
}