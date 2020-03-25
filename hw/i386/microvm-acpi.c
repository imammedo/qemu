#include "qemu/osdep.h"
#include "qemu/cutils.h"
#include "qapi/error.h"
#include "hw/acpi/acpi-defs.h"
#include "hw/acpi/acpi.h"
#include "hw/nvram/fw_cfg.h"
#include "hw/acpi/bios-linker-loader.h"
#include "hw/acpi/aml-build.h"
#include "hw/acpi/utils.h"
#include "sysemu/reset.h"
#include "migration/vmstate.h"
#include "hw/i386/microvm.h"
#include "hw/i386/microvm-acpi.h"

#include "hw/core/cpu.h"
#include "target/i386/cpu.h"
#include "hw/nvram/fw_cfg.h"
#include "hw/i386/fw_cfg.h"
#include "hw/virtio/virtio-mmio.h"
#include "hw/acpi/generic_event_device.h"
#include "hw/i386/pc.h"

#define ACPI_BUILD_IOAPIC_ID 0x0

typedef
struct AcpiBuildState {
    /* Copy of table in RAM (for patching). */
    MemoryRegion *table_mr;
    MemoryRegion *rsdp_mr;
    MemoryRegion *linker_mr;
    /* Is table patched? */
    bool patched;
} AcpiBuildState;

static void
build_madt(GArray *table_data, BIOSLinker *linker,
           X86MachineState *x86ms, AcpiDeviceIf *adev)
{
    MachineClass *mc = MACHINE_GET_CLASS(x86ms);
    const CPUArchIdList *apic_ids = mc->possible_cpu_arch_ids(MACHINE(x86ms));
    int madt_start = table_data->len;
    AcpiDeviceIfClass *adevc = ACPI_DEVICE_IF_GET_CLASS(adev);
    bool x2apic_mode = false;

    AcpiMultipleApicTable *madt;
    AcpiMadtIoApic *io_apic;
    AcpiMadtIntsrcovr *intsrcovr;
    int i;

    madt = acpi_data_push(table_data, sizeof *madt);
    madt->local_apic_address = cpu_to_le32(APIC_DEFAULT_ADDRESS);
    madt->flags = cpu_to_le32(1);

    for (i = 0; i < apic_ids->len; i++) {
        adevc->madt_cpu(adev, i, apic_ids, table_data);
        if (apic_ids->cpus[i].arch_id > 254) {
            x2apic_mode = true;
        }
    }

    io_apic = acpi_data_push(table_data, sizeof *io_apic);
    io_apic->type = ACPI_APIC_IO;
    io_apic->length = sizeof(*io_apic);
    io_apic->io_apic_id = ACPI_BUILD_IOAPIC_ID;
    io_apic->address = cpu_to_le32(IO_APIC_DEFAULT_ADDRESS);
    io_apic->interrupt = cpu_to_le32(0);

    if (x86ms->apic_xrupt_override) {
        intsrcovr = acpi_data_push(table_data, sizeof *intsrcovr);
        intsrcovr->type   = ACPI_APIC_XRUPT_OVERRIDE;
        intsrcovr->length = sizeof(*intsrcovr);
        intsrcovr->source = 0;
        intsrcovr->gsi    = cpu_to_le32(2);
        intsrcovr->flags  = cpu_to_le16(0); /* conforms to bus specifications */
    }
    for (i = 1; i < 16; i++) {
#define ACPI_BUILD_PCI_IRQS ((1<<5) | (1<<9) | (1<<10) | (1<<11))
        if (!(ACPI_BUILD_PCI_IRQS & (1 << i))) {
            /* No need for a INT source override structure. */
            continue;
        }
        intsrcovr = acpi_data_push(table_data, sizeof *intsrcovr);
        intsrcovr->type   = ACPI_APIC_XRUPT_OVERRIDE;
        intsrcovr->length = sizeof(*intsrcovr);
        intsrcovr->source = i;
        intsrcovr->gsi    = cpu_to_le32(i);
        intsrcovr->flags  = cpu_to_le16(0xd); /* active high, level triggered */
    }

    if (x2apic_mode) {
        AcpiMadtLocalX2ApicNmi *local_nmi;

        local_nmi = acpi_data_push(table_data, sizeof *local_nmi);
        local_nmi->type   = ACPI_APIC_LOCAL_X2APIC_NMI;
        local_nmi->length = sizeof(*local_nmi);
        local_nmi->uid    = 0xFFFFFFFF; /* all processors */
        local_nmi->flags  = cpu_to_le16(0);
        local_nmi->lint   = 1; /* ACPI_LINT1 */
    } else {
        AcpiMadtLocalNmi *local_nmi;

        local_nmi = acpi_data_push(table_data, sizeof *local_nmi);
        local_nmi->type         = ACPI_APIC_LOCAL_NMI;
        local_nmi->length       = sizeof(*local_nmi);
        local_nmi->processor_id = 0xff; /* all processors */
        local_nmi->flags        = cpu_to_le16(0);
        local_nmi->lint         = 1; /* ACPI_LINT1 */
    }

    build_header(linker, table_data,
                 (void *)(table_data->data + madt_start), "APIC",
                 table_data->len - madt_start, 1, NULL, NULL);
}


static Aml *build_rtc_device_aml(void)
{
    Aml *dev;
    Aml *crs;

    dev = aml_device("RTC");
    aml_append(dev, aml_name_decl("_HID", aml_eisaid("PNP0B00")));
    crs = aml_resource_template();
    aml_append(crs, aml_io(AML_DECODE16, 0x0070, 0x0070, 0x10, 0x02));
    aml_append(crs, aml_irq_no_flags(8));
    aml_append(crs, aml_io(AML_DECODE16, 0x0072, 0x0072, 0x02, 0x06));
    aml_append(dev, aml_name_decl("_CRS", crs));

    return dev;
}

static Aml *build_com_device_aml(uint8_t uid, bool sta)
{
    Aml *dev;
    Aml *crs;
    Aml *method;
    Aml *if_ctx;
    Aml *else_ctx;
    Aml *zero = aml_int(0);
    Aml *is_present = aml_local(0);
    const char *enabled_field = "CAEN";
    uint8_t irq = 4;
    uint16_t io_port = 0x03F8;

    assert(uid == 1 || uid == 2);
    if (uid == 2) {
        enabled_field = "CBEN";
        irq = 3;
        io_port = 0x02F8;
    }

    dev = aml_device("COM%d", uid);
    aml_append(dev, aml_name_decl("_HID", aml_eisaid("PNP0501")));
    aml_append(dev, aml_name_decl("_UID", aml_int(uid)));

    if (sta) {
        method = aml_method("_STA", 0, AML_NOTSERIALIZED);
        aml_append(method, aml_store(aml_name("%s", enabled_field),
                                         is_present));
        if_ctx = aml_if(aml_equal(is_present, zero));
        {
            aml_append(if_ctx, aml_return(aml_int(0x00)));
        }
        aml_append(method, if_ctx);
        else_ctx = aml_else();
        {
            aml_append(else_ctx, aml_return(aml_int(0x0f)));
        }
        aml_append(method, else_ctx);
        aml_append(dev, method);
    }

    crs = aml_resource_template();
    aml_append(crs, aml_io(AML_DECODE16, io_port, io_port, 0x00, 0x08));
    aml_append(crs, aml_irq_no_flags(irq));
    aml_append(dev, aml_name_decl("_CRS", crs));

    return dev;
}

static void acpi_dsdt_add_fw_cfg(Aml *scope, Object *fw_cfg)
{
    /*
     * when using port i/o, the 8-bit data register *always* overlaps
     * with half of the 16-bit control register. Hence, the total size
     * of the i/o region used is FW_CFG_CTL_SIZE; when using DMA, the
     * DMA control register is located at FW_CFG_DMA_IO_BASE + 4
     */
    uint8_t io_size = object_property_get_bool(fw_cfg, "dma_enabled", NULL) ?
        ROUND_UP(FW_CFG_CTL_SIZE, 4) + sizeof(dma_addr_t) :
        FW_CFG_CTL_SIZE;
    Aml *dev = aml_device("FWCF");
    Aml *crs = aml_resource_template();

    aml_append(dev, aml_name_decl("_HID", aml_string("QEMU0002")));

    /* device present, functioning, decoding, not shown in UI */
    aml_append(dev, aml_name_decl("_STA", aml_int(0xB)));

    aml_append(crs,
        aml_io(AML_DECODE16, FW_CFG_IO_BASE, FW_CFG_IO_BASE, 0x01, io_size));

    aml_append(dev, aml_name_decl("_CRS", crs));
    aml_append(scope, dev);
}

static void acpi_dsdt_add_virtio(Aml *scope, MicrovmMachineState *mms)
{
    gchar *separator;
    long int index;
    BusState *bus;
    BusChild *kid;

    bus = sysbus_get_default();
    QTAILQ_FOREACH(kid, &bus->children, sibling) {
        DeviceState *dev = kid->child;
        ObjectClass *class = object_get_class(OBJECT(dev));

        if (class == object_class_by_name(TYPE_VIRTIO_MMIO)) {
            VirtIOMMIOProxy *mmio = VIRTIO_MMIO(OBJECT(dev));
            VirtioBusState *mmio_virtio_bus = &mmio->bus;
            BusState *mmio_bus = &mmio_virtio_bus->parent_obj;

            if (QTAILQ_EMPTY(&mmio_bus->children)) {
                continue;
            }
            separator = g_strrstr(mmio_bus->name, ".");
            if (!separator) {
                continue;
            }
            if (qemu_strtol(separator + 1, NULL, 10, &index) != 0) {
                continue;
            }

            uint32_t irq = mms->virtio_irq_base + index;
            hwaddr base = VIRTIO_MMIO_BASE + index * 512;
            hwaddr size = 512;

            Aml *dev = aml_device("VR%02u", (unsigned)index);
            aml_append(dev, aml_name_decl("_HID", aml_string("LNRO0005")));
            aml_append(dev, aml_name_decl("_UID", aml_int(index)));
            aml_append(dev, aml_name_decl("_CCA", aml_int(1)));

            Aml *crs = aml_resource_template();
            aml_append(crs, aml_memory32_fixed(base, size, AML_READ_WRITE));
            aml_append(crs,
                       aml_interrupt(AML_CONSUMER, AML_LEVEL, AML_ACTIVE_HIGH,
                                     AML_EXCLUSIVE, &irq, 1));
            aml_append(dev, aml_name_decl("_CRS", crs));
            aml_append(scope, dev);
        }
    }
}

static void
build_dsdt_microvm(GArray *table_data, BIOSLinker *linker,
                   MicrovmMachineState *mms)
{
    X86MachineState *x86ms = X86_MACHINE(mms);
    Aml *dsdt, *sb_scope;

    dsdt = init_aml_allocator();

    /* Reserve space for header */
    acpi_data_push(dsdt->buf, sizeof(AcpiTableHeader));

    sb_scope = aml_scope("_SB");
    if (mms->rtc_state) {
        aml_append(sb_scope, build_rtc_device_aml());
    }
    if (mms->isa_serial) {
        aml_append(sb_scope, build_com_device_aml(1, false));
    }
    acpi_dsdt_add_fw_cfg(sb_scope, OBJECT(x86ms->fw_cfg));
    acpi_dsdt_add_virtio(sb_scope, mms);
    aml_append(dsdt, sb_scope);

    /* copy AML table into ACPI tables blob and patch header there */
    g_array_append_vals(table_data, dsdt->buf->data, dsdt->buf->len);
    build_header(linker, table_data,
        (void *)(table_data->data + table_data->len - dsdt->buf->len),
        "DSDT", dsdt->buf->len, 1, NULL, NULL);
    free_aml_allocator();
}

static void build_fadt_rev5(GArray *table_data, BIOSLinker *linker,
                            MicrovmMachineState *mms, unsigned dsdt_tbl_offset)
{
    /* ACPI v5.1 */
    AcpiFadtData fadt = {
        .rev = 5,
        .minor_ver = 1,
        .flags = 1 << ACPI_FADT_F_HW_REDUCED_ACPI,
        .xdsdt_tbl_offset = &dsdt_tbl_offset,
    };

    build_fadt(table_data, linker, &fadt, NULL, NULL);
}

static
void microvm_acpi_build(MicrovmMachineState *mms, AcpiBuildTables *tables)
{
    GArray *table_offsets;
    unsigned dsdt, xsdt;
    GArray *tables_blob = tables->table_data;

    table_offsets = g_array_new(false, true /* clear */,
                                        sizeof(uint32_t));

    bios_linker_loader_alloc(tables->linker,
                             ACPI_BUILD_TABLE_FILE, tables_blob,
                             64, false /* high memory */);

    /* DSDT is pointed to by FADT */
    dsdt = tables_blob->len;
    build_dsdt_microvm(tables_blob, tables->linker, mms);

    /* FADT MADT pointed to by RSDT */
    acpi_add_table(table_offsets, tables_blob);
    build_fadt_rev5(tables_blob, tables->linker, mms, dsdt);

    acpi_add_table(table_offsets, tables_blob);
    build_madt(tables_blob, tables->linker, X86_MACHINE(mms), mms->acpi_dev);

    acpi_add_table(table_offsets, tables_blob);

    /* XSDT is pointed to by RSDP */
    xsdt = tables_blob->len;
    build_xsdt(tables_blob, tables->linker, table_offsets, NULL, NULL);

    /* RSDP is in FSEG memory, so allocate it separately */
    {
        AcpiRsdpData rsdp_data = {
            .revision = 2,
            .oem_id = ACPI_BUILD_APPNAME6,
            .xsdt_tbl_offset = &xsdt,
            .rsdt_tbl_offset = NULL,
        };
        build_rsdp(tables->rsdp, tables->linker, &rsdp_data);
    }

    /* Cleanup memory that's no longer used. */
    g_array_free(table_offsets, true);
}

static void acpi_ram_update(MemoryRegion *mr, GArray *data)
{
    uint32_t size = acpi_data_len(data);

    /* Make sure RAM size is correct - in case it got changed
     * e.g. by migration */
    memory_region_ram_resize(mr, size, &error_abort);

    memcpy(memory_region_get_ram_ptr(mr), data->data, size);
    memory_region_set_dirty(mr, 0, size);
}

static void microvm_acpi_build_update(void *build_opaque)
{
    AcpiBuildState *build_state = build_opaque;
    AcpiBuildTables tables;

    /* No state to update or already patched? Nothing to do. */
    if (!build_state || build_state->patched) {
        return;
    }
    build_state->patched = true;

    acpi_build_tables_init(&tables);

    microvm_acpi_build(MICROVM_MACHINE(qdev_get_machine()), &tables);

    acpi_ram_update(build_state->table_mr, tables.table_data);
    acpi_ram_update(build_state->rsdp_mr, tables.rsdp);
    acpi_ram_update(build_state->linker_mr, tables.linker->cmd_blob);

    acpi_build_tables_cleanup(&tables, true);
}


static void microvm_acpi_build_reset(void *build_opaque)
{
    AcpiBuildState *build_state = build_opaque;
    build_state->patched = false;
}

static const VMStateDescription vmstate_microvm_acpi_build = {
    .name = "microvm_acpi_build",
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (VMStateField[]) {
        VMSTATE_BOOL(patched, AcpiBuildState),
        VMSTATE_END_OF_LIST()
    },
};

void microvm_acpi_setup(MicrovmMachineState *mms)
{
    AcpiBuildTables tables;
    AcpiBuildState *build_state;
    X86MachineState *x86ms = X86_MACHINE(mms);

    if (!x86ms->fw_cfg) {
        return;
    }

    if (!acpi_enabled) {
        return;
    }

    build_state = g_malloc0(sizeof *build_state);

    acpi_build_tables_init(&tables);
    microvm_acpi_build(mms, &tables);

    /* Now expose it all to Guest */
    build_state->table_mr = acpi_add_rom_blob(microvm_acpi_build_update,
                                              build_state, tables.table_data,
                                              ACPI_BUILD_TABLE_FILE,
                                              ACPI_BUILD_TABLE_MAX_SIZE);
    assert(build_state->table_mr != NULL);

    build_state->linker_mr =
        acpi_add_rom_blob(microvm_acpi_build_update, build_state,
                          tables.linker->cmd_blob, "etc/table-loader", 0);

    fw_cfg_add_file(x86ms->fw_cfg, ACPI_BUILD_TPMLOG_FILE, tables.tcpalog->data,
                    acpi_data_len(tables.tcpalog));

    build_state->rsdp_mr = acpi_add_rom_blob(microvm_acpi_build_update,
                                             build_state, tables.rsdp,
                                             ACPI_BUILD_RSDP_FILE, 0);

    qemu_register_reset(microvm_acpi_build_reset, build_state);
    microvm_acpi_build_reset(build_state);
    vmstate_register(NULL, 0, &vmstate_microvm_acpi_build, build_state);

    /* Cleanup tables but don't free the memory: we track it
     * in build_state.
     */
    acpi_build_tables_cleanup(&tables, false);
}

static void microvm_acpi_ged_class_init(ObjectClass *class, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(class);
    AcpiDeviceIfClass *adevc = ACPI_DEVICE_IF_CLASS(class);

    dc->desc = "MicroVM ACPI Generic Event Device";

    adevc->madt_cpu = pc_madt_cpu_entry;
}

static const TypeInfo microvm_acpi_ged_info = {
    .name          = TYPE_MICROVM_ACPI_GED,
    .parent        = TYPE_ACPI_GED,
    .instance_size = sizeof(AcpiGedState),
    .class_init    = microvm_acpi_ged_class_init,
};

static void microvm_acpi_ged_register_types(void)
{
    type_register_static(&microvm_acpi_ged_info);
}

type_init(microvm_acpi_ged_register_types)
