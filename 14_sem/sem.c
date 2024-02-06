#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/semaphore.h>
struct semaphore semaphore_test; // 定义一个 semaphore 类型的结构体变量 semaphore_test
static int open_test(struct inode *inode, struct file *file)
{
    printk("\nthis is open_test \n");
    down(&semaphore_test); // 信号量数量减 1
    return 0;
}
static ssize_t read_test(struct file *file, char __user *ubuf, size_t len, loff_t *off)
{
    int ret;
    char kbuf[10] = "topeet"; // 定义 char 类型字符串变量 kbuf
    printk("\nthis is read_test \n");
    ret = copy_to_user(ubuf, kbuf, strlen(kbuf)); // 使用 copy_to_user 接收用户空间传递的数据
    if (ret != 0)
    {
        printk("copy_to_user is error \n");
    }
    printk("copy_to_user is ok \n");
    return 0;
}
static char kbuf[10] = {0}; // 定义 char 类型字符串全局变量 kbuf
static ssize_t write_test(struct file *file, const char __user *ubuf, size_t len, loff_t *off)
{
    int ret;
    ret = copy_from_user(kbuf, ubuf, len); // 使用 copy_from_user 接收用户空间传递的数据
    if (ret != 0)
    {
        printk("copy_from_user is error\n");
    }
    if (strcmp(kbuf, "topeet") == 0)
    { // 如果传递的 kbuf 是 topeet 就睡眠四秒钟
        ssleep(4);
    }
    else if (strcmp(kbuf, "itop") == 0)
    { // 如果传递的 kbuf 是 itop 就睡眠两秒钟
        ssleep(2);
    }
    printk("copy_from_user buf is %s \n", kbuf);
    return 0;
}
static int release_test(struct inode *inode, struct file *file)
{
    up(&semaphore_test); // 信号量数量加 1
    printk("\nthis is release_test \n");
    return 0;
}
struct chrdev_test
{
    dev_t dev_num;            // 定义 dev_t 类型变量 dev_num 来表示设备号
    int major, minor;         // 定义 int 类型的主设备号 major 和次设备号 minor
    struct cdev cdev_test;    // 定义 struct cdev 类型结构体变量 cdev_test，表示要注册的字符设备
    struct class *class_test; // 定于 struct class *类型结构体变量 class_test，表示要创建的类
};
struct chrdev_test dev1; // 创建 chrdev_test 类型的
struct file_operations fops_test = {
    .owner = THIS_MODULE,    // 将 owner 字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
    .open = open_test,       // 将 open 字段指向 open_test(...)函数
    .read = read_test,       // 将 read 字段指向 read_test(...)函数
    .write = write_test,     // 将 write 字段指向 write_test(...)函数
    .release = release_test, // 将 release 字段指向 release_test(...)函数
};
static int __init atomic_init(void)
{
    sema_init(&semaphore_test, 1); // 初始化信号量结构体 semaphore_test，并设置信号量的数量为 1
    if (alloc_chrdev_region(&dev1.dev_num, 0, 1, "chrdev_name") < 0)
    { // 自动获取设备号，设备名chrdev_name
        printk("alloc_chrdev_region is error \n");
    }
    printk("alloc_chrdev_region is ok \n");
    dev1.major = MAJOR(dev1.dev_num); // 使用 MAJOR()函数获取主设备号
    dev1.minor = MINOR(dev1.dev_num); // 使用 MINOR()函数获取次设备号
    printk("major is %d,minor is %d\n", dev1.major, dev1.minor);
    cdev_init(&dev1.cdev_test, &fops_test);                            // 使用 cdev_init()函数初始化 cdev_test 结构体，并链接到fops_test 结构体
    dev1.cdev_test.owner = THIS_MODULE;                                // 将 owner 字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
    cdev_add(&dev1.cdev_test, dev1.dev_num, 1);                        // 使用 cdev_add()函数进行字符设备的添加
    dev1.class_test = class_create(THIS_MODULE, "class_test");         // 使用 class_create 进行类的创建，类名称为class_test
    device_create(dev1.class_test, 0, dev1.dev_num, 0, "device_test"); // 使用 device_create 进行设备的创建，设备名称为 device_test
    return 0;
}
static void __exit atomic_exit(void)
{
    device_destroy(dev1.class_test, dev1.dev_num); // 删除创建的设备
    class_destroy(dev1.class_test);                // 删除创建的类
    cdev_del(&dev1.cdev_test);                     // 删除添加的字符设备 cdev_test
    unregister_chrdev_region(dev1.dev_num, 1);     // 释放字符设备所申请的设备号
    printk("module exit \n");
}
module_init(atomic_init);
module_exit(atomic_exit)
    MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("topeet");
