#ifndef QEMU_RAM_DEVICE_H
#define QEMU_RAM_DEVICE_H

#include "hw/qdev.h"
#include "sysemu/hostmem.h"

#define TYPE_RAM_DEVICE "ram-device"
#define RAM_DEVICE(obj) \
            OBJECT_CHECK(RamDevice, (obj), TYPE_RAM_DEVICE)

/**
 * RamDevice:
 * @addr: starting guest physical address, where @RamDevice is mapped
 *         Default value: 0, means that address is auto-allocated.
 * @hostmem: host memory backend providing memory for @RamDevice
 */
typedef struct RamDevice {
    /* private */
    DeviceState parent_obj;

    /* public */
    uint64_t addr;
    HostMemoryBackend *hostmem;
} RamDevice;

#endif
