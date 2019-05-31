#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

ssize_t mi_lectura(struct file *filp, char __user *data, size_t s, loff_t *off) {
    return 0;
}

ssize_t mi_escritura(struct file *filp, const char __user *data, size_t s, loff_t *off) {
    return s;
}

int mi_open (struct inode *inodo, struct file *archivo) {
    return 0;
}

int mi_release (struct inode *inodo, struct file *archivo) {
    return 0;
}

static struct file_operations mis_operaciones = {.owner = THIS_MODULE,
     .read = mi_lectura,
     .write = mi_escritura,
     .open = mi_open,
     .release = mi_release
};

static struct cdev dispositivo;

dev_t major;
unsigned int minor = 0;
unsigned int count = 1;
static struct class *mi_class;
char* DEVICE_NAME = "nulo";


static int __init nulo_init(void) {
	printk(KERN_ALERT "Hola, recibo tu basura v 2!\n");
    cdev_init(&dispositivo, &mis_operaciones);
    alloc_chrdev_region(&major, minor, count,DEVICE_NAME);
    cdev_add(&dispositivo, major, count);

    mi_class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(mi_class, NULL, major, NULL, DEVICE_NAME);
	return 0;
}

static void __exit nulo_exit(void) {
	printk(KERN_ALERT "Ya no te banco mas! Hasta nunca\n");
    cdev_del(&dispositivo);
    unregister_chrdev_region(major, count);

    device_destroy(mi_class, major);
    class_destroy(mi_class);
}

module_init(nulo_init);
module_exit(nulo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de 'Hola, mundo'");
