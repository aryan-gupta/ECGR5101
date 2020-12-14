/* A simple character device, "memory", that allows a character to be read */
/* or written from memory */

/* Necessary includes for device drivers */
#include <linux/init.h>
// #include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/uaccess.h> /* copy_from/to_user */
#include <asm/io.h>

#define UART0_ADDR 0x101F2000 // UART0 base register
#define UART_RXFE 0b00010000 // 5th bit of flag register
#define UART_TXFF 0b00100000 // 6th bit of flag register
#define UART_RXFF 0b01000000 // 7th bit of flag register
#define UART_TXFE 0b10000000 // 8th bit of flag register
#define UARTTCR 0x080
#define UARTCR 0x30
#define UARTFR 0x18 // Offset from UART0 base address
#define UARTDR 0x00 // Offset from UART0 base address

uint16_t *uart0_virtual; // Virtual address of uart0

MODULE_LICENSE("Dual BSD/GPL");

/* Declaration of memory.c functions */
int memory_open(struct inode *inode, struct file *filp);
int memory_release(struct inode *inode, struct file *filp);
ssize_t memory_read(struct file *filp,  char *buf, size_t count, loff_t *f_pos);
ssize_t memory_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
void memory_exit(void);
int memory_init(void);

/* Structure that declares the usual file */
/* access functions */
struct file_operations memory_fops = {
  read:  memory_read,
  write:  memory_write,
  open:  memory_open,
  release:  memory_release
};

/* Declaration of the init and exit functions */
module_init(memory_init);
module_exit(memory_exit);

/* Global variables of the driver */
/* Major number */
int memory_major = 60;
/* Buffer to store data */
char *memory_buffer;

/* Memory device initialization function */

int memory_init(void)
{
  int result;
  uint16_t cr;
  
  // Get virtual address of UART0
  uart0_virtual = (uint16_t *)ioremap(UART0_ADDR, 0x1000);

//   cr = ioread16(uart0_virtual + UARTTCR);
//   cr |= 1UL << 2;
//   cr |= 1UL << 1;
//   iowrite16(cr, uart0_virtual + UARTTCR);

//   printk(KERN_ALERT "Set test register in UART");
//   printk(KERN_ALERT "Writing to loopback fifo");


  cr = ioread16(uart0_virtual + UARTCR);
  cr |= 1UL << 7;
  // cr |= 1UL << 1;
  iowrite16(cr, uart0_virtual + UARTCR);

  printk(KERN_ALERT "Set loop in UART");

  /* Registering device */
  result = register_chrdev(memory_major, "uart", &memory_fops);
  if (result < 0) {
    printk(KERN_ERR "mem_dev: cannot obtain major number %d\n", memory_major);
    return result;
  }

 /* Allocating memory for the buffer */
  memory_buffer = kmalloc(1, GFP_KERNEL); 
  if (!memory_buffer) { 
    result = -ENOMEM;
    goto fail; 
  } 
  memset(memory_buffer, 0, 1);

  printk(KERN_ERR "Inserting memory module\n"); 
  return 0;

  fail: 
    memory_exit(); 
    return result;
}

/* Memory device exit function */

void memory_exit(void) 
{
  /* Freeing the major number */
  unregister_chrdev(memory_major, "uart");

  /* Freeing buffer memory */
  if (memory_buffer) {
    kfree(memory_buffer);
  }

  printk(KERN_ERR "Removing uart module\n");

}

/* Memory device open function */
int memory_open(struct inode *inode, struct file *filp)
{
  /* Success */
  return 0;
}

/* Memory device release function */
int memory_release(struct inode *inode, struct file *filp)
{
  /* Success */
  return 0;
}

/* Memory device read function */
ssize_t memory_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	if (count > 1) 
		count = 1;
	while(ioread16(uart0_virtual + UARTFR) & UART_RXFE) 
		;	
	
	*memory_buffer = (char)(ioread8(uart0_virtual + UARTDR));
	if (copy_to_user(buf, memory_buffer, 1)) 
		return -EFAULT;
	printk("Byte '%c' read from UART0 data register.\n", *memory_buffer);
	
	return count;
}

/* Memory device write function */
ssize_t memory_write( struct file *filp, const char *buf, size_t count, loff_t *f_pos) 
{
	if (count > 1) 
		count = 1;
	if(copy_from_user(memory_buffer, buf, 1)) 
		return -EFAULT;
	while (ioread16(uart0_virtual + UARTFR) & UART_TXFF)
    ;
	
  iowrite8(*memory_buffer, uart0_virtual + UARTDR);
  wmb();
	printk("Byte '%c' written to UART0 data register.\n", *memory_buffer);
        
	return count;
}
