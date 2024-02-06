#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#define CMD_TEST1 _IOW('L', 1, int)

struct args
{
    int a;
    int b;
    int c;
};

int main(int argc, char *argv[])
{
    int fd; // 定义 int 类型的文件描述符 fd
    struct args test;
    test.a = 1;
    test.b = 2;
    test.c = 3;
    fd = open("/dev/test", O_RDWR, 0777); // 打开 test 设备节点
    if (fd < 0)
    {
        printf("file open fail\n");
    }
    ioctl(fd, CMD_TEST1, &test);
    close(fd);
}