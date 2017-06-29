#include <nrpck.h>
#include <nrpck_drv_console.h>
#include <nrpck_drv_ccmonitor.h>
#include <nrpck_drv_matrix.h>
#include <nrpck_drv_cblock.h>
#include <nrpck_cblock.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void main() {
    long x, z;
    long x1 = 2;
    long z2 = -36;
    uchar yy = 64, id = 1;
    NRPCKDeviceRef* cblock, *display;
    NRPCKDeviceDriver* displayDriver, *cblockDriver;
    uchar width, height, y;
    
    // Initialize Modules
    nrpck_reboot_init();
    nrpck_device_init();

    // Initialize Drivers
    nrpck_init_driver_console();
    nrpck_init_driver_ccmonitor();
    nrpck_init_driver_matrix();
    nrpck_init_driver_cblock();

    // Initialize Kernel
    if (nrpck_display_boot("NexusTools 6502 Creative Block Test", 0x1, 0xF, &display, &displayDriver, &width, &height, &y, CREATIVE_TYPE, &cblock, &cblockDriver, 0) < 0) {
        nrpck_hault();
    }
    
    nrpck_device_remap(cblock);
    while(true) {
        x = -1;
        while(x!=x1) {
            z = -39;
            while(z!=-z2) {
                nrpck_cblock_drv_setblock(cblock->device, cblockDriver, x, yy-1, z, 0, 0);
                z++;
            }
            x++;
        }
        if(++yy > 100)
            yy = 64;
        x = -1;
        while(x!=x1) {
            z = -39;
            while(z!=-z2) {
                nrpck_cblock_drv_setblock(cblock->device, cblockDriver, x, yy, z, 700, id);
                z++;
            }
            x++;
        }
        if(++id > 9)
            id = 1;
        nrpck_sleep100ms();
        nrpck_sleep100ms();
        nrpck_sleep100ms();
        nrpck_sleep100ms();
        nrpck_sleep100ms();
    }
    //nrpck_device_remap(display);
    //nrpck_display_drv_print(display->device, displayDriver, "Complete!", &y, width, height);
    
    nrpck_hault();
}