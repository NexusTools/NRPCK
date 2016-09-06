#include "nrpck_reboot.h"

#include <string.h>

#define DISK_ID_ADDR		0x0000

char nrpck_bootloader[256];

void nrpck_reboot_init() {
	memcpy(nrpck_bootloader, (char*)0x0400, 256);
}

void nrpck_reboot(uchar disk_port) {
	memcpy((char*)0x0400, nrpck_bootloader, 0xFE);
	*((char*)0x04FF) = *((char*)DISK_ID_ADDR);
	*((char*)DISK_ID_ADDR) = disk_port;
	((void(*)(void))0x0400)();
}

void nrpck_reboot_return() {
	nrpck_reboot(nrpck_bootloader[0xFF]);
}