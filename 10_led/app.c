#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd;
    char buf[32] = {0};
    fd = open("/dev/test", O_RDWR); // 打开 led 驱动
    if (fd < 0)
    {
        perror("open error \n");
        return fd;
    }
    // atoi()将字符串转为整型，这里将第一个参数转化为整型后，存放在 buf[0]中
    buf[0] = atoi(argv[1]);
    write(fd, buf, sizeof(buf)); // 向/dev/test 文件写入数据
    close(fd);                   // 关闭文件
    return 0;
}
