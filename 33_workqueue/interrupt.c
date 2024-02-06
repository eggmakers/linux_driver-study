#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

int irq;
struct work_struct test_workqueue;

void test_work(struct work_struct *work)
{
    msleep(1000);
    printk("This is test_work\n");
}

irqreturn_t test_interrupt(int irq, void *args)
{
    printk("This id test_interrupt\n");
    schedule_work(&test_workqueue);
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

    INIT_WORK(&test_workqueue, test_work);
    return 0;
}
static void __exit interrupt_irq_exit(void)
{
    free_irq(irq, NULL);
    printk("bye bye\n");
}

module_init(interrupt_irq_init);
module_exit(interrupt_irq_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("topeet");