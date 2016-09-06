#include "nrpck.h"
#include "nrpck_drv_console.h"

#include <string.h>

NRPCKDeviceDriver nrpck_drv_console_display;
NRPCKDeviceDriver nrpck_drv_console_keyboard;

bool nrpck_drv_console_detect(NRPCKDevice* device) {
	return device->ID == CONSOLE_ID;
}

void nrpck_drv_console_set(NRPCKDevice* device, uchar x, uchar y, uchar c, uchar count) {
	device->data.console.line = y;
	memset(device->data.console.display+x, c, count);
}

void nrpck_drv_console_line(NRPCKDevice* device, uchar x, uchar y, const char* str, uchar len) {
	device->data.console.line = y;
	memcpy(device->data.console.display+x, str, len);
}

bool nrpck_drv_console_hascursor(NRPCKDevice*) {
	return true;
}

void nrpck_drv_console_movecursor(NRPCKDevice* device, uchar x, uchar y) {
	device->data.console.cursor_x = x;
	device->data.console.cursor_y = y;
}

void nrpck_drv_console_setcursor(NRPCKDevice* device, bool visible) {
	device->data.console.cursor_mode = visible ? 2 : 0;
}

void nrpck_drv_console_size(NRPCKDevice*, uchar* width, uchar* height) {
	if(width)
		*width = 80;
	if(height)
		*height = 50;
}

void nrpck_drv_console_initcolours(NRPCKDevice* device, uchar* palette) {
	uint i, count;
	
	count = (*palette)*3;
	palette++;
	for(i=0;i<count;i+=3) {
		device->data.console.red = *palette;
		device->data.console.green = *(palette+1);
		device->data.console.blue = *(palette+2);
		device->data.console.special_command = 1;
		while(device->data.console.special_command == 1) {nrpck_sleep10ms();}
	}
}

void nrpck_drv_console_setcursorcolour(NRPCKDevice* device, uchar colour) {
	device->data.console.blit_colour = colour;
}

void nrpck_drv_console_setcursorbgcolour(NRPCKDevice* device, uchar colour) {
	device->data.console.blit_bgcolour = colour;
}

schar nrpck_drv_console_scrollup(NRPCKDevice* device, uchar by) {
	uchar i;
	
	i = 50-by;
	device->data.console.blit_start_x = 0;
    device->data.console.blit_start_y = by;
    device->data.console.blit_offset_x = 0;
    device->data.console.blit_offset_y = 0;
    device->data.console.blit_width = 80;
    device->data.console.blit_height = i;
    device->data.console.blit_mode = 3;
    while (device->data.console.blit_mode != 0) {nrpck_sleep10ms();}
	
	return 0;
}

uchar nrpck_drv_console_nextkey(NRPCKDevice* device) {
	uchar key;
	if (device->data.console.kb_pos != device->data.console.kb_start) {
		key = device->data.console.kb_key;
		device->data.console.kb_start++;
		return key;
	}
	return 0;
}

void nrpck_drv_console_display_describe(NRPCKDevice*, char* buffer) {
	strcpy(buffer, "80x50 Text Display Device");
}

void nrpck_drv_console_input_describe(NRPCKDevice*, char* buffer) {
	strcpy(buffer, "Buffered Text Input Device");
}

void nrpck_init_driver_console() {
	nrpck_drv_console_display.name = "RedPower Console Display Driver";
	nrpck_drv_console_display.device_type = TEXT_DISPLAY_TYPE;
	nrpck_drv_console_display.detect = nrpck_drv_console_detect;
	nrpck_drv_console_display.methods[0] = nrpck_drv_console_set;
	nrpck_drv_console_display.methods[1] = nrpck_drv_console_line;
	nrpck_drv_console_display.methods[2] = nrpck_drv_console_hascursor;
	nrpck_drv_console_display.methods[3] = nrpck_drv_console_movecursor;
	nrpck_drv_console_display.methods[4] = nrpck_drv_console_setcursor;
	nrpck_drv_console_display.methods[5] = nrpck_drv_console_size;
	nrpck_drv_console_display.methods[6] = nrpck_drv_console_initcolours;
	nrpck_drv_console_display.methods[7] = nrpck_drv_console_setcursorcolour;
	nrpck_drv_console_display.methods[8] = nrpck_drv_console_setcursorbgcolour;
	nrpck_drv_console_display.methods[9] = nrpck_drv_console_scrollup;
	nrpck_drv_console_display.methods[0xF] = nrpck_drv_console_display_describe;
	nrpck_device_register_driver(&nrpck_drv_console_display);
	
	nrpck_drv_console_keyboard.name = "RedPower Console Keyboard Driver";
	nrpck_drv_console_keyboard.device_type = TEXT_INPUT_TYPE;
	nrpck_drv_console_keyboard.detect = nrpck_drv_console_detect;
	nrpck_drv_console_keyboard.methods[0] = nrpck_drv_console_nextkey;
	nrpck_drv_console_keyboard.methods[0xF] = nrpck_drv_console_input_describe;
	nrpck_device_register_driver(&nrpck_drv_console_keyboard);
}
