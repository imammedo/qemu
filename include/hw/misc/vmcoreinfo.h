/*
 * Virtual Machine coreinfo device
 *
 * Copyright (C) 2017 Red Hat, Inc.
 *
 * Authors: Marc-André Lureau <marcandre.lureau@redhat.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 *
 */
#ifndef VMCOREINFO_H
#define VMCOREINFO_H

#include "hw/qdev-core.h"
#include "standard-headers/linux/qemu_fw_cfg.h"
#include "qom/object.h"

#define VMCOREINFO_DEVICE "vmcoreinfo"
typedef struct VMCoreInfoState VMCoreInfoState;
DECLARE_INSTANCE_CHECKER(VMCoreInfoState, VMCOREINFO,
                         VMCOREINFO_DEVICE)

typedef struct fw_cfg_vmcoreinfo FWCfgVMCoreInfo;

struct VMCoreInfoState {
    DeviceClass parent_obj;

    bool has_vmcoreinfo;
    FWCfgVMCoreInfo vmcoreinfo;
};

/* returns NULL unless there is exactly one device */
static inline VMCoreInfoState *vmcoreinfo_find(void)
{
    Object *o = object_resolve_path_type("", VMCOREINFO_DEVICE, NULL);

    return o ? VMCOREINFO(o) : NULL;
}

#endif
