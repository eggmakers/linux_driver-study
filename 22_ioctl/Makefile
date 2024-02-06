# export ARCH=arm64
# export CROSS_COMPILE?=/home/topeet/Linux/rk356x_linux/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
obj-m += ioctl_dev.o
KDIR := /lib/modules/5.4.0-113-generic/build
# ARCH?=arm64
# CROSS_COMPILE?=/home/topeet/Linux/rk356x_linux/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

PWD ?= $(shell pwd)
all:
		make -C $(KDIR) M=$(PWD) modules
clean:
		rm -f *.ko *.o *.mod.c *.symvers *.order
