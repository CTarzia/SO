#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

#include <linux/uaccess.h>
#include <linux/random.h>

static int numeroMagico = -1;

/*int medir(int n) {
    int s = 0;
    while (n != 0) {
        n /= 10;
        s++;
    }

    return s;
}*/

static ssize_t mi_lectura(struct file *filp, char __user *data, size_t s, loff_t *off) {
    if (numeroMagico == -1) {
        return -EPERM;
    }

    int resultado;

    get_random_bytes(&resultado, sizeof(resultado));

    resultado = ((resultado % numeroMagico) + numeroMagico) % numeroMagico;

    // if (resultado < 0) {
    //     resultado *= -1;
    // }


    char numeroEnChars[s];

    /*while (resultado != 0 && s > 0) {
        numeroEnChars[s-1] = (resultado % 10) + '0';
        resultado /= 10;
    }*/

    int largo = snprintf(numeroEnChars, s, "%d\n", resultado);

    copy_to_user(data, numeroEnChars, s);
    if (largo > s) largo = s;
    return largo;
}

static ssize_t mi_escritura(struct file *filp, const char __user *data, size_t s, loff_t *off) {
    char buffer[s+1];
    copy_from_user(buffer, data, s);
    buffer[s] = NULL;

    int numero;
    if (kstrtoint(buffer, 10, &numero) != 0) {
        return -EPERM;
    }

    numeroMagico = numero;
    return s;
}

static int mi_open (struct inode *inodo, struct file *archivo) {
    return 0;
}

static int mi_release (struct inode *inodo, struct file *archivo) {
    return 0;
}

static struct file_operations mis_operaciones = {.owner = THIS_MODULE,
     .read = mi_lectura,
     .write = mi_escritura,
     .open = mi_open,
     .release = mi_release
};

static struct cdev dispositivo;

static dev_t major;
static unsigned int minor = 0;
static unsigned int count = 1;
static struct class *mi_class;
static char* DEVICE_NAME = "azar";


static int __init azar_init(void) {
	printk(KERN_ALERT "Hola, numero!\n");
    cdev_init(&dispositivo, &mis_operaciones);
    alloc_chrdev_region(&major, minor, count,DEVICE_NAME);
    cdev_add(&dispositivo, major, count);

    mi_class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(mi_class, NULL, major, NULL, DEVICE_NAME);
	return 0;
}

static void __exit azar_exit(void) {
	printk(KERN_ALERT "Ya no te banco mas! Hasta nunca\n");
    cdev_del(&dispositivo);
    unregister_chrdev_region(major, count);

    device_destroy(mi_class, major);
    class_destroy(mi_class);
}

module_init(azar_init);
module_exit(azar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de 'Hola, mundo'");
