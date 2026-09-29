/*
 * SPDX-License-Identifier: GPL-2.0
 * cgroup2.h - cgroup v2 API definitions
 *
 * Copyright (c) 2024
 * Author: Kernel Contributors
 *
 * This file contains the core definitions and structures
 * for cgroup v2 (unified hierarchy) support.
 */

#ifndef _CGROUP2_H
#define _CGROUP2_H

#include <linux/cgroup.h>
#include <linux/kernfs.h>

/* cgroup2 features and capabilities */
#define CGROUP2_CAP_MEMORY     0x01
#define CGROUP2_CAP_CPU        0x02
#define CGROUP2_CAP_IO         0x04
#define CGROUP2_CAP_PIDS       0x08
#define CGROUP2_CAP_RDMA       0x10
#define CGROUP2_CAP_FREEZER    0x20
#define CGROUP2_CAP_CPUSET     0x40
#define CGROUP2_CAP_HUGETLB    0x80

/* cgroup2 controller states */
#define CGROUP2_STATE_INIT     0x00
#define CGROUP2_STATE_ACTIVE   0x01
#define CGROUP2_STATE_DISABLED 0x02

/* Memory controller structure for cgroup2 */
struct cgroup2_memory {
    unsigned long limit;
    unsigned long soft_limit;
    unsigned long usage;
    unsigned long high;
    unsigned long reserved;
};

/* CPU controller structure for cgroup2 */
struct cgroup2_cpu {
    unsigned long max_bandwidth;
    unsigned long min_bandwidth;
    unsigned long period;
    unsigned long quota;
};

/* Core cgroup2 structure */
struct cgroup2 {
    /* Basic cgroup information */
    struct cgroup_subsys_state *subsys[CGROUP_SUBSYS_COUNT];
    struct kernfs_node *kn;
    
    /* Capabilities and state */
    unsigned int capabilities;
    unsigned int state;
    
    /* Controllers */
    struct cgroup2_memory *memory;
    struct cgroup2_cpu *cpu;
    
    /* Hierarchy information */
    struct cgroup2 *parent;
    struct list_head children;
    struct list_head sibling;
    
    /* Resource tracking */
    atomic_long_t memory_usage;
    atomic_long_t cpu_usage;
};

/* API Functions */
int cgroup2_init(void);
int cgroup2_register_controller(const char *name, unsigned int cap);
struct cgroup2 *cgroup2_create(struct cgroup2 *parent, const char *name);
int cgroup2_remove(struct cgroup2 *cgrp);
int cgroup2_attach_task(struct cgroup2 *cgrp, struct task_struct *task);
int cgroup2_detach_task(struct cgroup2 *cgrp, struct task_struct *task);

/* Memory controller functions */
int cgroup2_memory_limit_set(struct cgroup2 *cgrp, unsigned long limit);
unsigned long cgroup2_memory_usage_get(struct cgroup2 *cgrp);

/* CPU controller functions */
int cgroup2_cpu_quota_set(struct cgroup2 *cgrp, unsigned long quota, unsigned long period);
int cgroup2_cpu_max_set(struct cgroup2 *cgrp, unsigned long max);

#endif /* _CGROUP2_H */
