#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/moduleparam.h>

static int major;
static int minor;

module_param(major, int, S_IRUGO);
module_param(minor, int, S_IRUGO);
static dev_t dev_num;

static int __init dev_t_init(void) // 驱动入口函数
{
    int ret;
    if (major)
    {
        dev_num = MKDEV(major, minor);
        printk("major is %d\n", major);
        printk("minor is %d\n", minor);
        ret = register_chrdev_region(dev_num, 1, "chrdev_name");
        if (ret < 0)
        {
            printk("register_chrdev_region failed\n");
        }
        printk("register_chrdev_region is OK\n");
    }
    else
    {
        ret = alloc_chrdev_region(&dev_num, 0, 1, "chrdev_name");
        if (ret < 0)
        {
            printk("alloc_chrdev_region failed\n");
        }
        printk("alloc_chrdev_region is OK\n");
        major = MAJOR(dev_num);
        minor = MINOR(dev_num);
        printk("major is %d\n", major);
        printk("minor is %d\n", minor);
    }
    return 0;
}

static void __exit __exit dev_t_exit(void)
{
    unregister_chrdev_region(dev_num, 1);
    printk("unregister_chrdev_region is OK\n");
}

module_init(dev_t_init);
module_exit(dev_t_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zr");
MODULE_VERSION("0.1.0");