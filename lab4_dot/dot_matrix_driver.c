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

#define CSEMAD_DOT_MAJOR 262
#define CSEMAD_DOT_NAME "csemad_dot"
#define CSEMAD_DOT_ADDRESS 0x08000210

static int dotport_usage=0;
static unsigned char *csemad_dot_addr;

ssize_t csemad_dot_matrix_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what);
int csemad_dot_matrix_open(struct inode *minode, struct file *mfile);
int csemad_dot_matrix_release(struct inode *minode, struct file *mfile);

struct file_operations csemad_dot_matrix_fops =
{
				.owner = THIS_MODULE,
				.open = csemad_dot_matrix_open,
				.write = csemad_dot_matrix_write,
				.release = csemad_dot_matrix_release,
};

// initialize push dot matrix
int __init csemad_dot_matrix_init(void)
{
				int a;
				a=register_chrdev(CSEMAD_DOT_MAJOR, CSEMAD_DOT_NAME, &csemad_dot_matrix_fops);

				if(a<0)
				{
								printk(KERN_WARNING "register_chrdev() FAIL!!\n");
								return a;
				}

				csemad_dot_addr=ioremap(CSEMAD_DOT_ADDRESS,1);
				if(csemad_dot_addr==NULL)
				{
								printk("Failed ioremap");
								printk(KERN_ALERT "Failed");
								return -1;
				}

				return 0;
}

// release and exit dot matrix port
void __exit csemad_dot_matrix_exit(void)
{
				iounmap(csemad_dot_addr);
				unregister_chrdev(CSEMAD_DOT_MAJOR, CSEMAD_DOT_NAME);
}

// write to dot matrix port
ssize_t csemad_dot_matrix_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what)
{
				int i;

				for(i=0; i<10; i++)
				{
								if(copy_from_user(csemad_dot_addr+i*2, gdata+i, length)!=0)
												return -EFAULT;
				}

				return length;
}

// open!!
int csemad_dot_matrix_open(struct inode *minode, struct file *mfile)
{
				if(dotport_usage!=0) return -EBUSY;
				dotport_usage=1;

				return 0;
}

// close
int csemad_dot_matrix_release(struct inode *minode, struct file *mfile)
{
				dotport_usage=0;
				return 0;
}

module_init(csemad_dot_matrix_init);
module_exit(csemad_dot_matrix_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CSEMAD");
