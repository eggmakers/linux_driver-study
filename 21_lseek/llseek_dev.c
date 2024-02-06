#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/atomic.h>
#define BUFSIZE 1024            // 设置最大偏移量为 1024
static char mem[BUFSIZE] = {0}; // 设置数据存储数组 mem
struct device_test
{
    dev_t dev_num;         // 设备号
    int major;             // 主设备号
    int minor;             // 次设备号
    struct cdev cdev_test; // cdev
    struct class *class;   // 类
    struct device *device; // 设备
    char kbuf[32];
};
static struct device_test dev1;
static int cdev_test_open(struct inode *inode, struct file *file)
{
    file->private_data = &dev1; // 设置私有数据
    return 0;
}
/*从设备读取数据*/
static ssize_t cdev_test_read(struct file *file, char __user *buf, size_t size, loff_t *off)
{
    loff_t p = *off; // 将读取数据的偏移量赋值给 loff_t 类型变量 p
    int i;
    size_t count = size;
    if (p > BUFSIZE)
    { // 如果当前偏移值比最大偏移量大则返回错误
        return -1;
    }
    if (count > BUFSIZE - p)
    {
        count = BUFSIZE - p; // 如果要读取的偏移值超出剩余的空间，则读取到最后位置
    }
    if (copy_to_user(buf, mem + p, count))
    { // 将 mem 中的值写入 buf，并传递到用户空间
        printk("copy_to_user error \n");
        return -1;
    }
    for (i = 0; i < 20; i++)
    {
        printk("buf[%d] is %c\n", i, mem[i]); // 将 mem 中的值打印出来
    }
    printk("mem is %s,p is %llu,count is %d\n", mem + p, p, count);
    *off = *off + count; // 更新偏移值
    return count;
}
/*向设备写入数据函数*/
static ssize_t cdev_test_write(struct file *file, const char __user *buf, size_t size, loff_t *off)
{
    loff_t p = *off; // 将写入数据的偏移量赋值给 loff_t 类型变量 p
    size_t count = size;
    if (p > BUFSIZE)
    { // 如果当前偏移值比最大偏移量大则返回错误
        return 0;
    }
    if (count > BUFSIZE - p)
    {
        count = BUFSIZE - p; // 如果要写入的偏移值超出剩余的空间，则写入到最后位置
    }
    if (copy_from_user(mem + p, buf, count))
    { // 将 buf 中的值，从用户空间传递到内核空间
        printk("copy_to_user error \n");
        return -1;
    }
    printk("mem is %s,p is %llu\n", mem + p, p); // 打印写入的值
    *off = *off + count;                         // 更新偏移值
    return count;
}
static int cdev_test_release(struct inode *inode, struct file *file)
{
    return 0;
}
static loff_t cdev_test_llseek(struct file *file, loff_t offset, int whence)
{
    loff_t new_offset; // 定义 loff_t 类型的新的偏移值
    switch (whence)    // 对 lseek 函数传递的 whence 参数进行判断
    {
    case SEEK_SET:
        if (offset < 0)
        {
            return -EINVAL;
            break;
        }
        if (offset > BUFSIZE)
        {
            return -EINVAL;
            break;
        }
        new_offset = offset; // 如果 whence 参数为 SEEK_SET，则新偏移值为 offset
        break;
    case SEEK_CUR:
        if (file->f_pos + offset > BUFSIZE)
        {
            return -EINVAL;
            break;
        }
        if (file->f_pos + offset < 0)
        {
            return -EINVAL;
            break;
        }
        new_offset = file->f_pos + offset; // 如果 whence 参数为 SEEK_CUR，则新偏移值为 file->f_pos + offset，file->f_pos 为当前的偏移值 break;
    case SEEK_END:
        if (file->f_pos + offset < 0)
        {
            return -EINVAL;
            break;
        }
        new_offset = BUFSIZE + offset; // 如果 whence 参数为 SEEK_END，则新偏移值为 BUFSIZE +  offset，BUFSIZE 为最大偏移量 break;
    default:
        break;
    }
    file->f_pos = new_offset; // 更新 file->f_pos 偏移值
    return new_offset;
}
/*设备操作函数*/
struct file_operations cdev_test_fops = {
    .owner = THIS_MODULE,         // 将 owner 字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
    .open = cdev_test_open,       // 将 open 字段指向 chrdev_open(...)函数
    .read = cdev_test_read,       // 将 open 字段指向 chrdev_read(...)函数
    .write = cdev_test_write,     // 将 open 字段指向 chrdev_write(...)函数
    .release = cdev_test_release, // 将 open 字段指向 chrdev_release(...)函数
    .llseek = cdev_test_llseek,
};
static int __init timer_dev_init(void) // 驱动入口函数
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
static void __exit timer_dev_exit(void) // 驱动出口函数
{
    /*注销字符设备*/
    unregister_chrdev_region(dev1.dev_num, 1); // 注销设备号
    cdev_del(&dev1.cdev_test);                 // 删除 cdev
    device_destroy(dev1.class, dev1.dev_num);  // 删除设备
    class_destroy(dev1.class);                 // 删除类
}
module_init(timer_dev_init);
module_exit(timer_dev_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("topeet");