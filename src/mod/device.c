#include "nrpck_min.h"
#include "nrpck_devices.h"

#include <string.h>

typedef struct NRPCKDeviceState {
    NRPCKDeviceRef ref;
    uchar knownID;
} NRPCKDeviceState;

#define RB 0x0300

uchar nrpck_driver_count = 0;
uchar nrpck_mapped_port = 0xFF;
NRPCKDeviceDriver* nrpck_drivers[0x20];
NRPCKDeviceState nrpck_port_states[0xFF];

NRPCKDeviceDriver* nrpck_installed_driver(uchar id) {
    return nrpck_drivers[id];
}
uchar nrpck_installed_driver_count() {
    return nrpck_driver_count;
}

const char* nrpck_driver_type(uchar id) {
    switch (id) {
        case 0:
            return "Nothing";

        case TEXT_DISPLAY_TYPE:
            return "Text Display";

        case TEXT_INPUT_TYPE:
            return "Text Input";

        case RETRIEVER_TYPE:
            return "Item Retriever";

        case SORTRON_TYPE:
            return "Item Sorter";

        case IO_EXPANDER_TYPE:
            return "IO Expander";

        case MODEM_TYPE:
            return "Modem";

        case DISK_TYPE:
            return "Disk";

        case POWERED_TYPE:
            return "Powered";

        default:
            return "Unknown";
    }
}

const char* nrpck_device_name(uchar id) {
    switch (id) {
        case 0:
            return "Nothing";

        case CONSOLE_ID:
            return "Console";

        case DISK_ID:
            return "Floppy Drive";

        case IO_EXPANDER_ID:
            return "IO Expander";

        case SORT_TRON_ID:
            return "Sortron";

        case RETRIEVER_ID:
            return "Retriever";

        case HARDDRIVE_ID:
            return "Hard Drive";

        case FPU_ID:
            return "FPU";

        case SUBSPACE_ACTIVATOR_ID:
            return "Activator";

        case MATRIX_ID:
            return "Console Matrix";

        case MODEM_ID:
            return "Modem";

        case HIFI_CONSOLE_ID:
            return "HiFi Console";

        case CCMONITOR_ID:
            return "ComputerCraft Monitor";
            
        case CREATIVE_BLOCK_ID:
            return "Creative Block";

        case CPU_ID:
            return "CPU";

        default:
            return "Unknown";
    }
}

uchar nrpck_device_knownid(uchar port) {
    return nrpck_port_states[port].knownID;
}

void nrpck_device_remap(NRPCKDeviceRef* ref) {
    uchar i, driverRefCount;
    uchar port = ref->rbport;
    if (port < 0xFF) {
        if (nrpck_mapped_port != port)
            nrpck_map_rb(nrpck_mapped_port = port);
        if (nrpck_port_states[port].knownID != ref->device->ID) {
            driverRefCount = 0;
            for (i = 0; i < nrpck_driver_count; i++)
                if (nrpck_drivers[i]->detect(ref->device))
                    ref->drivers[driverRefCount++] = nrpck_drivers[i];
            if (driverRefCount < 0xf)
                ref->drivers[driverRefCount] = 0;

            nrpck_port_states[port].knownID = ref->device->ID;
        }
    }
}

NRPCKDeviceDriver* nrpck_device_lookupdriver(NRPCKDeviceRef* device, uchar device_type) {
    uchar i;
    for (i = 0; i < 0xf; i++) {
        if (!device->drivers[i])
            break;
        if (device->drivers[i]->device_type == device_type)
            return device->drivers[i];
    }
    return 0;
}

schar nrpck_device_scanfordriver(uchar min_port, uchar max_port, uchar device_type, NRPCKDeviceRef** device, NRPCKDeviceDriver** driver) {
    uchar port;
    NRPCKDeviceRef* ref;
    NRPCKDeviceDriver* drv;

    for (port = min_port; port <= max_port; port++) {
        if ((drv = nrpck_device_lookupdriver(ref = nrpck_device_map(port), device_type)) && driver) {
            *device = ref;
            *driver = drv;
            return 0;
        }
    }

    return ERROR_GENERIC;
}

NRPCKDeviceRef* nrpck_device_map(unsigned char port) {
    nrpck_device_remap(&nrpck_port_states[port].ref);
    return &nrpck_port_states[port].ref;
}

void nrpck_device_register_driver(NRPCKDeviceDriver* driver) {
    if (nrpck_driver_count >= 0x20)
        return;

    nrpck_drivers[nrpck_driver_count++] = driver;
}

void nrpck_device_init() {
    uchar i;

    nrpck_enable_rb();

    for (i = 0; i < 0xFF; i++) {
        nrpck_port_states[i].ref.device = (NRPCKDevice*) RB;
        nrpck_port_states[i].ref.rbport = i;
        nrpck_port_states[i].knownID = UNPLUGGED_ID;
    }
}