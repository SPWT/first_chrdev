#include <linux/init.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/device.h>

#include <linux/cdev.h>

#if 1
#define MODULE_PRINT_DEBUG(fmt,arg...) printk(KERN_EMERG fmt,##arg)
#define MODULE_PRINT_ERROR(fmt,arg...) printk(KERN_EMERG "ERROR:[%s][%d]" fmt,__FUNCTION__,__LINE__,##arg)
#else
#define MODULE_PRINT_DEBUG(fmt,arg...)
#define MODULE_PRINT_ERROR(fmt,arg...)
#endif

#define DEVICE_NAME "first_chrdev"
#define DEVICE_MINOR_NUM 1

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("LSQ");

static struct class *first_chrdev_class;
static struct class_device *first_chrdev_class_devs;
int auto_major;

int numdev_major = 0;
int numdev_minor = 0;

module_param(numdev_major, int, S_IRUSR);

module_param(numdev_minor, int, S_IRUSR);

struct chr_dev_t {
	struct cdev cdev;
}chr_dev;

static int first_chrdev_open(struct inode *inode, struct file *file)
{
	printk(KERN_EMERG"open the chrdev now!\n");
	return 0;
}

static int first_chrdev_write(struct file *file, const unsigned char __user *buf, size_t count, loff_t *ppos)
{
	printk(KERN_EMERG"chrdev writen now!\n");
	return 0;
}

static struct file_operations first_chrdev_fops = {
		.owner  =  THIS_MODULE,
		.open   =  first_chrdev_open,
		.write  =  first_chrdev_write,
};

static int first_chrdev_init(void)
{
	printk(KERN_EMERG"char dev enter!\n");

	dev_t num_dev;
	int ret = 0;

	cdev_init(&chr_dev.cdev, &first_chrdev_fops);
	chr_dev.cdev.owner = THIS_MODULE;

	if (numdev_major)
	{
		// static
		num_dev = MKDEV(numdev_major, numdev_minor);

		ret = register_chrdev_region(num_dev, DEVICE_MINOR_NUM, DEVICE_NAME);

		MODULE_PRINT_DEBUG("[static] num_dev: 0x%x\n", num_dev);
	}
	else
	{
		// alloc
		ret = alloc_chrdev_region(&num_dev, numdev_minor, DEVICE_MINOR_NUM, DEVICE_NAME);

		numdev_major = MAJOR(num_dev);
		MODULE_PRINT_DEBUG("[alloc]num_dev: 0x%x, numdev_major: %d\n", num_dev, numdev_major);
	}

	cdev_add(&chr_dev.cdev, num_dev, DEVICE_MINOR_NUM);

//	auto_major = register_chrdev(0, "first_chrdev", &first_chrdev_fops);

	first_chrdev_class = class_create(THIS_MODULE, "firstdev");
	first_chrdev_class_devs = device_create(first_chrdev_class, NULL, MKDEV(numdev_major, numdev_minor), NULL, "first_chrdev");

	if (ret < 0)
	{
		printk(KERN_EMERG "register_chrdev_region req %d is failed!\n", numdev_major);
	}

	return 0;
}

static void first_chrdev_exit(void)
{
	printk(KERN_EMERG "char dev exit!\n");

	unregister_chrdev_region(MKDEV(numdev_major, numdev_minor), DEVICE_MINOR_NUM);

	cdev_del(&chr_dev.cdev);

//	unregister_chrdev(auto_major, "first_chrdev");

	device_unregister(first_chrdev_class_devs);
	class_destroy(first_chrdev_class);
}

module_init(first_chrdev_init);
module_exit(first_chrdev_exit);
