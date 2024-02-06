#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#define GPIO_PIN 101

static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
    printk(KERN_INFO "interrupt occurred on GPIO %d \n", GPIO_PIN);
    printk(KERN_INFO "This is irq_handler\n");
    return IRQ_HANDLED;
}

static int __init interrupt_init(void)
{
    int irq_num;

    printk(KERN_INFO "Initializing GPIO interrupt Driver\n");

    irq_num = gpio_to_irq(GPIO_PIN);
    printk(KERN_INFO "GPIO %d mapped to IRQ %d\n", GPIO_PIN, irq_num);

    if (request_irq(irq_num, gpio_irq_handler, IRQF_TRIGGER_RISING, "irq_test", NULL) != 0)
    {
        printk(KERN_ERR "Failed to request IRQ %d\n", irq_num);
        gpio_free(GPIO_PIN);
        return -ENODEV;
    }
    return 0;
}
static void __exit interrupt_exit(void)
{
    int irq_num = gpio_to_irq(GPIO_PIN);
    // 释放中断
    free_irq(irq_num, NULL);
    printk(KERN_INFO "GPIO Interrupt Driver exited successfully\n");
}
module_init(interrupt_init);
module_exit(interrupt_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("topeet");