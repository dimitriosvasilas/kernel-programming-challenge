#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/uaccess.h>

#define DRIVER_AUTHOR "Dimitrios Vasilas <dimitrios.vasilas@scality.com>"
#define DRIVER_DESC "Eudyptula task06"
#define DEVICE_NAME "eudyptula"

#define UID "afa9c09dcaae"
#define BUFFER_SIZE 13

ssize_t miscd_read(struct file *fp, char __user *user, size_t size,
			loff_t *offs);

ssize_t miscd_write(struct file *fp, const char __user *user, size_t size,
			loff_t *offs);

static const struct file_operations miscd_fops = {
	.owner = THIS_MODULE,
	.read = miscd_read,
	.write = miscd_write
};

static struct miscdevice miscd_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &miscd_fops,
};

static int __init miscd_init(void)
{
	return misc_register(&miscd_misc);
}

static void __exit miscd_exit(void)
{
	misc_deregister(&miscd_misc);
	return;
}

ssize_t miscd_read(struct file *fp, char __user *user, size_t size,
			loff_t *offs)
{
	if (*offs != 0)
		return 0;

	if (copy_to_user(user, UID, BUFFER_SIZE))
		return -EINVAL;

	*offs += size;
	return size;
}

ssize_t miscd_write(struct file *fp, const char __user *user, size_t size,
			loff_t *offs)
{
	char *str = UID;
	char buff[BUFFER_SIZE];

	if (size != BUFFER_SIZE ||
		copy_from_user(buff, user, BUFFER_SIZE-1) ||
		strncmp(buff, str, BUFFER_SIZE - 1))
		return -EINVAL;

	return size;
}

module_init(miscd_init);
module_exit(miscd_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
