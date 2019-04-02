/*
 * basic RAM device
 *
 * Copyright (C) 2019 Red Hat Inc
 *
 * Authors:
 *    Igor Mammedov <imammedo@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>
 */

#include "qemu/osdep.h"
#include "qapi/error.h"
#include "hw/mem/ram-device.h"

static void ram_device_realize(DeviceState *dev, Error **errp)
{
    RamDevice *ram = RAM_DEVICE(dev);

    if (!ram->hostmem) {
        error_setg(errp, "'memdev' property is not set");
        return;
    } else if (host_memory_backend_is_mapped(ram->hostmem)) {
        char *path = object_get_canonical_path_component(OBJECT(ram->hostmem));
        error_setg(errp, "can't use already busy memdev: %s", path);
        g_free(path);
        return;
    }

    host_memory_backend_set_mapped(ram->hostmem, true); 
}

static void ram_device_unrealize(DeviceState *dev, Error **errp)
{
    RamDevice *ram = RAM_DEVICE(dev);

    host_memory_backend_set_mapped(ram->hostmem, false);
}

static Property ram_device_properties[] = {
    DEFINE_PROP_UINT64("addr", RamDevice, addr, 0),
    DEFINE_PROP_LINK("memdev", RamDevice, hostmem,
                     TYPE_MEMORY_BACKEND, HostMemoryBackend *),
    DEFINE_PROP_END_OF_LIST(),
};

static void ram_device_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->realize = ram_device_realize;
    dc->unrealize = ram_device_unrealize;
    dc->props = ram_device_properties;
}

static const TypeInfo ram_device_info = {
    .name          = TYPE_RAM_DEVICE,
    .parent        = TYPE_DEVICE,
    .instance_size = sizeof(RamDevice),
    .class_init    = ram_device_class_init,
};

static void register_types(void)
{
    type_register_static(&ram_device_info);
}

type_init(register_types)
