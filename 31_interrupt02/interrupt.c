#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

int irq;
struct tasklet_struct mytastlet;

void mytastlet_func(unsigned long data)
{
    printk("data is %ld\n", data);
}

irqreturn_t test_interrupt(int irq, void *args)
{
    printk("This id test_interrupt\n");
    tasklet_schedule(&mytastlet);
    return IRQ_RETVAL(IRQ_HANDLED);
}

static int __init interrupt_init(void)
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

    tasklet_init(&mytastlet, mytastlet_func, 1);
    return 0;
}
static void __exit interrupt_exit(void)
{
    free_irq(irq, NULL);
    tasklet_enable(&mytastlet);
    tasklet_disable(&mytastlet);
    printk("bye bye\n");
}

module_init(interrupt_init);
module_exit(interrupt_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("topeet");