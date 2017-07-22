/*
 * State Helper Driver
 *
 * Copyright (c) 2016, Pranav Vashi <neobuddy89@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/cpu.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/state_helper.h>
#include <linux/cpufreq.h>
#include <linux/display_state.h>

#define STATE_HELPER			"state_helper"
#define HELPER_ENABLED			0
#define DELAY_MSEC			100
#define DYNAMIC_ENABLED			0
#define MIN_ALLOWED_INTERVAL		20
#define DYN_INTERVAL_MS			200
#define DYN_UP_THRES			75
#define DYN_DOWN_THRES			25
#define DEFAULT_MAX_CPUS_ONLINE		4
#define DEFAULT_MIN_CPUS_ONLINE		1

static struct state_helper {
	unsigned int enabled;
	unsigned int dynamic;
	unsigned int dyn_interval_ms;
	unsigned int dyn_up_threshold;
	unsigned int dyn_down_threshold;
	unsigned int max_cpus_online;
	unsigned int min_cpus_online;
} helper = {
	.enabled = HELPER_ENABLED,
	.dynamic = DYNAMIC_ENABLED,
	.dyn_interval_ms = DYN_INTERVAL_MS,
	.dyn_up_threshold = DYN_UP_THRES,
	.dyn_down_threshold = DYN_DOWN_THRES,
	.max_cpus_online = DEFAULT_MAX_CPUS_ONLINE,
	.min_cpus_online = DEFAULT_MIN_CPUS_ONLINE,
};

static struct state_info {
	unsigned int dynamic_cpus;
} info = {
	.dynamic_cpus = DEFAULT_MAX_CPUS_ONLINE,
};

static u64 last_load_time;

static struct workqueue_struct *helper_wq;
static struct delayed_work helper_work;

static void __ref state_helper_work(struct work_struct *work)
{
	if (info.dynamic_cpus == 0)
	{
	   if (cpu_online(3))
	      cpu_down(3);
	   if (cpu_online(2))
	      cpu_down(2);
	   if (cpu_online(1)) 
              cpu_down(1);
	   if (cpu_online(0))
	      cpu_down(0);
	}
	else if (info.dynamic_cpus == 1)
	{
	        if (!cpu_online(0))
	           cpu_up(0);
	 
	        if (cpu_online(3))
	     	   cpu_down(3);
	        if (cpu_online(2))
	       	   cpu_down(2);
	      	if (cpu_online(1)) 
        	   cpu_down(1);
	}
	else if (info.dynamic_cpus == 2)
	{
	       	if (!cpu_online(0))
	       	   cpu_up(0);
		if (!cpu_online(1))
	  	   cpu_up(1);
	   
	        if (cpu_online(3))
	       	   cpu_down(3);
		if (cpu_online(2))
	      	   cpu_down(2);
	}
	else if (info.dynamic_cpus == 3)
	{
		if (!cpu_online(0))
	       	   cpu_up(0);
		if (!cpu_online(1))
	   	   cpu_up(1);
		if (!cpu_online(2))
	  	   cpu_up(2);
	   
		if (cpu_online(3))
	   	   cpu_down(3);
	}
	else if (info.dynamic_cpus == 4)
	{
		if (!cpu_online(0))
	   	   cpu_up(0);
		if (!cpu_online(1))
	   	   cpu_up(1);
		if (!cpu_online(2))
	   	   cpu_up(2);
		if (!cpu_online(3))
	   	   cpu_up(3);
	}
}

void reschedule_helper(void)
{	
	if (!helper.enabled || !is_display_on ())
		return;

	cancel_delayed_work_sync(&helper_work);
	queue_delayed_work(helper_wq, &helper_work,
		msecs_to_jiffies(DELAY_MSEC));
}

static void load_cpus(void)
{
	unsigned int avg_load = 0, cpu;
	unsigned int online_core_count = 0;
	int req_cpus;

	get_online_cpus();
	for_each_online_cpu (cpu)
	{
		avg_load = avg_load + cpufreq_quick_get_util (cpu);
		online_core_count++;
	}
	put_online_cpus();

	avg_load = avg_load / online_core_count;

	if (avg_load >= helper.dyn_up_threshold) {
		req_cpus = info.dynamic_cpus + 1;

		if (req_cpus > helper.max_cpus_online)
			req_cpus = helper.max_cpus_online;

		info.dynamic_cpus = req_cpus;
	} else if (avg_load <= helper.dyn_down_threshold) {
		req_cpus = info.dynamic_cpus - 1;

		if (req_cpus < helper.min_cpus_online)
			req_cpus = helper.min_cpus_online;

		info.dynamic_cpus = req_cpus;
	}

	if (online_core_count - 4 != info.dynamic_cpus)
		reschedule_helper();
}

void load_notify ()
{
	u64 now;

	if (!helper.enabled || !helper.dynamic || !is_display_on ()) {
		info.dynamic_cpus = helper.max_cpus_online;
		last_load_time = ktime_to_us(ktime_get());
		return;
	}

	now = ktime_to_us(ktime_get());

	if (now - last_load_time < helper.dyn_interval_ms * USEC_PER_MSEC)
		return;

	last_load_time = ktime_to_us(ktime_get());
	load_cpus();
}

static void state_helper_start(void)
{
	helper_wq =
	    alloc_workqueue("state_helper_wq", WQ_HIGHPRI | WQ_FREEZABLE, 0);
	if (!helper_wq) {
		pr_err("%s: Failed to allocate helper workqueue\n",
		       STATE_HELPER);
		goto err_out;
	}

	INIT_DELAYED_WORK(&helper_work, state_helper_work);
	reschedule_helper();

	return;

err_out:
	helper.enabled = 0;
	return;
}

static void __ref state_helper_stop(void)
{
	int cpu;

	flush_workqueue(helper_wq);
	cancel_delayed_work_sync(&helper_work);

	/* Wake up all the sibling cores */
	for_each_possible_cpu(cpu)
		if (!cpu_online(cpu))
			cpu_up(cpu);
}

/************************** sysfs interface ************************/

static ssize_t show_enabled(struct kobject *kobj,
				struct kobj_attribute *attr, 
				char *buf)
{
	return sprintf(buf, "%u\n", helper.enabled);
}

static ssize_t store_enabled(struct kobject *kobj,
				struct kobj_attribute *attr,
				const char *buf, size_t count)
{
	int ret;
	unsigned int val;

	ret = sscanf(buf, "%u", &val);
	if (ret != 1 || val < 0 || val > 1)
		return -EINVAL;

	if (val == helper.enabled)
		return count;

	helper.enabled = val;

	if (helper.enabled)
		state_helper_start();
	else
		state_helper_stop();

	return count;
}

static ssize_t show_dynamic(struct kobject *kobj,
				struct kobj_attribute *attr, 
				char *buf)
{
	return sprintf(buf, "%u\n", helper.dynamic);
}

static ssize_t store_dynamic(struct kobject *kobj,
				struct kobj_attribute *attr,
				const char *buf, size_t count)
{
	int ret;
	unsigned int val;

	ret = sscanf(buf, "%u", &val);
	if (ret != 1 || val < 0 || val > 1)
		return -EINVAL;

	if (val == helper.dynamic)
		return count;

	helper.dynamic = val;

	if (!helper.dynamic)
		info.dynamic_cpus = helper.max_cpus_online;

	reschedule_helper();

	return count;
}

static ssize_t show_dyn_interval_ms(struct kobject *kobj,
				struct kobj_attribute *attr, 
				char *buf)
{
	return sprintf(buf, "%u\n",helper.dyn_interval_ms);
}

static ssize_t store_dyn_interval_ms(struct kobject *kobj,
				struct kobj_attribute *attr,
				const char *buf, size_t count)
{
	int ret;
	unsigned int val;

	ret = sscanf(buf, "%u", &val);
	if (ret != 1)
		return -EINVAL;

	if (val < MIN_ALLOWED_INTERVAL)
		val = MIN_ALLOWED_INTERVAL;

	helper.dyn_interval_ms = val;

	return count;
}

static ssize_t show_dyn_up_threshold(struct kobject *kobj,
				struct kobj_attribute *attr, 
				char *buf)
{
	return sprintf(buf, "%u\n",helper.dyn_up_threshold);
}

static ssize_t store_dyn_up_threshold(struct kobject *kobj,
				struct kobj_attribute *attr,
				const char *buf, size_t count)
{
	int ret;
	unsigned int val;

	ret = sscanf(buf, "%u", &val);
	if (ret != 1)
		return -EINVAL;

	if (val < helper.dyn_down_threshold)
		val = helper.dyn_down_threshold;

	helper.dyn_up_threshold = val;

	return count;
}

static ssize_t show_dyn_down_threshold(struct kobject *kobj,
				struct kobj_attribute *attr, 
				char *buf)
{
	return sprintf(buf, "%u\n",helper.dyn_down_threshold);
}

static ssize_t store_dyn_down_threshold(struct kobject *kobj,
				struct kobj_attribute *attr,
				const char *buf, size_t count)
{
	int ret;
	unsigned int val;

	ret = sscanf(buf, "%u", &val);
	if (ret != 1)
		return -EINVAL;

	if (val > helper.dyn_up_threshold)
		val = helper.dyn_up_threshold;

	helper.dyn_down_threshold = val;

	return count;
}

static ssize_t show_max_cpus_online(struct kobject *kobj,
				struct kobj_attribute *attr, 
				char *buf)
{
	return sprintf(buf, "%u\n",helper.max_cpus_online);
}

static ssize_t store_max_cpus_online(struct kobject *kobj,
				struct kobj_attribute *attr,
				const char *buf, size_t count)
{
	int ret;
	unsigned int val;

	ret = sscanf(buf, "%u", &val);
	if (ret != 1 || val < 1)
		return -EINVAL;

	if (val > DEFAULT_MAX_CPUS_ONLINE)
		val = DEFAULT_MAX_CPUS_ONLINE;

	if (val < helper.min_cpus_online)
		val = helper.min_cpus_online;

	helper.max_cpus_online = val;

	reschedule_helper();

	return count;
}

static ssize_t show_min_cpus_online(struct kobject *kobj,
				struct kobj_attribute *attr, 
				char *buf)
{
	return sprintf(buf, "%u\n",helper.min_cpus_online);
}

static ssize_t store_min_cpus_online(struct kobject *kobj,
				struct kobj_attribute *attr,
				const char *buf, size_t count)
{
	int ret;
	unsigned int val;

	ret = sscanf(buf, "%u", &val);
	if (ret != 1 || val < 0)
		return -EINVAL;

	if (val > helper.max_cpus_online)
		val = helper.max_cpus_online;

	if (val == helper.min_cpus_online)
		return count;

	helper.min_cpus_online = val;

	if (!helper.enabled || !helper.dynamic)
		return count;

	last_load_time = ktime_to_us(ktime_get());
	load_cpus();

	return count;
}

#define KERNEL_ATTR_RW(_name) 				\
static struct kobj_attribute _name##_attr = 		\
	__ATTR(_name, 0664, show_##_name, store_##_name)

#define KERNEL_ATTR_RO(_name) 				\
static struct kobj_attribute _name##_attr = 		\
	__ATTR(_name, 0444, show_##_name, NULL)

KERNEL_ATTR_RW(enabled);
KERNEL_ATTR_RW(dynamic);
KERNEL_ATTR_RW(dyn_interval_ms);
KERNEL_ATTR_RW(dyn_up_threshold);
KERNEL_ATTR_RW(dyn_down_threshold);
KERNEL_ATTR_RW(max_cpus_online);
KERNEL_ATTR_RW(min_cpus_online);

static struct attribute *state_helper_attrs[] = {
	&enabled_attr.attr,
	&dynamic_attr.attr,
	&dyn_interval_ms_attr.attr,
	&dyn_up_threshold_attr.attr,
	&dyn_down_threshold_attr.attr,
	&max_cpus_online_attr.attr,
	&min_cpus_online_attr.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = state_helper_attrs,
	.name = STATE_HELPER,
};

/************************** sysfs end ************************/

static int state_helper_probe(struct platform_device *pdev)
{
	int ret = 0;

	ret = sysfs_create_group(kernel_kobj, &attr_group);

	if (helper.enabled)
		state_helper_start();

	return ret;
}

static struct platform_device state_helper_device = {
	.name = STATE_HELPER,
	.id = -1,
};

static int state_helper_remove(struct platform_device *pdev)
{
	if (helper.enabled)
		state_helper_stop();

	return 0;
}

static struct platform_driver state_helper_driver = {
	.probe = state_helper_probe,
	.remove = state_helper_remove,
	.driver = {
		.name = STATE_HELPER,
		.owner = THIS_MODULE,
	},
};

static int __init state_helper_init(void)
{
	int ret;

	ret = platform_driver_register(&state_helper_driver);
	if (ret) {
		pr_err("%s: Driver register failed: %d\n", STATE_HELPER, ret);
		return ret;
	}

	ret = platform_device_register(&state_helper_device);
	if (ret) {
		pr_err("%s: Device register failed: %d\n", STATE_HELPER, ret);
		return ret;
	}

	pr_info("%s: Device init\n", STATE_HELPER);

	return ret;
}

static void __exit state_helper_exit(void)
{
	platform_device_unregister(&state_helper_device);
	platform_driver_unregister(&state_helper_driver);
}

late_initcall(state_helper_init);
module_exit(state_helper_exit);

MODULE_AUTHOR("Pranav Vashi <neobuddy89@gmail.com>");
MODULE_DESCRIPTION("State Helper Driver");
MODULE_LICENSE("GPLv2");

