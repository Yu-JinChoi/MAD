#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/delay.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/version.h>

#define CSEMAD_LED_MAJOR 260
#define CSEMAD_LED_NAME "csemad_led"

#define CSEMAD_LED_ADDRESS 0x08000016

static  int ledport_usage = 0;
static unsigned char *csemad_led_addr;

ssize_t csemad_led_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what);
ssize_t csemad_led_read(struct file *inode, char *gdata, size_t length, loff_t *off_what);
int csemad_led_open(struct inode *minode,  struct file *mfile);
int csemad_led_release(struct inode *minode, struct file *mfile);

struct file_operations csemad_led_fops =
{
				.owner = THIS_MODULE,
				.open = csemad_led_open,
				.write = csemad_led_write,
				.read = csemad_led_read,
				.release = csemad_led_release,
};

// initialize LED port
int __init csemad_led_init(void)
{
			int a;
			a=register_chrdev(CSEMAD_LED_MAJOR, CSEMAD_LED_NAME, &csemad_led_fops);
			if(a<0)
			{
							printk(KERN_WARNING "register_chrdev() FAIL!\n");
							return a;
			}
			
			csemad_led_addr=ioremap(CSEMAD_LED_ADDRESS,0x1);

			return 0;
}

// release and exit LED port
void __exit csemad_led_exit(void)
{
				iounmap((void *)CSEMAD_LED_ADDRESS);
				unregister_chrdev(CSEMAD_LED_MAJOR, CSEMAD_LED_NAME);
}

// open LED port
int csemad_led_open(struct inode *minode, struct file *mfile)
{
				if(ledport_usage!=0) return -EBUSY;
				ledport_usage=1;

				return 0;

}

// close LED port 
int csemad_led_release(struct inode *minode, struct file *mfile)
{
				ledport_usage=0;
				return 0;
}


// read from LED port
ssize_t csemad_led_read(struct file *inode, char *gdata, size_t length, loff_t *off_what)
{
				inb((unsigned int)csemad_led_addr);
				if(copy_to_user(gdata,csemad_led_addr, length)!=0)
								return -EFAULT;

				return length;
}

// write to LED port
ssize_t csemad_led_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what)
{
				unsigned char led_data;

				if(copy_from_user(&led_data, gdata, length)!=0)
								return -EFAULT;

				outb(led_data, (unsigned int)csemad_led_addr);

				return length;
}

module_init(csemad_led_init);
module_exit(csemad_led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CSEMAD");
