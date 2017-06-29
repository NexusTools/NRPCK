#include "nrpck_min.h"

#include <string.h>

uint nrpck_ram_available;
uint nrpck_ram_size;
uint nrpck_ram_end;

void nrpck_noop() {}

void nrpck_init() {
    uint nrpck_ram_start, t;

    *((char*) 0x04FF) = 0xFF;
    t = nrpck_ram_start = nrpck_bss_end();
    do {
        *((unsigned char*) t) = 127;
        if (*((unsigned char*) t) != 127)
            break;

        t = t + 0x100;
    } while (t > nrpck_ram_start);
    nrpck_ram_end = t - 0x100;
    if (nrpck_ram_end < nrpck_ram_start)
        nrpck_ram_end = nrpck_ram_start;
    nrpck_ram_available = nrpck_ram_size = (nrpck_ram_end - nrpck_ram_start);
}

uint nrpck_memory_available() {
    return nrpck_ram_available;
}
uint nrpck_ram_alloc = 0;

void* nrpck_memory_alloc(unsigned char size) {
    void* ptr;
    ptr = (void*) (nrpck_bss_end() + nrpck_ram_alloc);
    nrpck_ram_alloc += size;
    return ptr;
}

void nrpck_memory_dealloc(void*) {

}