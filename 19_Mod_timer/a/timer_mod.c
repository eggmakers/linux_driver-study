#include <linux/module.h>
#include <linux/init.h>
#include <linux/time.h>

static void function_test(struct timer_list *t);
DEFINE_TIMER(timer_test, function_test);

static void function_test(struct timer_list *t)
{
    printk("this is function test\n");
}

static int __init timer_mod_init(void)
{
    printk("Hello World!\n");
    add_timer(&timer_test);
    return 0;
}

static void __exit timer_mod_exit(void)
{
    del_timer(&timer_test);
    printk("Goodbye World!\n");
}

module_init(timer_mod_init);
module_exit(timer_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zr");
MODULE_VERSION("0.1.0");