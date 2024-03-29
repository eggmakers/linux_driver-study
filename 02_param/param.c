#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

static int a = 0;
static int array[5];
static int array_size;
static char str1[10] = {0};

module_param(a, int, S_IRUGO);
MODULE_PARM_DESC(a, "e.g:a=1");

module_param_array(array, int, &array_size, S_IRUGO);
MODULE_PARM_DESC(array, "e.g:array=1 2 3 4 5");

module_param_string(str, str1, sizeof(str1), S_IRUGO);
MODULE_PARM_DESC(str, "e.g:str=hello");

static int moduleparam_init(void)
{
    int i = 0;
    printk("a is %d\n", a);

    for (i = 0; i < array_size; i++)
    {
        printk("array[%d] is %d\n", i, array[i]);
    }
    printk("array_size is %d\n", array_size);

    printk("str1 is %s\n", str1);

    return 0;
}

static void moduleparam_exit(void)
{
    printk("Goodbye World!\n");
}

module_init(moduleparam_init);
module_exit(moduleparam_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zr");
MODULE_VERSION("0.1.0");