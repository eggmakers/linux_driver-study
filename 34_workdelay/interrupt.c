#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
int irq;
struct workqueue_struct *test_workqueue;
struct delayed_work test_workqueue_work;
// 工作项处理函数
void test_work(struct work_struct *work)
{
    msleep(1000);
    printk("This is test_work\n");
}
// 中断处理函数
irqreturn_t test_interrupt(int irq, void *args)
{
    printk("This is test_interrupt\n");
    // 提交延迟工作项到自定义工作队列
    queue_delayed_work(test_workqueue, &test_workqueue_work, 3 * HZ);
    return IRQ_RETVAL(IRQ_HANDLED);
}

static int __init interrupt_irq_init(void)
{
    int ret;
    irq = gpio_to_irq(101);
    printk("irq is %d\n", irq);

    ret = request_irq(irq, test_interrupt, IRQF_TRIGGER_RISING, "test", NULL);
    if (ret < 0)
    {
        printk("request_irq is error\n");
        return -1;
    }
    // 创建工作队列
    test_workqueue = create_workqueue("test_workqueue");
    // 初始化延迟工作项
    INIT_DELAYED_WORK(&test_workqueue_work, test_work);
    return 0;
}
static void __exit interrupt_irq_exit(void)
{
    free_irq(irq, NULL);
    cancel_delayed_work_sync(&test_workqueue_work);
    flush_workqueue(test_workqueue);
    destroy_workqueue(test_workqueue);
    printk("bye bye\n");
}

module_init(interrupt_irq_init);
module_exit(interrupt_irq_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("topeet");