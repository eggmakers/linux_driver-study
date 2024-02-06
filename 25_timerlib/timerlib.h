#ifndef _TIMERLIB_H_
#define _TIMERLIB_H_
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "unistd.h"
#include "sys/ioctl.h"

#define TIME_OPEN _IO('L', 0)
#define TIME_CLOSE _IO('L', 1)
#define TIME_SET _IOW('L', 2, int)

int dev_open();
int timer_open(int fd);
int timer_close(int fd);
int timer_set(int fd, int arg);

#endif