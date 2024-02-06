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
    char buf2[32] = "nihao";
    fd = open("/dev/test", O_RDWR | O_NONBLOCK); // 调用 open 函数，打开输入的第一个参数文件，权限为可读可写
    if (fd < 0)
    {
        printf("file open failed \n");
        return fd;
    }
    printf("write before\n");
    write(fd, buf2, sizeof(buf2));
    printf("write after\n");
    close(fd);
    return 0;
}
