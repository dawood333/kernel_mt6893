/*
 * SPDX-License-Identifier: GPL-2.0
 * cgroup2-cpu.c - cgroup v2 CPU controller
 *
 * Copyright (c) 2024
 * Author: Kernel Contributors
 *
 * CPU resource management controller for cgroup v2.
 * Provides CPU bandwidth control and scheduling priority management.
 */

#include <linux/cgroup.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include "cgroup2.h"
#include "cgroup-internal.h"

/**
 * cgroup2_cpu_quota_set - Set CPU quota and period
 * @cgrp: Target cgroup
 * @quota: CPU quota (microseconds)
 * @period: Scheduling period (microseconds)
 *
 * Sets the CPU quota for tasks in the cgroup.
 * Quota defines the maximum CPU time available per period.
 *
 * Return: 0 on success, negative errno on failure
 */
int cgroup2_cpu_quota_set(struct cgroup2 *cgrp, unsigned long quota, unsigned long period)
{
    if (!cgrp || !cgrp->cpu)
        return -EINVAL;

    if (quota > period)
        return -EINVAL;

    cgrp->cpu->quota = quota;
    cgrp->cpu->period = period;
    pr_info("cgroup2: Set CPU quota to %lu/%lu microseconds\n", quota, period);
    return 0;
}

/**
 * cgroup2_cpu_max_set - Set CPU max (for absolute limit)
 * @cgrp: Target cgroup
 * @max: Maximum CPU utilization (percentage, 0-100)
 *
 * Sets an absolute maximum CPU limit for the cgroup.
 *
 * Return: 0 on success, negative errno on failure
 */
int cgroup2_cpu_max_set(struct cgroup2 *cgrp, unsigned long max)
{
    if (!cgrp || !cgrp->cpu)
        return -EINVAL;

    if (max > 100)
        return -EINVAL;

    cgrp->cpu->max_bandwidth = max;
    pr_info("cgroup2: Set CPU max to %lu%%\n", max);
    return 0;
}

/**
 * cgroup2_cpu_usage_get - Get current CPU usage
 * @cgrp: Target cgroup
 *
 * Returns the CPU time used by tasks in the cgroup.
 *
 * Return: CPU time in nanoseconds
 */
unsigned long cgroup2_cpu_usage_get(struct cgroup2 *cgrp)
{
    if (!cgrp)
        return 0;

    return atomic_long_read(&cgrp->cpu_usage);
}

/**
 * cgroup2_cpu_weight_set - Set CPU weight for fair scheduling
 * @cgrp: Target cgroup
 * @weight: Weight value (1-10000)
 *
 * Sets the scheduling weight for fair CPU time distribution.
 * Higher weights get more CPU time.
 *
 * Return: 0 on success, negative errno on failure
 */
int cgroup2_cpu_weight_set(struct cgroup2 *cgrp, unsigned long weight)
{
    if (!cgrp || !cgrp->cpu)
        return -EINVAL;

    if (weight < 1 || weight > 10000)
        return -EINVAL;

    cgrp->cpu->min_bandwidth = weight; /* Reuse field for weight */
    pr_debug("cgroup2: Set CPU weight to %lu\n", weight);
    return 0;
}
