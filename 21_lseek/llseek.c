#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    int fd;                              // 定义 int 类型文件描述符
    unsigned int off;                    // 定义读写偏移位置
    char readbuf[13] = {0};              // 定义读取缓冲区 readbuf
    char readbuf1[19] = {0};             // 定义读取缓冲区 readbuf1
    fd = open("/dev/test", O_RDWR, 666); // 打开/dev/test 设备
    if (fd < 0)
    {
        printf("file open error \n");
    }
    write(fd, "hello world", 13); // 向 fd 写入数据 hello world
    off = lseek(fd, 0, SEEK_CUR); // 读取当前位置的偏移量
    printf("off is %d\n", off);
    off = lseek(fd, 0, SEEK_SET); // 将偏移量设置为 0
    printf("off is %d\n", off);
    read(fd, readbuf, sizeof(readbuf)); // 将写入的数据读取到 readbuf 缓冲区
    printf("read is %s\n", readbuf);
    off = lseek(fd, 0, SEEK_CUR); // 读取当前位置的偏移量
    printf("off is %d\n", off);
    off = lseek(fd, -1, SEEK_CUR); // 将当前位置的偏移量向前挪动一位
    printf("off is %d\n", off);
    write(fd, "Linux", 6);        // 向 fd 写入数据 Linux
    off = lseek(fd, 0, SEEK_CUR); // 读取当前位置的偏移量
    printf("off is %d\n", off);
    off = lseek(fd, 0, SEEK_SET); // 将偏移量设置为 0
    printf("off is %d\n", off);
    read(fd, readbuf1, sizeof(readbuf1)); // 将写入的数据读取到 readbuf1 缓冲区
    printf("read is %s\n", readbuf1);
    off = lseek(fd, 0, SEEK_CUR); // 读取当前位置的偏移量
    printf("off is %d\n", off);
    close(fd);
    return 0;
}
