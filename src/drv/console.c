#include "nrpck.h"
#include "nrpck_drv_console.h"

#include <string.h>

NRPCKDeviceDriver nrpck_drv_console_display;
NRPCKDeviceDriver nrpck_drv_hificonsole_display;
NRPCKDeviceDriver nrpck_drv_hificonsole_mouse;
NRPCKDeviceDriver nrpck_drv_console_keyboard;

bool nrpck_drv_console_detect(NRPCKDevice* device) {
    return device->ID == CONSOLE_ID;
}

bool nrpck_drv_hificonsole_detect(NRPCKDevice* device) {
    return device->ID == HIFI_CONSOLE_ID;
}

bool nrpck_drv_eitherconsole_detect(NRPCKDevice* device) {
    return device->ID == CONSOLE_ID || device->ID == HIFI_CONSOLE_ID;
}

void nrpck_drv_console_set(NRPCKDevice* device, uchar x, uchar y, uchar c, uchar count) {
    device->data.console.line = y;
    memset(device->data.console.display + x, c, count);
}

void nrpck_drv_console_line(NRPCKDevice* device, uchar x, uchar y, const char* str, uchar len) {
    device->data.console.line = y;
    memcpy(device->data.console.display + x, str, len);
}

uchar nrpck_drv_console_capabilities(NRPCKDevice*) {
    return DISPLAY_CAPABILITY_COLOUR | DISPLAY_CAPABILITY_CURSOR;
}

void nrpck_drv_console_movecursor(NRPCKDevice* device, uchar x, uchar y) {
    device->data.console.cursor_x = x;
    device->data.console.cursor_y = y;
}

void nrpck_drv_console_setcursormode(NRPCKDevice* device, uchar mode) {
    device->data.console.cursor_mode = mode;
}

void nrpck_drv_console_size(NRPCKDevice*, uchar* width, uchar* height) {
    if (width)
        *width = 80;
    if (height)
        *height = 50;
}

void nrpck_drv_hificonsole_size(NRPCKDevice*, uchar* width, uchar* height) {
    if (width)
        *width = 160;
    if (height)
        *height = 64;
}

void nrpck_drv_console_initcolours(NRPCKDevice* device, uchar* palette) {
    uint i, count;

    count = (*palette)*3;
    palette++;
    for (i = 0; i < count; i += 3) {
        device->data.console.blit_colour = i;
        device->data.console.red = *palette;
        device->data.console.green = *(palette + 1);
        device->data.console.blue = *(palette + 2);
        device->data.console.special_command = 1;
        while (device->data.console.special_command == 1) {
            nrpck_sleep10ms();
        }
    }
}

void nrpck_drv_hificonsole_initcolours(NRPCKDevice* device, uchar* palette) {
    uint i, count;

    count = (*palette)*3;
    palette++;
    device->data.hificonsole.charset = 0;
    for (i = 0; i < count; i += 3) {
        device->data.hificonsole.blit_colour = i;
        device->data.hificonsole.red = *palette;
        device->data.hificonsole.green = *(palette + 1);
        device->data.hificonsole.blue = *(palette + 2);
        device->data.hificonsole.special_command = 1;
        while (device->data.hificonsole.special_command == 1) {
            nrpck_sleep10ms();
        }
    }
}

void nrpck_drv_console_setcursorcolour(NRPCKDevice* device, uchar colour) {
    device->data.console.blit_colour = colour;
}

void nrpck_drv_console_setcursorbgcolour(NRPCKDevice* device, uchar colour) {
    device->data.console.blit_bgcolour = colour;
}

schar nrpck_drv_console_scroll(NRPCKDevice* device, uchar sx, uchar sy, uchar dx, uchar dy, uchar w, uchar h) {
    device->data.console.blit_start_x = sx;
    device->data.console.blit_start_y = sy;
    device->data.console.blit_offset_x = dx;
    device->data.console.blit_offset_y = dy;
    device->data.console.blit_width = w;
    device->data.console.blit_height = h;
    device->data.console.blit_mode = sy > dy ? 3 : 4;
    while (device->data.console.blit_mode != 0) {
        nrpck_sleep10ms();
    }

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

void nrpck_drv_hificonsole_display_describe(NRPCKDevice*, char* buffer) {
    strcpy(buffer, "160x64 Text Display Device");
}

void nrpck_drv_hificonsole_mouse_describe(NRPCKDevice*, char* buffer) {
    strcpy(buffer, "160x64 Mouse Input Device");
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
    nrpck_drv_console_display.methods[2] = nrpck_drv_console_capabilities;
    nrpck_drv_console_display.methods[3] = nrpck_drv_console_movecursor;
    nrpck_drv_console_display.methods[4] = nrpck_drv_console_setcursormode;
    nrpck_drv_console_display.methods[5] = nrpck_drv_console_size;
    nrpck_drv_console_display.methods[6] = nrpck_drv_console_initcolours;
    nrpck_drv_console_display.methods[7] = nrpck_drv_console_setcursorcolour;
    nrpck_drv_console_display.methods[8] = nrpck_drv_console_setcursorbgcolour;
    nrpck_drv_console_display.methods[9] = nrpck_drv_console_scroll;
    nrpck_drv_console_display.methods[0xF] = nrpck_drv_console_display_describe;
    nrpck_device_register_driver(&nrpck_drv_console_display);

    nrpck_drv_hificonsole_display.name = "RedPower HiFi Console Display Driver";
    nrpck_drv_hificonsole_display.device_type = TEXT_DISPLAY_TYPE;
    nrpck_drv_hificonsole_display.detect = nrpck_drv_hificonsole_detect;
    nrpck_drv_hificonsole_display.methods[0] = nrpck_drv_console_set;
    nrpck_drv_hificonsole_display.methods[1] = nrpck_drv_console_line;
    nrpck_drv_hificonsole_display.methods[2] = nrpck_drv_console_capabilities;
    nrpck_drv_hificonsole_display.methods[3] = nrpck_drv_console_movecursor;
    nrpck_drv_hificonsole_display.methods[4] = nrpck_drv_console_setcursormode;
    nrpck_drv_hificonsole_display.methods[5] = nrpck_drv_hificonsole_size;
    nrpck_drv_hificonsole_display.methods[6] = nrpck_drv_hificonsole_initcolours;
    nrpck_drv_hificonsole_display.methods[7] = nrpck_drv_console_setcursorcolour;
    nrpck_drv_hificonsole_display.methods[8] = nrpck_drv_console_setcursorbgcolour;
    nrpck_drv_hificonsole_display.methods[9] = nrpck_drv_console_scroll;
    nrpck_drv_hificonsole_display.methods[0xF] = nrpck_drv_hificonsole_display_describe;
    nrpck_device_register_driver(&nrpck_drv_hificonsole_display);

    nrpck_drv_hificonsole_mouse.name = "RedPower HiFi Console Mouse Driver";
    nrpck_drv_hificonsole_mouse.device_type = MOUSE_INPUT_TYPE;
    nrpck_drv_hificonsole_mouse.detect = nrpck_drv_hificonsole_detect;
    nrpck_drv_hificonsole_mouse.methods[0xF] = nrpck_drv_hificonsole_mouse_describe;
    nrpck_device_register_driver(&nrpck_drv_hificonsole_mouse);

    nrpck_drv_console_keyboard.name = "RedPower Console Keyboard Driver";
    nrpck_drv_console_keyboard.device_type = TEXT_INPUT_TYPE;
    nrpck_drv_console_keyboard.detect = nrpck_drv_eitherconsole_detect;
    nrpck_drv_console_keyboard.methods[0] = nrpck_drv_console_nextkey;
    nrpck_drv_console_keyboard.methods[0xF] = nrpck_drv_console_input_describe;
    nrpck_device_register_driver(&nrpck_drv_console_keyboard);
}
