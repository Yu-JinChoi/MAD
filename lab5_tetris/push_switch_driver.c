#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/platform_device.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/version.h>

#define CSEMAD_PS_MAJOR 265
#define CSEMAD_PS_NAME "csemad_ps"
#define CSEMAD_PS_ADDRESS 0x08000050

static int psport_usage = 0;
static unsigned char *csemad_ps_addr;
unsigned char addr_map[9];

ssize_t csemad_push_switch_read(struct file *inode, char *gdata, size_t length, loff_t *off_what);
int __init csemad_push_switch_init(void);
void __exit csemad_push_switch_exit(void);
int csemad_push_switch_open(struct inode *minode, struct file *mfile);
int csemad_push_switch_release(struct inode *minode, struct file *mfile);

struct file_operations csemad_push_switch_fops = 
{
				.owner = THIS_MODULE,
				.read = csemad_push_switch_read,
				.open = csemad_push_switch_open,
				.release = csemad_push_switch_release,
};

ssize_t csemad_push_switch_read(struct file *inode, char *gdata, size_t length, loff_t *off_what)
{
				int i;
				
				for(i=0;i<9; i++)
				{
								addr_map[i]=inb((unsigned int)csemad_ps_addr + i*2);
								printk(KERN_ALERT "[%d] = %d\n", i, addr_map[i]);
				}

				if(copy_to_user(gdata, addr_map, length)!=0)
								return -EFAULT;

				return length;
}

int __init csemad_push_switch_init(void)
{
				int a;
				a = register_chrdev(CSEMAD_PS_MAJOR, CSEMAD_PS_NAME, &csemad_push_switch_fops);

				if(a<0)
				{
								printk(KERN_WARNING "register_chrdev() FAIL!\n");
								return a;
				}

				csemad_ps_addr = ioremap(CSEMAD_PS_ADDRESS, 1);
				if(csemad_ps_addr==NULL)
				{
								printk("Failed ioremap");
								printk(KERN_ALERT "Failed");
								return -1;
				}

				return 0;
}

void __exit csemad_push_switch_exit(void)
{
				iounmap(csemad_ps_addr);
				unregister_chrdev(CSEMAD_PS_MAJOR, CSEMAD_PS_NAME);
}

int csemad_push_switch_open(struct inode *minode, struct file *mfile)
{
				if(psport_usage!=0) return -EBUSY;
				psport_usage=1;

				return 0;
}

int csemad_push_switch_release(struct inode *minode, struct file *mfile)
{
				psport_usage=0;
				return 0;
}

module_init(csemad_push_switch_init);
module_exit(csemad_push_switch_exit);

MODULE_LICENSE("GPL");
MODULE_LICENSE("CSEMAD");
