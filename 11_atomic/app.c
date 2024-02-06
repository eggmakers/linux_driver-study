#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;                     // 定义 int 类型的文件描述符
    char str1[10] = {0};        // 定义读取缓冲区 str1
    fd = open(argv[1], O_RDWR); // 调用 open 函数，打开输入的第一个参数文件，权限为可读可写
    if (fd < 0)
    {
        printf("file open failed \n");
        return -1;
    }
    /*如果第二个参数为 topeet，条件成立，调用 write 函数，写入 topeet*/
    if (strcmp(argv[2], "topeet") == 0)
    {
        write(fd, "topeet", 10);
    }
    /*如果第二个参数为 itop，条件成立，调用 write 函数，写入 itop*/
    else if (strcmp(argv[2], "itop") == 0)
    {
        write(fd, "itop", 10);
    }
    close(fd);
    return 0;
}
