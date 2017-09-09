/*
 * PowerPlug HotPlug v1.3, a Dynamic HotPlug (Based on State Helper Driver) for Octa-Core big.LITTLE SoCs.
 *
 * Copyright (c) 2016, Pranav Vashi <neobuddy89@gmail.com>.
 * Copyright (c) 2017, Shoaib Anwar <Shoaib0595@gmail.com>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/cpu.h>
#include <linux/module.h>
#include <linux/cpufreq.h>
#include <linux/Power_Plug.h>
#include <linux/display_state.h>
#include <linux/platform_device.h>

#define POWER_PLUG			"power_plug"
#define HOTPLUG_TOGGLE			0
#define DYNAMIC_TOGGLE			0
#define DYN_INTERVAL_MS			500
#define DYN_UP_THRES			98
#define DYN_DOWN_THRES			49
#define DEFAULT_MAX_CPUS_ONLINE		NR_CPUS
#define DEFAULT_MIN_CPUS_ONLINE		5

static struct power_plug {
	unsigned int toggle;
	unsigned int dynamic;
	unsigned int dyn_interval_ms;
	unsigned int dyn_up_threshold;
	unsigned int dyn_down_threshold;
	unsigned int max_cpus_online;
	unsigned int min_cpus_online;
	unsigned int dynamic_cpus;
} hotplug = {
	.toggle = HOTPLUG_TOGGLE,
	.dynamic = DYNAMIC_TOGGLE,
	.dyn_interval_ms = DYN_INTERVAL_MS,
	.dyn_up_threshold = DYN_UP_THRES,
	.dyn_down_threshold = DYN_DOWN_THRES,
	.max_cpus_online = DEFAULT_MAX_CPUS_ONLINE,
	.min_cpus_online = DEFAULT_MIN_CPUS_ONLINE,
	.dynamic_cpus = DEFAULT_MAX_CPUS_ONLINE,
};

static u64 last_load_time;

static struct delayed_work hotplug_work;
static struct workqueue_struct *hotplug_wq;

static void __ref power_plug_work (struct work_struct *work)
{
	if (hotplug.dynamic)
	{
	   if (hotplug.dynamic_cpus == 1)
	   {
	      if (!cpu_online (4))
	         cpu_up (4);


	      if (cpu_online (3))
	         cpu_down (3);
	      if (cpu_online (2))
	         cpu_down (2);
	      if (cpu_online (1)) 
                 cpu_down (1);
	      if (cpu_online (0))
	         cpu_down (0);

	      if (cpu_online (7))
	         cpu_down (7);
	      if (cpu_online (6))
	         cpu_down (6);
	      if (cpu_online (5))
	         cpu_down (5);
	   }
	   else if (hotplug.dynamic_cpus == 2)
	   {
	           if (!cpu_online (4))
	              cpu_up (4);
	       	   if (!cpu_online (5))
		      cpu_up (5);
	 

	           if (cpu_online (3))
	     	      cpu_down (3);
	           if (cpu_online (2))
	       	      cpu_down (2);
	      	   if (cpu_online (1)) 
        	      cpu_down (1);
		   if (cpu_online (0))
		      cpu_down (0);

		   if (cpu_online (7))
	              cpu_down (7);
	   	   if (cpu_online (6))
		      cpu_down (6);
	   }
	   else if (hotplug.dynamic_cpus == 3)
	   {
	           if (!cpu_online (4))
	              cpu_up (4);
		   if (!cpu_online (5))
	              cpu_up (5);
		   if (!cpu_online (6))
		      cpu_up (6);
	   

	           if (cpu_online (3))
	      	      cpu_down (3);
	   	   if (cpu_online (2))
	              cpu_down (2);
	   	   if (cpu_online (1)) 
                      cpu_down (1);
	   	   if (cpu_online (0))
	              cpu_down (0);

	   	   if (cpu_online (7))
		      cpu_down (7);
	   }
	   else if (hotplug.dynamic_cpus == 4)
	   {
		   if (!cpu_online (4))
	              cpu_up (4);
		   if (!cpu_online (5))
	              cpu_up (5);
		   if (!cpu_online (6))
	              cpu_up (6);
		   if (!cpu_online (7))
	              cpu_up (7);

	 
	           if (cpu_online (3))
	      	      cpu_down (3);
	   	   if (cpu_online (2))
	              cpu_down (2);
	   	   if (cpu_online (1)) 
                      cpu_down (1);
	   	   if (cpu_online (0))
		      cpu_down (0);
	   }
	   else if (hotplug.dynamic_cpus == 5)
	   {
		   if (!cpu_online (4))
	              cpu_up (4);
		   if (!cpu_online (5))
	              cpu_up (5);
		   if (!cpu_online (6))
	              cpu_up (6);
		   if (!cpu_online (7))
	              cpu_up (7);
		
		   if (!cpu_online (0))
	              cpu_up (0);
	 

	           if (cpu_online (3))
	      	      cpu_down (3);
	   	   if (cpu_online (2))
	              cpu_down (2);
	   	   if (cpu_online (1)) 
		      cpu_down (1);
	   }
	   else if (hotplug.dynamic_cpus == 6)
	   {
		   if (!cpu_online (4))
	              cpu_up (4);
		   if (!cpu_online (5))
	              cpu_up (5);
		   if (!cpu_online (6))
	              cpu_up (6);
		   if (!cpu_online (7))
	              cpu_up (7);
		
		   if (!cpu_online (0))
	              cpu_up (0);
		   if (!cpu_online (1))
	              cpu_up (1);
	 

	           if (cpu_online (3))
	      	      cpu_down (3);
	   	   if (cpu_online (2))
		      cpu_down (2);
	   }
	   else if (hotplug.dynamic_cpus == 7)
	   {
		   if (!cpu_online (4))
	              cpu_up (4);
		   if (!cpu_online (5))
	              cpu_up (5);
		   if (!cpu_online (6))
	              cpu_up (6);
		   if (!cpu_online (7))
	              cpu_up (7);
		
		   if (!cpu_online (0))
	              cpu_up (0);
		   if (!cpu_online (1))
	              cpu_up (1);
		   if (!cpu_online (2))
	              cpu_up (2);
	 

	           if (cpu_online (3))
		      cpu_down (3);
	   }
	   else if (hotplug.dynamic_cpus == 8)
	   {
		   if (!cpu_online (4))
	              cpu_up (4);
		   if (!cpu_online (5))
	              cpu_up (5);
		   if (!cpu_online (6))
	              cpu_up (6);
		   if (!cpu_online (7))
	              cpu_up (7);
		
		   if (!cpu_online (0))
	              cpu_up (0);
		   if (!cpu_online (1))
	              cpu_up (1);
		   if (!cpu_online (2))
	              cpu_up (2);
		   if (!cpu_online (3))
		      cpu_up (3);
	   }
	}
	else
	{
	    if (hotplug.max_cpus_online == 1)
	    {
	       if (!cpu_online (4))
	          cpu_up (4);


	       if (cpu_online (3))
	          cpu_down (3);
	       if (cpu_online (2))
	          cpu_down (2);
	       if (cpu_online (1)) 
                  cpu_down (1);
	       if (cpu_online (0))
	          cpu_down (0);

	       if (cpu_online (7))
	          cpu_down (7);
	       if (cpu_online (6))
	          cpu_down (6);
	       if (cpu_online (5))
	          cpu_down (5);
	    }
	    else if (hotplug.max_cpus_online == 2)
	    {
	            if (!cpu_online (4))
	               cpu_up (4);
	       	    if (!cpu_online (5))
		       cpu_up (5);
	 

	            if (cpu_online (3))
	     	       cpu_down (3);
	            if (cpu_online (2))
	       	       cpu_down (2);
	      	    if (cpu_online (1)) 
        	       cpu_down (1);
		    if (cpu_online (0))
		       cpu_down (0);

		    if (cpu_online (7))
	               cpu_down (7);
	   	    if (cpu_online (6))
		       cpu_down (6);
	    }
	    else if (hotplug.max_cpus_online == 3)
	    {
	       	    if (!cpu_online (4))
	               cpu_up (4);
		    if (!cpu_online (5))
	               cpu_up (5);
		    if (!cpu_online (6))
		       cpu_up (6);
	   

	            if (cpu_online (3))
	      	       cpu_down (3);
	   	    if (cpu_online (2))
	               cpu_down (2);
	   	    if (cpu_online (1)) 
                       cpu_down (1);
	   	    if (cpu_online (0))
	               cpu_down (0);

	   	    if (cpu_online (7))
		       cpu_down (7);
	    }
	    else if (hotplug.max_cpus_online == 4)
	    {
		    if (!cpu_online (4))
	               cpu_up (4);
		    if (!cpu_online (5))
	               cpu_up (5);
		    if (!cpu_online (6))
	               cpu_up (6);
		    if (!cpu_online (7))
	               cpu_up (7);

	 
	            if (cpu_online (3))
	      	       cpu_down (3);
	   	    if (cpu_online (2))
	               cpu_down (2);
	   	    if (cpu_online (1)) 
                       cpu_down (1);
	   	    if (cpu_online (0))
		       cpu_down (0);
	    }
	    else if (hotplug.max_cpus_online == 5)
	    {
		    if (!cpu_online (4))
	               cpu_up (4);
		    if (!cpu_online (5))
	               cpu_up (5);
		    if (!cpu_online (6))
	               cpu_up (6);
		    if (!cpu_online (7))
	               cpu_up (7);
		
		    if (!cpu_online (0))
	               cpu_up (0);
	 

	            if (cpu_online (3))
	      	       cpu_down (3);
	   	    if (cpu_online (2))
	               cpu_down (2);
	   	    if (cpu_online (1)) 
		       cpu_down (1);
	    }
	    else if (hotplug.max_cpus_online == 6)
	    {
		    if (!cpu_online (4))
	               cpu_up (4);
		    if (!cpu_online (5))
	               cpu_up (5);
		    if (!cpu_online (6))
	               cpu_up (6);
		    if (!cpu_online (7))
	               cpu_up (7);
		
		    if (!cpu_online (0))
	               cpu_up (0);
		    if (!cpu_online (1))
	               cpu_up (1);
	 

	            if (cpu_online (3))
	      	       cpu_down (3);
	   	    if (cpu_online (2))
		       cpu_down (2);
	    }
	    else if (hotplug.max_cpus_online == 7)
	    {
		    if (!cpu_online (4))
	               cpu_up (4);
		    if (!cpu_online (5))
	               cpu_up (5);
		    if (!cpu_online (6))
	               cpu_up (6);
		    if (!cpu_online (7))
	               cpu_up (7);
		
		    if (!cpu_online (0))
	               cpu_up (0);
		    if (!cpu_online (1))
	               cpu_up (1);
		    if (!cpu_online (2))
	               cpu_up (2);
	 

	            if (cpu_online (3))
		       cpu_down (3);
	    }
	    else if (hotplug.max_cpus_online == 8)
	    {
		    if (!cpu_online (4))
	               cpu_up (4);
		    if (!cpu_online (5))
	               cpu_up (5);
		    if (!cpu_online (6))
	               cpu_up (6);
		    if (!cpu_online (7))
	               cpu_up (7);
		
		    if (!cpu_online (0))
	               cpu_up (0);
		    if (!cpu_online (1))
	               cpu_up (1);
		    if (!cpu_online (2))
	               cpu_up (2);
		    if (!cpu_online (3))
		       cpu_up (3);
	    }
	}
}

void reschedule_hotplug (void)
{	
	if (!hotplug.toggle)
	   return;

	cancel_delayed_work_sync (&hotplug_work);
	queue_delayed_work (hotplug_wq, &hotplug_work, msecs_to_jiffies (100));
}

static void load_cpus (void)
{
	unsigned int cpu;
	unsigned int avg_load = 0; 
	unsigned int online_core_count = 0;
	int req_cpus;

	get_online_cpus ();
	for_each_online_cpu (cpu)
	{
		avg_load = avg_load + cpufreq_quick_get_util (cpu);
		online_core_count++;
	}
	put_online_cpus ();

	avg_load = avg_load / online_core_count;

	if (avg_load >= hotplug.dyn_up_threshold) 
	{
	   req_cpus = hotplug.dynamic_cpus + 1;

	   if (req_cpus > hotplug.max_cpus_online)
	      req_cpus = hotplug.max_cpus_online;

	   hotplug.dynamic_cpus = req_cpus;
	} 
	else if (avg_load <= hotplug.dyn_down_threshold) 
	{
		req_cpus = hotplug.dynamic_cpus - 1;

		if (req_cpus < hotplug.min_cpus_online)
		   req_cpus = hotplug.min_cpus_online;

		hotplug.dynamic_cpus = req_cpus;
	}

	if (online_core_count != hotplug.dynamic_cpus)
	   reschedule_hotplug ();
}

void load_notify ()
{
	u64 now;

	if (!hotplug.toggle || !hotplug.dynamic || !is_display_on ()) 
	{
	   hotplug.dynamic_cpus = hotplug.max_cpus_online;
	   last_load_time = ktime_to_us (ktime_get ());
	   return;
	}

	now = ktime_to_us (ktime_get ());

	if (now - last_load_time < hotplug.dyn_interval_ms * USEC_PER_MSEC)
	   return;

	last_load_time = ktime_to_us (ktime_get ());
	load_cpus ();
}

static void power_plug_start (void)
{
	hotplug_wq = alloc_workqueue ("power_plug_wq", WQ_HIGHPRI | WQ_FREEZABLE, 0);
	
	if (!hotplug_wq) 
	{
	   pr_err ("%s: Failed to Allocate Helper WorkQueue\n", POWER_PLUG);
	   goto err_out;
	}

	INIT_DELAYED_WORK (&hotplug_work, power_plug_work);
	reschedule_hotplug ();

	return;

err_out:
	hotplug.toggle = 0;
	return;
}

static void __ref power_plug_stop (void)
{
	int cpu;

	flush_workqueue (hotplug_wq);
	cancel_delayed_work_sync (&hotplug_work);

	// Wake-Up all Cores
	for_each_possible_cpu (cpu)
		if (!cpu_online (cpu))
		   cpu_up (cpu);
}

// sysFS Interface Begins Here---
static ssize_t show_toggle (struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf (buf, "%u\n", hotplug.toggle);
}

static ssize_t store_toggle (struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret;
	unsigned int val;

	ret = sscanf (buf, "%u", &val);
	
	if (ret != 1 || val < 0 || val > 1)
	   return -EINVAL;

	if (val == hotplug.toggle)
	   return count;

	hotplug.toggle = val;

	if (hotplug.toggle)
	   power_plug_start ();
	else
	    power_plug_stop ();

	return count;
}

static ssize_t show_dynamic (struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf (buf, "%u\n", hotplug.dynamic);
}

static ssize_t store_dynamic (struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret;
	unsigned int val;

	ret = sscanf (buf, "%u", &val);

	if (ret != 1 || val < 0 || val > 1)
	   return -EINVAL;

	if (val == hotplug.dynamic)
	   return count;

	hotplug.dynamic = val;

	if (!hotplug.dynamic)
	{
	   hotplug.dynamic_cpus = hotplug.max_cpus_online;
	   reschedule_hotplug ();
	}

	return count;
}

static ssize_t show_dyn_interval_ms (struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf (buf, "%u\n", hotplug.dyn_interval_ms);
}

static ssize_t store_dyn_interval_ms (struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret;
	unsigned int val;

	ret = sscanf (buf, "%u", &val);
	
	if (ret != 1 || val < 200)
	   return -EINVAL;

	hotplug.dyn_interval_ms = val;

	return count;
}

static ssize_t show_dyn_up_threshold (struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf (buf, "%u\n", hotplug.dyn_up_threshold);
}

static ssize_t store_dyn_up_threshold (struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret;
	unsigned int val;

	ret = sscanf (buf, "%u", &val);
	
	if (ret != 1 || val > 100)
	   return -EINVAL;

	if (val <= hotplug.dyn_down_threshold)
	   val = hotplug.dyn_down_threshold + 1;

	hotplug.dyn_up_threshold = val;

	return count;
}

static ssize_t show_dyn_down_threshold (struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf (buf, "%u\n", hotplug.dyn_down_threshold);
}

static ssize_t store_dyn_down_threshold (struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret;
	unsigned int val;

	ret = sscanf(buf, "%u", &val);

	if (ret != 1 || val < 0)
	   return -EINVAL;

	if (val >= hotplug.dyn_up_threshold)
	   val = hotplug.dyn_up_threshold - 1;

	hotplug.dyn_down_threshold = val;

	return count;
}

static ssize_t show_max_cpus_online (struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf (buf, "%u\n", hotplug.max_cpus_online);
}

static ssize_t store_max_cpus_online (struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret;
	unsigned int val;

	ret = sscanf(buf, "%u", &val);

	if (ret != 1 || val < hotplug.min_cpus_online || val > NR_CPUS)
	   return -EINVAL;

	hotplug.max_cpus_online = val;

	reschedule_hotplug ();

	return count;
}

static ssize_t show_min_cpus_online (struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf (buf, "%u\n", hotplug.min_cpus_online);
}

static ssize_t store_min_cpus_online (struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret;
	unsigned int val;

	ret = sscanf(buf, "%u", &val);

	if (ret != 1 || val < 1 || val >= hotplug.max_cpus_online)
	   return -EINVAL;

	if (val > hotplug.max_cpus_online)
	   val = hotplug.max_cpus_online;

	if (val == hotplug.min_cpus_online)
	   return count;

	hotplug.min_cpus_online = val;

	if (!hotplug.toggle || !hotplug.dynamic)
	   return count;

	last_load_time = ktime_to_us (ktime_get ());
	load_cpus ();

	return count;
}

#define KERNEL_ATTR_RW(_name) 				\
static struct kobj_attribute _name##_attr = 		\
	__ATTR(_name, 0664, show_##_name, store_##_name)

#define KERNEL_ATTR_RO(_name) 				\
static struct kobj_attribute _name##_attr = 		\
	__ATTR(_name, 0444, show_##_name, NULL)

KERNEL_ATTR_RW (toggle);
KERNEL_ATTR_RW (dynamic);
KERNEL_ATTR_RW (dyn_interval_ms);
KERNEL_ATTR_RW (dyn_up_threshold);
KERNEL_ATTR_RW (dyn_down_threshold);
KERNEL_ATTR_RW (max_cpus_online);
KERNEL_ATTR_RW (min_cpus_online);

static struct attribute *power_plug_attrs[] = {
	&toggle_attr.attr,
	&dynamic_attr.attr,
	&dyn_interval_ms_attr.attr,
	&dyn_up_threshold_attr.attr,
	&dyn_down_threshold_attr.attr,
	&max_cpus_online_attr.attr,
	&min_cpus_online_attr.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = power_plug_attrs,
	.name = POWER_PLUG,
};
// sysFS Interface Ends Here.

static int power_plug_probe (struct platform_device *pdev)
{
	int ret = 0;

	ret = sysfs_create_group (kernel_kobj, &attr_group);

	if (hotplug.toggle)
	   power_plug_start ();

	return ret;
}

static struct platform_device power_plug_device = {
	.name = POWER_PLUG,
	.id = -1,
};

static int power_plug_remove (struct platform_device *pdev)
{
	if (hotplug.toggle)
	   power_plug_stop();

	return 0;
}

static struct platform_driver power_plug_driver = {
	.probe = power_plug_probe,
	.remove = power_plug_remove,
	.driver = {
		.name = POWER_PLUG,
		.owner = THIS_MODULE,
	},
};

static int __init power_plug_init (void)
{
	int ret;

	ret = platform_driver_register (&power_plug_driver);
	
	if (ret) 
	{
	   pr_err ("%s: Driver-Register Failed: %d\n", POWER_PLUG, ret);
	   return ret;
	}

	ret = platform_device_register(&power_plug_device);

	if (ret) 
	{
	   pr_err ("%s: Device-Register Failed: %d\n", POWER_PLUG, ret);
	   return ret;
	}

	pr_info ("%s: Device Initialized\n", POWER_PLUG);

	return ret;
}

static void __exit power_plug_exit (void)
{
	platform_device_unregister (&power_plug_device);
	platform_driver_unregister (&power_plug_driver);
}

late_initcall (power_plug_init);
module_exit (power_plug_exit);

MODULE_AUTHOR ("Pranav Vashi <neobuddy89@gmail.com> and Shoaib Anwar <Shoaib0595@gmail.com>");
MODULE_DESCRIPTION ("PowerPlug HotPlug");
MODULE_LICENSE ("GPL v2");
