/*
	procfs
	https://github.com/cirosantilli/linux-kernel-module-cheat/blob/e8f09a76e6b40f61f4b445a40eb28eb4f36a7392/kernel_module/procfs.c
*/

#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/proc_fs.h>
#include <linux/types.h>
#include <linux/seq_file.h> /* seq_read, seq_lseek, single_open, single_release */
#include <uapi/linux/stat.h> /* S_IRUSR */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("author");
MODULE_VERSION("1.0");

static const char *filename = "ubuntu_kernel_module_3_procfs";

static int proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "message\n");
	return 0;
}

static int proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_show, NULL);
}

static const struct file_operations fops = {
	.llseek = seq_lseek,
	.open = proc_open,
	.owner = THIS_MODULE,
	.read = seq_read,
	.release = single_release,
};

static int simple_init(void)
{
	printk(KERN_ALERT "hello...\n");
	proc_create(filename, 0, NULL, &fops);
	return 0;
}

static void simple_cleanup(void)
{
	printk(KERN_WARNING "bye ...\n");
	remove_proc_entry(filename, NULL);
}

module_init(simple_init);
module_exit(simple_cleanup);

/*

sudo apt update && sudo apt upgrade && sudo apt autoremove
reboot

sudo apt install build-essential chec kinstall make cmake git

# Install Kernel source code snd headers
# https://askubuntu.com/questions/159833/how-do-i-get-the-kernel-source-code
sudo apt install linux-source

uname -r
#> 5.4.0-150-generic
# /lib/modules/5.4.0-150-generic
# /usr/src/linux-headers-5.4.0-150-generic
# /usr/src/linux-source-5.4.0-150

# https://devarea.com/linux-kernel-development-and-writing-a-simple-kernel-module
make
# make clean

sudo insmod ubuntu_kernel_module_3.ko
dmesg | tail -2
sudo rmmod ubuntu_kernel_module_3
dmesg | tail -2
lsmod
modinfo ubuntu_kernel_module_3.ko

tail -f /var/log/syslog

sudo sysctl kernel.printk=7 # not works on ssh
# or:
echo 8 > /proc/sys/kernel/printk # not works on ssh

make && sudo insmod ubuntu_kernel_module_3.ko
sudo rmmod ubuntu_kernel_module_3.ko
# --------------------------------------

cat /proc/ubuntu_kernel_module_3_procfs
#> message

*/
