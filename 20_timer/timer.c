#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    int count;
    fd = open("/dev/test", O_RDWR); // 调用 open 函数，打开输入的第一个参数文件，权限为可读可写
    while (1)
    {
        read(fd, &count, sizeof(count));
        sleep(1);
        printf("num is %d\n", count);
    }
    return 0;
}
