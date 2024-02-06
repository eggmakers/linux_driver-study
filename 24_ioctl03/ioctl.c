#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#define TIME_OPEN _IO('L', 0)
#define TIME_CLOSE _IO('L', 1)
#define TIME_SET _IOW('L', 2, int)

int main(int argc, char *argv[])
{
    int fd;                               // 定义 int 类型的文件描述符 fd
    fd = open("/dev/test", O_RDWR, 0777); // 打开 test 设备节点
    if (fd < 0)
    {
        printf("file open fail\n");
    }
    ioctl(fd, TIME_SET, 1000);
    ioctl(fd, TIME_OPEN);
    sleep(3);
    ioctl(fd, TIME_SET, 3000);
    sleep(7);
    ioctl(fd, TIME_CLOSE);
    close(fd);
}