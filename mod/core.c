#include "nrpck_min.h"

#include <string.h>

extern ulong _DATA_SIZE__;

uchar oninitlength = 0;
uint nrpck_ram_start;
uint nrpck_ram_available;
uint nrpck_ram_size;
uint nrpck_ram_end;
union NRPCKCoreData {
	void (*oninit[0x20])(void);
} nrpck_core;

void nrpck_oninit(void (*oninit)(void)) {
	if(oninitlength >= 0x20)
		return;
	
	nrpck_core.oninit[oninitlength++] = oninit;
}

void nrpck_init() {
	uchar i;
	uint t;
	t = nrpck_ram_start = _DATA_SIZE__;
	do {
		*((unsigned char*)t) = 127;
		if(*((unsigned char*)t) != 127)
			break;
		
		t = t + 0x100;
	} while(t > nrpck_ram_start);
	nrpck_ram_end = t - 0x100;
	if(nrpck_ram_end < nrpck_ram_start)
		nrpck_ram_end = nrpck_ram_start;
	nrpck_ram_available = nrpck_ram_size = (nrpck_ram_end - nrpck_ram_start);
	for(i=0;i<oninitlength;i++)
		nrpck_core.oninit[i]();
}

uint nrpck_memory_available() {
	return nrpck_ram_available;
}
uint nrpck_ram_alloc = 0;
void* nrpck_memory_alloc(unsigned char size) {
	void* ptr;
	ptr = (void*)(nrpck_ram_start+nrpck_ram_alloc);
	nrpck_ram_alloc += size;
	return ptr;
}
void nrpck_memory_dealloc(void*) {
	
}