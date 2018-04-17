#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/kthread.h>

struct task_struct *task0;
static spinlock_t spinlock;
int val;

int task(void *arg)
{
    printk(KERN_INFO "%s:%d\n",__func__,__LINE__);
    /* To generate panic uncomment following */
    /* panic("softlockup: hung tasks"); */

    while(!kthread_should_stop()) {
        printk(KERN_INFO "%s:%d\n",__func__,__LINE__);
        spin_lock(&spinlock);
        /* busy loop in critical section */
        while(1) {
            printk(KERN_INFO "%s:%d\n",__func__,__LINE__);
        }

        spin_unlock(&spinlock);
    }

    return val;
}

static int softlockup_init(void)
{
    printk(KERN_INFO "%s:%d\n",__func__,__LINE__);

    val = 1;
    spin_lock_init(&spinlock);
    task0 = kthread_run(&task,(void *)val,"softlockup_thread");
    set_cpus_allowed_ptr(task0, cpumask_of(0));

    return 0;
}

static void softlockup_exit(void)
{
    printk(KERN_INFO "%s:%d\n",__func__,__LINE__);
    kthread_stop(task0);
}

module_init(softlockup_init);
module_exit(softlockup_exit);

MODULE_AUTHOR("Saiyam Doshi");
MODULE_DESCRIPTION("Test module to generate CPU soft lock-up/panic");
MODULE_LICENSE("GPL v2");
