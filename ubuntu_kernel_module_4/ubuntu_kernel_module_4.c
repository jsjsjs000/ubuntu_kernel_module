/*
	sysfs - posiada strukturę, nowsze niż procfs (bez struktury)
	https://github.com/cirosantilli/linux-kernel-module-cheat/blob/e8f09a76e6b40f61f4b445a40eb28eb4f36a7392/kernel_module/sysfs.c
	https://unix.stackexchange.com/questions/4884/what-is-the-difference-between-procfs-and-sysfs
*/

#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/types.h>
#include <uapi/linux/stat.h> /* S_IRUSR, S_IWUSR */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("author");
MODULE_VERSION("1.0");

enum { FOO_SIZE_MAX = 4 };
static int foo_size;
static char foo_tmp[FOO_SIZE_MAX];

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buff)
{
	strncpy(buff, foo_tmp, foo_size);
	return foo_size;
}

static ssize_t foo_store(struct  kobject *kobj, struct kobj_attribute *attr,
		const char *buff, size_t count)
{
	foo_size = min(count, (size_t)FOO_SIZE_MAX);
	strncpy(foo_tmp, buff, foo_size);
	return count;
}

static struct kobj_attribute foo_attribute =
		__ATTR(foo, S_IRUGO | S_IWUSR, foo_show, foo_store);

static struct attribute *attrs[] = {
	&foo_attribute.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *kobj;

static int simple_init(void)
{
	printk(KERN_ALERT "hello...\n");
	kobj = kobject_create_and_add("ubuntu_kernel_module_sysfs", kernel_kobj);
	if (!kobj)
		return -ENOMEM;
	int ret = sysfs_create_group(kobj, &attr_group);
	if (ret)
		kobject_put(kobj);
	return ret;
}

static void simple_cleanup(void)
{
	printk(KERN_WARNING "bye ...\n");
	kobject_put(kobj);
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

sudo insmod ubuntu_kernel_module_4.ko
dmesg | tail -2
sudo rmmod ubuntu_kernel_module_4
dmesg | tail -2
lsmod
modinfo ubuntu_kernel_module_4.ko

tail -f /var/log/syslog

sudo sysctl kernel.printk=7 # not works on ssh
# or:
echo 8 > /proc/sys/kernel/printk # not works on ssh

make && sudo insmod ubuntu_kernel_module_4.ko
sudo rmmod ubuntu_kernel_module_4.ko
# --------------------------------------

echo "12345" > /sys/kernel/ubuntu_kernel_module_sysfs/foo
cat /sys/kernel/ubuntu_kernel_module_sysfs/foo

*/
