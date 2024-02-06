#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

struct device_test
{
    dev_t dev_num;
    int major;
    int minor;
    struct cdev cdev_test;
    struct class *class;
    struct device *device;
    char kbuf[32];
};
struct device_test dev1;
struct device_test dev2;

static int cdev_test_open(struct inode *inode, struct file *file)
{
    dev1.minor = 0;
    dev2.minor = 1;

    file->private_data = container_of(inode->i_cdev, struct device_test, cdev_test);
    printk("This is a cdev_test_open\n");
    return 0;
}

static ssize_t cdev_test_read(struct file *file, char __user *buf, size_t size, loff_t *off)
{
    struct device_test *test_dev = (struct device_test *)file->private_data;

    if (copy_to_user(buf, test_dev->kbuf, strlen(test_dev->kbuf)) != 0)
    {
        printk("copy_to_user error\n");
        return -1;
    }

    return 0;
}

static ssize_t cdev_test_write(struct file *file, const char __user *buf, size_t size, loff_t *off)
{
    struct device_test *test_dev = (struct device_test *)file->private_data;

    if (test_dev->minor == 0)
    {
        if (copy_from_user(test_dev->kbuf, buf, size) != 0)
        {
            printk("copy_from_user error\n");
            return -1;
        }
        printk("kbuf is %s\n", test_dev->kbuf);
    }
    else if (test_dev->minor == 1)
    {
        if (copy_from_user(test_dev->kbuf, buf, size) != 0)
        {
            printk("copy_from_user error\n");
            return -1;
        }
        printk("kbuf is %s\n", test_dev->kbuf);
    }

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

static int private_data_init(void) // 驱动入口函数
{
    /*注册字符设备驱动*/
    int ret;
    /*1 创建设备号,,这里注册 2 个设备号*/
    ret = alloc_chrdev_region(&dev1.dev_num, 0, 2, "alloc_name"); // 动态分配设备号
    if (ret < 0)
    {
        printk("alloc_chrdev_region is error\n");
    }
    printk("alloc_chrdev_region is ok\n");
    dev1.major = MAJOR(dev1.dev_num);       // 获取主设备号
    dev1.minor = MINOR(dev1.dev_num);       // 获取次设备号
    printk("major is %d \r\n", dev1.major); // 打印主设备号
    printk("minor is %d \r\n", dev1.minor); // 打印次设备号
    // 对设备 1 进行操作
    /*2 初始化 cdev*/
    dev1.cdev_test.owner = THIS_MODULE;
    cdev_init(&dev1.cdev_test, &cdev_test_ops);
    /*3 添加一个 cdev,完成字符设备注册到内核*/
    cdev_add(&dev1.cdev_test, dev1.dev_num, 1);
    /*4 创建类*/
    dev1.class = class_create(THIS_MODULE, "test1");
    /*5 创建设备*/
    dev1.device = device_create(dev1.class, NULL, dev1.dev_num, NULL, "test1");

    dev2.major = MAJOR(dev1.dev_num + 1);   // 获取主设备号
    dev2.minor = MINOR(dev1.dev_num + 1);   // 获取次设备号
    printk("major is %d \r\n", dev2.major); // 打印主设备号
    printk("minor is %d \r\n", dev2.minor); // 打印次设备号
    // 对设备 2 进行操作
    /*2 初始化 cdev*/
    dev2.cdev_test.owner = THIS_MODULE;
    cdev_init(&dev2.cdev_test, &cdev_test_ops);
    /*3 添加一个 cdev,完成字符设备注册到内核*/
    cdev_add(&dev2.cdev_test, dev1.dev_num + 1, 1);
    /*4 创建类*/
    dev2.class = class_create(THIS_MODULE, "test2");
    /*5 创建设备*/
    dev2.device = device_create(dev2.class, NULL, dev1.dev_num + 1, NULL, "test2");
    return 0;
}

static void __exit __exit private_data_exit(void)
{
    unregister_chrdev_region(dev1.dev_num, 1);
    unregister_chrdev_region(dev2.dev_num + 1, 1);

    cdev_del(&dev1.cdev_test);
    device_destroy(dev1.class, dev1.dev_num);
    class_destroy(dev1.class);

    cdev_del(&dev2.cdev_test);
    device_destroy(dev2.class, dev2.dev_num + 1);
    class_destroy(dev2.class);

    printk("bye bye\n");
}

module_init(private_data_init);
module_exit(private_data_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zr");
MODULE_VERSION("0.1.0");