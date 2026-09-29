/*
 * SPDX-License-Identifier: GPL-2.0
 * cgroup2-memory.c - cgroup v2 memory controller
 *
 * Copyright (c) 2024
 * Author: Kernel Contributors
 *
 * Memory resource management controller for cgroup v2.
 * Provides memory usage tracking and enforcement of memory limits.
 */

#include <linux/cgroup.h>
#include <linux/slab.h>
#include <linux/page_counter.h>
#include "cgroup2.h"
#include "cgroup-internal.h"

/**
 * cgroup2_memory_limit_set - Set memory limit for a cgroup
 * @cgrp: Target cgroup
 * @limit: Memory limit in bytes
 *
 * Sets the maximum amount of memory that can be used by tasks in this cgroup.
 *
 * Return: 0 on success, negative errno on failure
 */
int cgroup2_memory_limit_set(struct cgroup2 *cgrp, unsigned long limit)
{
    if (!cgrp || !cgrp->memory)
        return -EINVAL;

    if (limit < atomic_long_read(&cgrp->memory_usage))
        return -EINVAL;

    cgrp->memory->limit = limit;
    pr_info("cgroup2: Set memory limit to %lu bytes\n", limit);
    return 0;
}

/**
 * cgroup2_memory_usage_get - Get current memory usage
 * @cgrp: Target cgroup
 *
 * Returns the current memory usage of tasks in the cgroup.
 *
 * Return: Memory usage in bytes
 */
unsigned long cgroup2_memory_usage_get(struct cgroup2 *cgrp)
{
    if (!cgrp)
        return 0;

    return atomic_long_read(&cgrp->memory_usage);
}

/**
 * cgroup2_memory_high_set - Set memory high threshold
 * @cgrp: Target cgroup
 * @high: High threshold in bytes
 *
 * Sets a soft limit for memory usage. Exceeding this will trigger
 * reclaim but won't fail memory allocations.
 *
 * Return: 0 on success, negative errno on failure
 */
int cgroup2_memory_high_set(struct cgroup2 *cgrp, unsigned long high)
{
    if (!cgrp || !cgrp->memory)
        return -EINVAL;

    cgrp->memory->high = high;
    pr_debug("cgroup2: Set memory high to %lu bytes\n", high);
    return 0;
}

/**
 * cgroup2_memory_pressure_stall - Get memory pressure stall information
 * @cgrp: Target cgroup
 *
 * Returns pressure stall information for memory resources.
 * This is used for monitoring memory pressure in the system.
 *
 * Return: PSI data or NULL on error
 */
void *cgroup2_memory_pressure_stall(struct cgroup2 *cgrp)
{
    if (!cgrp)
        return NULL;

    /* This would integrate with PSI (Pressure Stall Information) */
    return NULL;
}
