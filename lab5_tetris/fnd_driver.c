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

#define CSEMAD_FND_MAJOR 261
#define CSEMAD_FND_NAME "csemad_fnd"

#define CSEMAD_FND_ADDRESS 0x08000004

static  int fndport_usage = 0;
static unsigned char *csemad_fnd_addr;

ssize_t csemad_fnd_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what);
int csemad_fnd_open(struct inode *minode,  struct file *mfile);
int csemad_fnd_release(struct inode *minode, struct file *mfile);

struct file_operations csemad_fnd_fops =
{
				.owner = THIS_MODULE,
				.open = csemad_fnd_open,
				.write = csemad_fnd_write,
				.release = csemad_fnd_release,
};

// initialize FND port
int __init csemad_fnd_init(void)
{
			int a;
			a=register_chrdev(CSEMAD_FND_MAJOR, CSEMAD_FND_NAME, &csemad_fnd_fops);
			if(a<0)
			{
							printk(KERN_WARNING "register_chrdev() FAIL!\n");
							return a;
			}
			
			csemad_fnd_addr=ioremap(CSEMAD_FND_ADDRESS,0x4);

			return 0;
}

// release and exit FND port
void __exit csemad_fnd_exit(void)
{
				iounmap(csemad_fnd_addr);
				unregister_chrdev(CSEMAD_FND_MAJOR, CSEMAD_FND_NAME);
}

// open FND port
int csemad_fnd_open(struct inode *minode, struct file *mfile)
{
				if(fndport_usage!=0) return -EBUSY;
				fndport_usage=1;

				return 0;

}

// close FND port 
int csemad_fnd_release(struct inode *minode, struct file *mfile)
{
				fndport_usage=0;
				return 0;
}

// write to FND port
ssize_t csemad_fnd_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what)
{
				unsigned char value[4];
				unsigned short int value_short = 0;

				if(copy_from_user(&value, gdata, 4))
								return -EFAULT;

				value_short = value[0]<< 12 | value[1] << 8 | value[2] << 4 | value[3];
				outw(value_short, (unsigned int)csemad_fnd_addr);

				return length;
}

module_init(csemad_fnd_init);
module_exit(csemad_fnd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CSEMAD");
