#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/device.h>

dev_t dev_num;
static int major;
static int minor;
struct cdev cdev_test;

struct class *class;
struct device *device;

static int cdev_test_open(struct inode *inode, struct file *file)
{
    printk("This is a cdev_test_open\n");
    return 0;
}

static ssize_t cdev_test_read(struct file *file, char __user *buf, size_t size, loff_t *off)
{
    printk("This is a cdev_test_read\n");
    return 0;
}

static ssize_t cdev_test_write(struct file *file, const char __user *buf, size_t size, loff_t *off)
{
    printk("This is a cdev_test_write\n");
    return 0;
}

static int cdev_tes_release(struct inode *inode, struct file *file)
{
    printk("This is a cdev_tes_release\n");
    return 0;
}

struct file_operations cdev_test_ops =
    {
        .owner = THIS_MODULE,
        .open = cdev_test_open,
        .read = cdev_test_read,
        .write = cdev_test_write,
        .release = cdev_tes_release};

static int file_init(void) // 驱动入口函数
{
    int ret = alloc_chrdev_region(&dev_num, 0, 1, "alloc_name");

    if (ret < 0)
    {
        printk("register_chrdev_region failed\n");
    }
    printk("register_chrdev_region is OK\n");

    major = MAJOR(dev_num);
    minor = MINOR(dev_num);
    printk("major is %d\n", major);
    printk("minor is %d\n", minor);

    cdev_test.owner = THIS_MODULE;
    cdev_init(&cdev_test, &cdev_test_ops);
    cdev_add(&cdev_test, dev_num, 1);

    class = class_create(THIS_MODULE, "test");
    device = device_create(class, NULL, dev_num, NULL, "test");

    return 0;
}

static void __exit __exit file_exit(void)
{
    unregister_chrdev_region(dev_num, 1);
    cdev_del(&cdev_test);

    device_destroy(class, dev_num);
    class_destroy(class);

    printk("bye bye\n");
}

module_init(file_init);
module_exit(file_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zr");
MODULE_VERSION("0.1.0");