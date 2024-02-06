#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/atomic.h>

struct device_test
{
    dev_t dev_num;
    int major;
    int minor;
    struct cdev cdev_test;
    struct class *class;
    struct device *device;
    int sec;
};

atomic64_t v = ATOMIC_INIT(0);
static struct device_test dev1;
static void function_test(struct timer_list *t);
DEFINE_TIMER(timer_test, function_test);

static void function_test(struct timer_list *t)
{
    atomic64_inc(&v);
    dev1.sec = atomic_read(&v);
    printk("this is function test\n");
    mod_timer(&timer_test, jiffies_64 + msecs_to_jiffies(1000));
}

static int cdev_test_open(struct inode *inode, struct file *file)
{
    file->private_data = &dev1;
    add_timer(&timer_test);
    return 0;
}

static ssize_t cdev_test_read(struct file *file, char __user *buf, size_t size, loff_t *off)
{
    if (copy_to_user(buf, &dev1.sec, sizeof(dev1.sec)))
    {
        printk("copy to user error\n");
        return -1;
    }
    return 0;
}

static int cdev_test_release(struct inode *inode, struct file *file)
{
    del_timer(&timer_test);
    return 0;
}

struct file_operations cdev_test_fops = {
    .owner = THIS_MODULE,
    .open = cdev_test_open,
    .read = cdev_test_read,
    .release = cdev_test_release,
};

static int __init timer_dev_init(void)
{
    int ret;
    ret = alloc_chrdev_region(&dev1.dev_num, 0, 1, "alloc_name");
    if (ret < 0)
    {
        goto err_chrdev;
    }
    printk("alloc_chrdev_region is ok\n");

    dev1.major = MAJOR(dev1.dev_num);
    dev1.minor = MINOR(dev1.dev_num);

    dev1.cdev_test.owner = THIS_MODULE;
    cdev_init(&dev1.cdev_test, &cdev_test_fops);
    if (ret < 0)
    {
        goto err_chr_add;
    }

    dev1.class = class_create(THIS_MODULE, "test");
    if (IS_ERR(dev1.class))
    {
        ret = PTR_ERR(dev1.class);
        goto err_class_create;
    }

    dev1.device = device_create(dev1.class, NULL, dev1.dev_num, NULL, "test");
    if (IS_ERR(dev1.device))
    {
        ret = PTR_ERR(dev1.device);
        goto err_device_create;
    }
    return 0;

err_device_create:
    class_destroy(dev1.class);

err_class_create:
    cdev_del(&dev1.cdev_test);

err_chr_add:
    unregister_chrdev_region(dev1.dev_num, 1);

err_chrdev:
    return ret;
}

static void __exit timer_dev_exit(void)
{
    unregister_chrdev_region(dev1.dev_num, 1); // 注销设备号
    cdev_del(&dev1.cdev_test);                 // 删除 cdev
    device_destroy(dev1.class, dev1.dev_num);  // 删除设备
    class_destroy(dev1.class);                 // 删除类
}

module_init(timer_dev_init);
module_exit(timer_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zr");
MODULE_VERSION("0.1.0");