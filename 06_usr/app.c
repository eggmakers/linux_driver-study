#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    char read_buf[32] = {0};
    char write_buf[32] = "nihao";
    fd = open("/dev/test", O_RDWR);
    if (fd < 0)
    {
        perror("open error \n");
        return fd;
    }
    read(fd, read_buf, sizeof(read_buf));
    printf("read_buf is %s\n", read_buf);

    write(fd, write_buf, sizeof(write_buf));

    close(fd);
    return 0;
}
