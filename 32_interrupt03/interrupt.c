#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

int irq;
struct tasklet_struct mytastlet;

void testsoft_func(struct softirq_action *softirq_action)
{
    printk("This is testsoft_func\n");
}

irqreturn_t test_interrupt(int irq, void *args)
{
    printk("This id test_interrupt\n");
    raise_softirq(TEST_SOFTIRQ);
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

    open_softirq(TEST_SOFTIRQ, testsoft_func);
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