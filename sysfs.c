#include <linux/init.h> /* Macros used to mark up functions e.g. __init __exit */
#include <linux/module.h> /* Core header for loading LKMs into the kernel */
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/fs.h> /* Header for the Linux file system support */
#include <linux/string.h>

MODULE_LICENSE("GPL");                              /* The license type -- this affects available functionality */
MODULE_AUTHOR("MadMax");                            /* The author -- visible when you use modinfo */
MODULE_DESCRIPTION("A simple proc filesystem lkm"); /* The description -- see modinfo */
MODULE_VERSION("0.1");                              /* A version number to inform users */

static struct kobject *example_kobject;
static char msg[256];

/* When user is trying to read */
static ssize_t msg_read(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf) {
  return sprintf(buf, "sysfs lkm received: %s", msg);
}

/* When user is trying to write something */
static ssize_t msg_write(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count) {
  return sprintf(msg, "%s (%zu letters)", buf, count);
}

static struct kobj_attribute msg_attribute =
    __ATTR(msg, 0660, msg_read, msg_write);

static int __init msysfs_init(void) {
  int error = 0;

  pr_debug("Module initialized successfully \n");

  example_kobject = kobject_create_and_add("sysfs_module", kernel_kobj);
  if (!example_kobject) return -ENOMEM;

  error = sysfs_create_file(example_kobject, &msg_attribute.attr);
  if (error) {
    pr_debug("failed to create the foo file in /sys/kernel/kobject_example \n");
  }

  return error;
}

static void __exit msysfs_exit(void) {
  pr_debug("Module un initialized successfully \n");
  kobject_put(example_kobject);
}

/*
 * A module must use the module_init() module_exit() macros from linux/init.h,
 * which
 *  identify the initialization function at insertion time and the cleanup
 * function (as
 *  listed above)
 */
module_init(msysfs_init);
module_exit(msysfs_exit);
