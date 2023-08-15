/******************************************************************************
 *
 *   Copyright (C) 2023  cx9ps3. All rights reserved.
 *
 *   SPDX-License-Identifier: GPL-2.0-only
 *
 *****************************************************************************/

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>

#define DEFAULT_ADDRESS 0xFFFFFFFF
static long address = DEFAULT_ADDRESS;

module_param(address,long,0660);
struct perf_event * __percpu *sample_hbp;

static void sample_hbp_handler(struct perf_event *bp,struct perf_sample_data *data,struct pt_regs *regs)
{
    printk(KERN_INFO "%s value is changed\n", address);
    dump_stack();
    printk(KERN_INFO "Dump stack from sample_hbp_handler\n");
}

static int register_breakpoint(void)
{
    struct perf_event_attr attr;

    attr.bp_addr = address;
    attr.bp_len = HW_BREAKPOINT_LEN_8;
    attr.bp_type = HW_BREAKPOINT_RW;

    sample_hbp = register_wide_hw_breakpoint(&attr,sample_hbp_handler,NULL);

    return 0;
}

static ssize_t sysfs_show(struct kobject *kobj,struct kobj_attribute *attr, char *buf)
{
    register_breakpoint();
    return sprintf(buf, "%lu\n", address);
}


static ssize_t sysfs_store(struct kobject *kobj,struct kobj_attribute *attr,const char *buf, size_t count)
{
    sscanf(buf,"%lu",&address);
    register_breakpoint();
    return count;
}

struct kobj_attribute etx_attr = __ATTR(address, 0660, sysfs_show, sysfs_store);
struct kobject *kobj_ref;

static int __init awesome_init(void)
{
	pr_info("Initialized module!\n");
    if(address != DEFAULT_ADDRESS)
    {
        register_breakpoint();
    }

    /*Creating a directory in /sys/kernel/ */
    kobj_ref = kobject_create_and_add("address",kernel_kobj);

    /*Creating sysfs file for address*/
    if(sysfs_create_file(kobj_ref,&etx_attr.attr))
    {
        pr_err("Cannot create sysfs file......\n");
        kobject_put(kobj_ref);
        sysfs_remove_file(kernel_kobj, &etx_attr.attr);
    }
    pr_info("Device Driver Insert...Done!!!\n");
	return 0;
}




static void __exit awesome_exit(void)
{
    unregister_wide_hw_breakpoint(sample_hbp);
	pr_info("exiting!\n");
}

module_init(awesome_init);
module_exit(awesome_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cx9ps3 <cx9ps3@gmail.com>");
MODULE_DESCRIPTION("My first kernel module\n");
MODULE_VERSION("1.0");