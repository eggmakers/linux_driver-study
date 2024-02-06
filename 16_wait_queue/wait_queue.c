#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/wait.h>

struct device_test
{
    dev_t dev_num;
    int major;
    int minor;
    struct cdev cdev_test;
    struct class *class;
    struct device *device;
    char kbuf[32];
    int flag;
};
struct device_test dev1;

DECLARE_WAIT_QUEUE_HEAD(read_wq);

static int cdev_test_open(struct inode *inode, struct file *file)
{
    file->private_data = &dev1;
    printk("This is cdev_test_open\r\n");
    return 0;
}

static ssize_t cdev_test_write(struct file *file, const char __user *buf, size_t size, loff_t *off)
{
    struct device_test *test_dev = (struct device_test *)file->private_data;

    if (copy_from_user(test_dev->kbuf, buf, size) != 0)
    {
        printk("copy_from_user error\r\n");
        return -1;
    }
    test_dev->flag = 1;
    wake_up_interruptible(&read_wq);

    return 0;
}

static ssize_t cdev_test_read(struct file *file, char __user *buf, size_t size, loff_t *off)
{
    struct device_test *test_dev = (struct device_test *)file->private_data;

    wait_event_interruptible(read_wq, test_dev->flag);
    if (copy_to_user(buf, test_dev->kbuf, strlen(test_dev->kbuf)) != 0)
    {
        printk("copy_to_user error\r\n");
        return -1;
    }
    return 0;
}

static int cdev_test_release(struct inode *inode, struct file *file)
{
    return 0;
};

struct file_operations cdev_test_fops =
    {
        .owner = THIS_MODULE,
        .open = cdev_test_open,
        .read = cdev_test_read,
        .write = cdev_test_write,
        .release = cdev_test_release,
};

static int __init chr_fops_init(void)
{
    /*注册字符设备驱动*/
    int ret;
    /*1 创建设备号*/
    ret = alloc_chrdev_region(&dev1.dev_num, 0, 1, "alloc_name"); // 动态分配设备号
    if (ret < 0)
    {
        goto err_chrdev;
    }
    printk("alloc_chrdev_region is ok\n");
    dev1.major = MAJOR(dev1.dev_num);       // 获取主设备号
    dev1.minor = MINOR(dev1.dev_num);       // 获取次设备号
    printk("major is %d \r\n", dev1.major); // 打印主设备号
    printk("minor is %d \r\n", dev1.minor); // 打印次设备号
    /*2 初始化 cdev*/
    dev1.cdev_test.owner = THIS_MODULE;
    cdev_init(&dev1.cdev_test, &cdev_test_fops);
    /*3 添加一个 cdev,完成字符设备注册到内核*/
    ret = cdev_add(&dev1.cdev_test, dev1.dev_num, 1);
    if (ret < 0)
    {
        goto err_chr_add;
    }
    /*4 创建类*/
    dev1.class = class_create(THIS_MODULE, "test");
    if (IS_ERR(dev1.class))
    {
        ret = PTR_ERR(dev1.class);
        goto err_class_create;
    }
    /*5 创建设备*/
    dev1.device = device_create(dev1.class, NULL, dev1.dev_num, NULL, "test");
    if (IS_ERR(dev1.device))
    {
        ret = PTR_ERR(dev1.device);
        goto err_device_create;
    }
    return 0;
err_device_create:
    class_destroy(dev1.class); // 删除类
err_class_create:
    cdev_del(&dev1.cdev_test); // 删除 cdev
err_chr_add:
    unregister_chrdev_region(dev1.dev_num, 1); // 注销设备号
err_chrdev:
    return ret;
}
static void __exit chr_fops_exit(void) // 驱动出口函数
{
    /*注销字符设备*/
    unregister_chrdev_region(dev1.dev_num, 1); // 注销设备号
    cdev_del(&dev1.cdev_test);                 // 删除 cdev
    device_destroy(dev1.class, dev1.dev_num);  // 删除设备
    class_destroy(dev1.class);                 // 删除类
}
module_init(chr_fops_init);
module_exit(chr_fops_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("topeet");
