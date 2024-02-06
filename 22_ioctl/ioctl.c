#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#define CMD_TEST0 _IO('L', 0)
#define CMD_TEST1 _IOW('L', 1, int)
#define CMD_TEST2 _IOR('L', 2, int)
int main(int argc, char *argv[])
{
    int fd;                         // 定义 int 类型的文件描述符 fd
    int val;                        // 定义 int 类型的传递参数 val
    fd = open("/dev/test", O_RDWR); // 打开 test 设备节点
    if (fd < 0)
    {
        printf("file open fail\n");
    }
    if (!strcmp(argv[1], "write"))
    {
        ioctl(fd, CMD_TEST1, 1); // 如果第二个参数为 write，向内核空间写入 1
    }
    else if (!strcmp(argv[1], "read"))
    {
        ioctl(fd, CMD_TEST2, &val); // 如果第二个参数为 read，则读取内核空间传递向用户空间传递的值
        printf("val is %d\n", val);
    }
    close(fd);
}