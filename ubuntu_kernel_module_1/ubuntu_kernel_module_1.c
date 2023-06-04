/*
	hello world
	threads
	parameters
	https://github.com/cirosantilli/linux-kernel-module-cheat/blob/e8f09a76e6b40f61f4b445a40eb28eb4f36a7392/kernel_module/params.c
*/

#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <uapi/linux/stat.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("author");
MODULE_VERSION("1.0");

static int param1 = 0;
static int param2 = 0;
module_param(param1, int, S_IRUSR | S_IWUSR);
module_param(param2, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(param1, "my param 1");
MODULE_PARM_DESC(param2, "my param 2");

static struct task_struct *kthread;

static int kthread_loop(void *data)
{
	while (!kthread_should_stop())
	{
		pr_info("params: %d %d\n", param1, param2);
		if ((param1 % 2) != 0)
			param1++;
		usleep_range(1000 * 1000, 1000 * 1000 + 1);
	}
	return 0;
}

static int simple_init(void)
{
		// KERN_EMERG, KERN_ALERT, KERT_CRIT, KERN_ERR, KERN_WARNING, KERN_NOTICE, KERN_INFO, KERN_DEBUG
	printk(KERN_ALERT "hello...\n");
		// pr_debug(""); pr_info(""); pr_notice(""); pr_warn(""); pr_err(""); pr_crit(""); pr_alert(""); pr_emerg("");
	pr_info("init\n");
	// panic("hello panic!"); // zawiesza linuxa na VM

	kthread = kthread_create(kthread_loop, NULL, "mykthread");
	wake_up_process(kthread);

	return 0;
}

static void simple_cleanup(void)
{
	printk(KERN_WARNING "bye ...\n");
	kthread_stop(kthread);
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

sudo insmod ubuntu_kernel_module_1.ko
dmesg | tail -2
sudo rmmod ubuntu_kernel_module_1
dmesg | tail -2
lsmod
modinfo ubuntu_kernel_module_1.ko

tail -f /var/log/syslog

sudo sysctl kernel.printk=7 # not works on ssh
# or:
echo 8 > /proc/sys/kernel/printk # not works on ssh

make && sudo insmod ubuntu_kernel_module_1.ko && sudo rmmod ubuntu_kernel_module_1.ko
# --------------------------------------

ls /sys/module/ubuntu_kernel_module_1

echo 2 > /sys/module/ubuntu_kernel_module_1/parameters/param1
cat /sys/module/ubuntu_kernel_module_1/parameters/param1

set default parameters via the /etc/modprobe.conf (?)

*/
