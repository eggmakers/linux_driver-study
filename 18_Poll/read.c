#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

int main(int argc, char *argv[])
{
    int fd;
    char buf1[32] = {0};
    char buf2[32] = {0};
    struct pollfd fds[1];
    int ret;
    fd = open("/dev/test", O_RDWR | O_NONBLOCK); // 调用 open 函数，打开输入的第一个参数文件，权限为可读可写

    if (fd < 0)
    {
        printf("file open failed \n");
        return fd;
    }

    fds[0].fd = fd;
    fds[0].events = POLLIN;
    printf("read before\n");
    while (1)
    {
        ret = poll(fds, 1, 3000); // 轮询文件是否可操作，超时 3000ms
        if (!ret)
        { // 超时
            printf("time out !!\n,");
        }
        else if (fds[0].revents == POLLIN) // 如果返回事件是有数据可读取
        {
            read(fd, buf1, sizeof(buf1));  // 从/dev/test 文件读取数据
            printf("buf is %s \n,", buf1); // 打印读取的数据
            sleep(1);
        }
    }
    printf("read after\n");
    close(fd);
    return 0;
}
