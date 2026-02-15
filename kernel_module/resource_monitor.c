#include <linux/module.h>      // Needed for all kernel modules
#include <linux/kernel.h>      // Kernel logging macros
#include <linux/init.h>        // Macros for init/exit functions
#include <linux/proc_fs.h>     // For creating /proc entries
#include <linux/seq_file.h>    // For seq_file interface
#include <linux/kernel_stat.h> // For CPU statistics
#include <linux/tick.h>        // For CPU time info
#include <linux/uaccess.h>     // Required for proc interface

#define PROC_NAME "resource_monitor"

static struct proc_dir_entry *proc_entry;

/*
 * This function is called when someone runs:
 * cat /proc/resource_monitor
 */
static int resource_show(struct seq_file *m, void *v)
{
    int cpu;
    u64 user, nice, system, idle, iowait, irq, softirq;

    seq_printf(m, "Linux Kernel Resource Monitor\n");
    seq_printf(m, "---------------------------------\n");

    for_each_possible_cpu(cpu) {
        struct kernel_cpustat kcs = kcpustat_cpu(cpu);

        user     = kcs.cpustat[CPUTIME_USER];
        nice     = kcs.cpustat[CPUTIME_NICE];
        system   = kcs.cpustat[CPUTIME_SYSTEM];
        idle     = kcs.cpustat[CPUTIME_IDLE];
        iowait   = kcs.cpustat[CPUTIME_IOWAIT];
        irq      = kcs.cpustat[CPUTIME_IRQ];
        softirq  = kcs.cpustat[CPUTIME_SOFTIRQ];

        seq_printf(m,
            "CPU %d:\n"
            "  User:    %llu\n"
            "  Nice:    %llu\n"
            "  System:  %llu\n"
            "  Idle:    %llu\n"
            "  IOWait:  %llu\n"
            "  IRQ:     %llu\n"
            "  SoftIRQ: %llu\n\n",
            cpu, user, nice, system, idle, iowait, irq, softirq);
    }

    return 0;
}

/* Open function for proc file */
static int resource_open(struct inode *inode, struct file *file)
{
    return single_open(file, resource_show, NULL);
}

/* File operations structure */
static const struct proc_ops resource_fops = {
    .proc_open    = resource_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

/* Module initialization */
static int __init resource_init(void)
{
    proc_entry = proc_create(PROC_NAME, 0, NULL, &resource_fops);

    if (!proc_entry) {
        pr_err("Failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }

    pr_info("Resource Monitor Module Loaded\n");
    return 0;
}

/* Module cleanup */
static void __exit resource_exit(void)
{
    proc_remove(proc_entry);
    pr_info("Resource Monitor Module Unloaded\n");
}

module_init(resource_init);
module_exit(resource_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Linux Kernel Resource Monitor");