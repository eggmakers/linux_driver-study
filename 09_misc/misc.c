#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>

static int cdev_test_open(struct inode *inode, struct file *file)
{
    printk("This is a cdev_test_open\n");
    return 0;
}

static ssize_t cdev_test_read(struct file *file, char __user *buf, size_t size, loff_t *off)
{
    return 0;
}

static ssize_t cdev_test_write(struct file *file, const char __user *buf, size_t size, loff_t *off)
{
    return 0;
}

static int cdev_tes_release(struct inode *inode, struct file *file)
{
    return 0;
}

struct file_operations cdev_test_ops =
    {
        .owner = THIS_MODULE,
        .open = cdev_test_open,
        .read = cdev_test_read,
        .write = cdev_test_write,
        .release = cdev_tes_release};

struct miscdevice misc_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "misc_test",
    .fops = &cdev_test_ops,
};

static int misc_init(void) // 驱动入口函数
{
    int ret;

    ret = misc_register(&misc_dev);
    if (ret < 0)
    {
        printk("misc_register\n");
        return -1;
    }
    return 0;
}

static void __exit __exit misc_exit(void)
{
    misc_deregister(&misc_dev);
    printk("bye bye\n");
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zr");
MODULE_VERSION("0.1.0");