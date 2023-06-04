/*
	debugfs
	https://github.com/cirosantilli/linux-kernel-module-cheat/blob/e8f09a76e6b40f61f4b445a40eb28eb4f36a7392/kernel_module/debugfs.c
*/

#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <uapi/linux/stat.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("author");
MODULE_VERSION("1.0");

static struct dentry *dir;
static uint32_t value = 42;

static int simple_init(void)
{
	printk(KERN_ALERT "hello...\n");
	struct dentry *file;
	dir = debugfs_create_dir("ubuntu_kernel_module_2_debugfs", 0);
	if (!dir)
	{
		printk(KERN_ALERT "debugfs_create_dir failed.");
		return -1;
	}

	file = debugfs_create_u32("ubuntu_kernel_module_2", S_IRUSR, dir, &value);
	if (!file)
	{
		printk(KERN_ALERT "debugfs_create_u32 failed.");
		return -1;
	}

	value = 43; // changed in /sys/kernel/debug/ubuntu_kernel_module_2_debugfs/ubuntu_kernel_module_2

	return 0;
}

static void simple_cleanup(void)
{
	printk(KERN_WARNING "bye ...\n");
	debugfs_remove_recursive(dir);
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

sudo insmod ubuntu_kernel_module_2.ko
dmesg | tail -2
sudo rmmod ubuntu_kernel_module_2
dmesg | tail -2
lsmod
modinfo ubuntu_kernel_module_2.ko

tail -f /var/log/syslog

sudo sysctl kernel.printk=7 # not works on ssh
# or:
echo 8 > /proc/sys/kernel/printk # not works on ssh

make && sudo insmod ubuntu_kernel_module_2.ko
sudo rmmod ubuntu_kernel_module_2.ko
# --------------------------------------

# https://docs.kernel.org/filesystems/debugfs.html
CONFIG_DEBUG_FS=y
cat /sys/kernel/debug/ubuntu_kernel_module_2_debugfs/ubuntu_kernel_module_2

*/
