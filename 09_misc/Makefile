export ARCH=arm64
export CROSS_COMPILE=/home/topeet/Linux/rk356x_linux/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
obj-m += misc.o
KDIR:=/home/topeet/Linux/rk356x_linux/kernel
PWD?=$(shell pwd)

all:
		make -C $(KDIR) M=$(PWD) modules
		echo $(PWD)
clean:
		rm -f *.ko *.o *.mod.c *.symvers *.order