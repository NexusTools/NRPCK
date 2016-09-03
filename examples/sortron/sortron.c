#include <nrpck.h>
#include <nrpck_drv_sortron.h>
#include <nrpck_drv_console.h>
#include <nrpck_drv_matrix.h>
#include <string.h>
#include <stdio.h>

char str[80];
NRPCKDeviceRef* display;
NRPCKDeviceDriver* displayDriver = 0;
uchar y = 0, width, height;

void print0(const char* line, uchar len) {
	nrpck_display_print0(line, len, &y, width, height, display, displayDriver);
}
void print(const char* line) {
	print0(line, strlen(line));
}

void main() {
    uchar i, color;
	NRPCKDeviceRef* sortron;
	NRPCKDeviceDriver* sortronDriver;
	SortronItem item;
	uint slots;
	
	// Initialize Modules
	nrpck_device_init();
	
	// Initialize Drivers
	nrpck_init_driver_sortron();
	nrpck_init_driver_console();
	nrpck_init_driver_matrix();
	
	// Initialize FileSystems
	
	// Initialize Kernel
	if(nrpck_display_boot("Testing App", 0x1, 0xF, &display, &displayDriver, &width, &height, &y, SORTRON_TYPE, &sortron, &sortronDriver, 0) < 0) {
		print("Haulting");
		nrpck_hault();
	}
	
	// Code
	print("Counting Number of Sortron Slots");
	nrpck_device_remap(sortron);
	slots = nrpck_sortron_drv_count(sortron->device, sortronDriver);
	nrpck_device_remap(display);
	sprintf(str, "Has %u slots", slots);
	print(str);
	print("");
	
	// Run
	for(i=0; i<slots; i++){
		sprintf(str, "Loading Item in Slot %02X", i);
		print(str);
		
		nrpck_device_remap(sortron);
		if(nrpck_sortron_drv_load(sortron->device, sortronDriver, i, &item) > -1) {
			nrpck_device_remap(display);
			sprintf(str, "Item: %u (%u/%u)", item.id, item.damage, item.maxdamage);
			print(str);
			
			if(item.id == 351) {
				print("Sending to Green Pipe");
				color = SORTRON_PAINT_LIME;
			} else if(item.id > 0) {
				print("Sending to Magenta Pipe");
				color = SORTRON_PAINT_MAGENTA;
			} else
				continue;
		} else {
			nrpck_device_remap(display);
			print("An error occured");
		}
		nrpck_device_remap(sortron);
		nrpck_sortron_drv_pull(sortron->device, sortronDriver, 64, color);
		nrpck_device_remap(display);
	}
	nrpck_device_remap(display);
	
	print0("", 0);
	print("Haulting");
	nrpck_hault();
}
