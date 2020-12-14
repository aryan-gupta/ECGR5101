#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>


MODULE_LICENSE("Dual BSD/GPL");

#define PROCFS_MAX_SIZE		1024
#define PROCFS_NAME 		"HelloWorld"

static int num=0;
module_param(num, int, 0660);
 
static char *devname = "simpdev";
module_param(devname, charp, 0660);

static struct proc_dir_entry *proc_file;


static int hello_proc_show(struct seq_file *m, void *v) {
	unsigned long msec;
	msec = jiffies_to_msecs(jiffies);
  	seq_printf(m, "jiffies to msec: %lu ms", msec);
  	return 0;
}

static int hello_proc_open(struct inode *inode, struct  file *file) {
  	return single_open(file, hello_proc_show, NULL);
}

static struct proc_ops my_fops = {
    .proc_open = hello_proc_open,
    .proc_release = single_release,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
};

static int hello_init(void)
{
	int i;
	unsigned long msec;

	for (i = 0; i < num; ++i) {
		printk(KERN_ALERT "Hello,%s!\n", devname);
	}

	msec = jiffies_to_msecs(jiffies);
	printk(KERN_ALERT "Time since start: %lu ms\n", msec);

	proc_file = proc_create(PROCFS_NAME, 0644, NULL, &my_fops);

	printk(KERN_INFO "/proc/%s created\n", PROCFS_NAME);	

	return 0;
}

static void hello_exit(void)
{
	remove_proc_entry(PROCFS_NAME, NULL);
	printk(KERN_ALERT "Goodbye, cruel world \n");
}

module_init(hello_init);
module_exit(hello_exit);

