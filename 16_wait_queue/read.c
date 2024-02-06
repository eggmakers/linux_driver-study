#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    char buf1[32] = {0};
    char buf2[32] = {0};
    fd = open("/dev/test", O_RDWR); // 调用 open 函数，打开输入的第一个参数文件，权限为可读可写
    if (fd < 0)
    {
        printf("file open failed \n");
        return fd;
    }
    printf("read before\n");
    read(fd, buf1, sizeof(buf1));
    printf("buf is %s\n", buf1);
    printf("read after\n");
    close(fd);
    return 0;
}
