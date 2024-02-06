#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>

dev_t dev_num;
struct cdev cdev_test;
struct file_operations cdev_test_ops =
    {
        .owner = THIS_MODULE};

static int moduledev_init(void) // 驱动入口函数
{
    int ret = alloc_chrdev_region(&dev_num, 0, 1, "alloc_name");

    if (ret < 0)
    {
        printk("register_chrdev_region failed\n");
    }
    printk("register_chrdev_region is OK\n");

    cdev_test.owner = THIS_MODULE;
    cdev_init(&cdev_test, &cdev_test_ops);
    cdev_add(&cdev_test, dev_num, 1);

    return 0;
}

static void __exit __exit moduledev_exit(void)
{
    unregister_chrdev_region(dev_num, 1);
    cdev_del(&cdev_test);
    printk("unregister_chrdev_region is OK\n");
}

module_init(moduledev_init);
module_exit(moduledev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zr");
MODULE_VERSION("0.1.0");