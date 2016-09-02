#include "nrpck_min.h"
#include "nrpck_devices.h"

#include <string.h>
#include <redbus.h>

typedef struct NRPCKDeviceState {
	NRPCKDeviceRef ref;
	uchar knownID;
} NRPCKDeviceState;

#define RB 0x0300

uchar nrpck_driver_count = 0;
uchar nrpck_mapped_port = 0xFF;
NRPCKDeviceDriver* nrpck_drivers[0x20];
NRPCKDeviceState nrpck_port_states[0xFF];

const char* nrpck_device_name(uchar id) {
	switch(id) {
		case 0:
			return "Nothing";

		case CONSOLE_ID:
			return "Console";

		case DISK_ID:
			return "Floppy Drive";

		case IO_EXPANDER_ID:
			return "IO Expander";

		case SORT_TRON_ID:
			return "Sorttron";

		case RETRIEVER_ID:
			return "Retriever";

		case HARD_DRIVE_ID:
			return "Hard Drive";

		case FPU_ID:
			return "FPU";

		case SUBSPACE_ACTIVATOR_ID:
			return "Activator";

		case MATRIX_ID:
			return "Console Matrix";
			
		case MODEM_ID:
			return "Modem";

		case CPU_ID:
			return "CPU";

		default:
			return "Unknown";
	}
}

void nrpck_device_remap(NRPCKDeviceRef* ref) {
	uchar i, driverRefCount;
	uchar port = ref->rbport;
	if(port < 0xFF) {
		if(nrpck_mapped_port != port)
			rb_map_device(nrpck_mapped_port = port);
		if(nrpck_port_states[port].knownID != ref->device->ID) {
			driverRefCount = 0;
			for(i=0; i<nrpck_driver_count; i++)
				if(nrpck_drivers[i]->detect(ref->device))
					ref->drivers[driverRefCount ++] = nrpck_drivers[i];
			if(driverRefCount < 0xf)
				ref->drivers[driverRefCount] = 0;
			
			nrpck_port_states[port].knownID = ref->device->ID;
		}
	}
}

NRPCKDeviceDriver* nrpck_device_lookupdriver(NRPCKDeviceRef* device, uchar device_type) {
	uchar i;
	for(i=0; i<0xf; i++) {
		if(!device->drivers[i])
			break;
		if(device->drivers[i]->device_type == device_type)
			return device->drivers[i];
	}
	return 0;
}

schar nrpck_device_scanfordriver(uchar min_port, uchar max_port, uchar device_type, NRPCKDeviceRef** device, NRPCKDeviceDriver** driver) {
	uchar port;
	NRPCKDeviceRef* ref;
	NRPCKDeviceDriver* drv;
	
	for(port = min_port; port <= max_port; port++) {
		ref = nrpck_device_map(port);
		drv = nrpck_device_lookupdriver(ref, device_type);
		if(driver) {
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
	if(nrpck_driver_count >= 0x20)
		return;
	
	nrpck_drivers[nrpck_driver_count++] = driver;
}

void nrpck_device_init() {
	uchar i;
	
	rb_set_window((void*)RB);
	rb_enable();
	
	for(i=0; i<0xFF; i++) {
		nrpck_port_states[i].ref.device = (NRPCKDevice*)RB;
		nrpck_port_states[i].ref.rbport = i;
		nrpck_port_states[i].knownID = UNPLUGGED_ID;
	}
}