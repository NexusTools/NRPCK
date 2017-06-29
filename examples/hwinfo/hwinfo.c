#include <nrpck.h>
#include <nrpck_drv_console.h>
#include <nrpck_drv_ccmonitor.h>
#include <nrpck_drv_matrix.h>
#include <nrpck_drv_ioexpander.h>
#include <nrpck_drv_sortron.h>
#include <nrpck_drv_modem.h>
#include <nrpck_drv_cblock.h>
#include <nrpck_drv_disk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void main() {
    uint i;
    uchar d;
    char str[80];
    NRPCKDeviceRef* deviceRef, *display;
    NRPCKDeviceDriver* displayDriver;
    uchar width, height, y;
    
    // Initialize Modules
    nrpck_reboot_init();
    nrpck_device_init();

    // Initialize Drivers
    nrpck_init_driver_console();
    nrpck_init_driver_ccmonitor();
    nrpck_init_driver_matrix();
    nrpck_init_driver_modem();
    nrpck_init_driver_disk();
    nrpck_init_driver_sortron();
    nrpck_init_driver_ioexpander();
    nrpck_init_driver_cblock();

    // Initialize Kernel
    if (nrpck_display_boot("NexusTools 6502 RPC Hardware Info", 0x1, 0xF, &display, &displayDriver, &width, &height, &y, 0) < 0) {
        nrpck_hault();
    }
    
    nrpck_display_drv_print(display->device, displayDriver, "Scanning RedBus for Hardware...", &y, width, height);
    for(i = 0; i <= 0xFF; i++) {
        deviceRef = nrpck_device_map(i);
        if(deviceRef->device->ID != UNPLUGGED_ID) {
            NRPCKDeviceDriver* driver;
            sprintf(str, " %u) %s", i, nrpck_device_name(deviceRef->device->ID));
            nrpck_device_remap(display);
            nrpck_display_drv_print(display->device, displayDriver, str, &y, width, height);
            for(d = 0; d < 0xF; d++) {
                driver = deviceRef->drivers[d];
                if(!driver)
                    break;
                sprintf(str, "   - %s", driver->name);
                nrpck_display_drv_print(display->device, displayDriver, str, &y, width, height);
            }
            nrpck_display_drv_print(display->device, displayDriver, "", &y, width, height);
        }
    }
    
    nrpck_hault();
}