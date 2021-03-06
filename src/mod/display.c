#include "nrpck.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CONSOLE_ID_ADDR  0x0001

void nrpck_display_drv_print0(NRPCKDevice* display, NRPCKDeviceDriver* displayDriver, const char* line, uchar len, uchar* y, uchar width, uchar height) {
    if (len > width) {
        nrpck_display_drv_line(display, displayDriver, 0, *y, line, width);
        *y = *y + 1;
        if (*y >= height) {
            if (nrpck_display_drv_scroll(display, displayDriver, 0, 1, 0, 0, width, height-1) < 0)
                *y = 0;
            else
                *y = height - 1;
        }
        nrpck_display_drv_set(display, displayDriver, 0, *y, ' ', width);
        nrpck_display_drv_print0(display, displayDriver, line + width, len - width, y, width, height);
    } else {
        nrpck_display_drv_line(display, displayDriver, 0, *y, line, len);
        *y = *y + 1;
        if (*y >= height) {
            if (nrpck_display_drv_scroll(display, displayDriver, 0, 1, 0, 0, width, height-1) < 0)
                *y = 0;
            else
                *y = height - 1;
        }
        nrpck_display_drv_set(display, displayDriver, 0, *y, ' ', width);
    }
}

void nrpck_display_drv_print(NRPCKDevice* display, NRPCKDeviceDriver* displayDriver, const char* line, uchar* y, uchar width, uchar height) {
    nrpck_display_drv_print0(display, displayDriver, line, strlen(line), y, width, height);
}

uchar nrpck_display_drv_bootscreen(NRPCKDevice* display, NRPCKDeviceDriver* displayDriver, const char* banner, uchar* widthOut, uchar* heightOut) {
    uchar width, height, y;
    char str[80];

    y = 0;
    nrpck_display_drv_reset(display, displayDriver);
    nrpck_display_drv_size(display, displayDriver, &width, &height);

    nrpck_display_drv_setcursorcolour(display, displayDriver, 13);
    nrpck_display_drv_print(display, displayDriver, banner, &y, width, height);
    nrpck_display_drv_setcursorcolour(display, displayDriver, 5);
    nrpck_display_drv_print(display, displayDriver, "Powered by NRPCK " NRPCK_KERNEL_VERSION, &y, width, height);
    nrpck_display_drv_print(display, displayDriver, "", &y, width, height);

    nrpck_display_drv_setcursorcolour(display, displayDriver, 11);
    sprintf(str, "Available Memory: %uB", nrpck_memory_available());
    nrpck_display_drv_print(display, displayDriver, str, &y, width, height);
    sprintf(str, "Display Device: %s", nrpck_device_name(display->ID));
    nrpck_display_drv_print(display, displayDriver, str, &y, width, height);
    sprintf(str, "Display Driver: %s", displayDriver->name);
    nrpck_display_drv_print(display, displayDriver, str, &y, width, height);
    nrpck_display_drv_print(display, displayDriver, "", &y, width, height);

    if (widthOut)
        *widthOut = width;
    if (heightOut)
        *heightOut = height;

    nrpck_display_drv_setcursorcolour(display, displayDriver, 0);
    return y;
}

schar nrpck_display_boot(const char* banner, uchar port_start, uchar port_end, NRPCKDeviceRef** displayOut, NRPCKDeviceDriver** displayDriverOut, uchar* widthOut, uchar* heightOut, uchar* yOut, ...) {
    NRPCKDeviceDriver* displayDriver, *inputDriver, **refDriver;
    NRPCKDeviceRef* display, **ref;
    uchar device_type, width, height;
    char str[80];
    va_list ap;
    uchar i, y;

    nrpck_init();
    if ((displayDriver = nrpck_device_lookupdriver(display = nrpck_device_map(*((char*) CONSOLE_ID_ADDR)), TEXT_DISPLAY_TYPE))) {
        if (displayDriverOut)
            *displayDriverOut = displayDriver;
        if (displayOut)
            *displayOut = display;
        goto founddisplay;
    }
    for (i = port_start; i <= port_end; i++) {
        if (i == *((char*) CONSOLE_ID_ADDR))
            continue;
        if ((displayDriver = nrpck_device_lookupdriver(display = nrpck_device_map(i), TEXT_DISPLAY_TYPE))) {
            if (displayDriverOut)
                *displayDriverOut = displayDriver;
            if (displayOut)
                *displayOut = display;
            goto founddisplay;
        }
    }

    if (yOut)
        *yOut = y;
    return ERROR_GENERIC;
founddisplay:
    y = nrpck_display_drv_bootscreen(display->device, displayDriver, banner, &width, &height);
    nrpck_device_remap(display);
    inputDriver = nrpck_device_lookupdriver(display, TEXT_INPUT_TYPE);
    if (inputDriver) {
        nrpck_display_drv_setcursorcolour(display->device, displayDriver, 11);
        nrpck_display_drv_print(display->device, displayDriver, "Text Input Supported by Display!", &y, width, height);
    } else {
        nrpck_display_drv_setcursorcolour(display->device, displayDriver, 12);
        nrpck_display_drv_print(display->device, displayDriver, "Text Input is not Supported by Display...", &y, width, height);
    }
    nrpck_display_drv_setcursorcolour(display->device, displayDriver, 11);
    sprintf(str, "Scanning Bus IDs #%u to #%u", port_start, port_end);
    nrpck_display_drv_print(display->device, displayDriver, str, &y, width, height);
    nrpck_display_drv_print(display->device, displayDriver, "", &y, width, height);

    va_start(ap, yOut);
    while (true) {
        device_type = va_arg(ap, uchar);
        if (!device_type)
            break;

        ref = va_arg(ap, NRPCKDeviceRef**);
        if (!ref)
            break;

        refDriver = va_arg(ap, NRPCKDeviceDriver**);
        if (!refDriver)
            break;

        *refDriver = 0;
        sprintf(str, "Scanning for %s Compatible Device", nrpck_driver_type(device_type));
        nrpck_display_drv_setcursorcolour(display->device, displayDriver, 11);
        nrpck_display_drv_print(display->device, displayDriver, str, &y, width, height);
        for (i = port_start; i <= port_end; i++)
            if (*refDriver = nrpck_device_lookupdriver(*ref = nrpck_device_map(i), device_type)) {
                sprintf(str, "Found %s #%u!", nrpck_device_name((*ref)->device->ID), (*ref)->rbport);
                nrpck_device_remap(display);
                nrpck_display_drv_setcursorcolour(display->device, displayDriver, 10);
                nrpck_display_drv_print(display->device, displayDriver, str, &y, width, height);
                nrpck_display_drv_print(display->device, displayDriver, "", &y, width, height);
                break;
            }

        if (!*refDriver) {
            char num[4];
            nrpck_device_remap(display);
            if (inputDriver) {
                uchar p;
                nrpck_display_drv_setcursorcolour(display->device, displayDriver, 5);
                nrpck_display_readline(display, displayDriver, display, inputDriver, "Device Not Found... Enter Device Port #", num, 4, y++, width, &nrpck_display_readline_hex, 0);
                p = atoi(num);
                
                if (p > 0) {
                    sprintf(str, "Scanning #%u", p);
                    nrpck_display_drv_print(display->device, displayDriver, str, &y, width, height);
                    if (*refDriver = nrpck_device_lookupdriver(*ref = nrpck_device_map(p), device_type)) {
                        sprintf(str, "Using %s #%u!", nrpck_device_name((*ref)->device->ID), (*ref)->rbport);
                        nrpck_device_remap(display);
                        nrpck_display_drv_setcursorcolour(display->device, displayDriver, 10);
                        nrpck_display_drv_print(display->device, displayDriver, str, &y, width, height);
                        nrpck_display_drv_print(display->device, displayDriver, "", &y, width, height);
                        continue;
                    } else {
                        uchar i, c = 0;
                        uchar count = nrpck_installed_driver_count();
                        nrpck_device_remap(display);
                        nrpck_display_drv_print(display->device, displayDriver, "No Compatible Drivers Detected on Device...", &y, width, height);
                        nrpck_display_drv_print(display->device, displayDriver, "", &y, width, height);
                        nrpck_display_drv_print(display->device, displayDriver, "Please Select a Driver:", &y, width, height);
                        for(i=0; i<count; i++) {
                            NRPCKDeviceDriver* driver = nrpck_installed_driver(i);
                            if(driver->device_type != device_type)
                                continue;
                            sprintf(str, " #%u) %s", i+1, driver->name);
                            nrpck_display_drv_print(display->device, displayDriver, str, &y, width, height);
                            c++;
                        }
                        
                        if(c > 0) {
                            nrpck_display_readline(display, displayDriver, display, inputDriver, "Enter Driver ID #", num, 4, y++, width, &nrpck_display_readline_hex, 0);
                            p = atoi(num);
                            if(p > 0 && p <= 0x20) {
                                *refDriver = nrpck_installed_driver(p-1);
                                nrpck_device_remap(display);
                                sprintf(str, "Using %s #%u!", (*refDriver)->name, (*ref)->rbport);
                                nrpck_display_drv_setcursorcolour(display->device, displayDriver, 10);
                                nrpck_display_drv_print(display->device, displayDriver, str, &y, width, height);
                                nrpck_display_drv_print(display->device, displayDriver, "", &y, width, height);
                                continue;
                            }
                        } else
                            nrpck_display_drv_print(display->device, displayDriver, "No compatible drivers built into kernel...", &y, width, height);
                    }
                }
            } else {
                nrpck_display_drv_setcursorcolour(display->device, displayDriver, 4);
                nrpck_display_drv_print(display->device, displayDriver, "No Compatible Device Found...", &y, width, height);
            }

            if (widthOut)
                *widthOut = width;
            if (heightOut)
                *heightOut = height;
            if (yOut)
                *yOut = y;
            va_end(ap);
            return ERROR_GENERIC;
        }
    }
    va_end(ap);

    if (widthOut)
        *widthOut = width;
    if (heightOut)
        *heightOut = height;
    if (yOut)
        *yOut = y;

    nrpck_display_drv_setcursorcolour(display->device, displayDriver, 15);
    return 0;
}

bool nrpck_display_readline_anything(char, char*, uchar) {
    return true;
}

bool nrpck_display_readline_hex(char a, char*, uchar) {
    return (a >= 'a' && a <= 'f') || (a >= '0' && a <= '9');
}

uchar nrpck_display_readline(NRPCKDeviceRef* display, NRPCKDeviceDriver* displayDriver, NRPCKDeviceRef* input, NRPCKDeviceDriver* inputDriver, const char* prefix, char* buffer, uchar bufferSize, uchar y, uchar width, bool(*cantype)(char, char*, uchar), void(*tick)(void)) {
    uchar bufferLimit, left, right, key, len, x;

    if (!tick)
        tick = nrpck_sleep10ms;
    if (!cantype)
        cantype = nrpck_display_readline_anything;
    bufferLimit = bufferSize - 1;
    left = strlen(prefix);
    right = width - left;
    buffer[0] = 0;
    len = 0;

    x = left;
    nrpck_device_remap(display);
    nrpck_display_drv_line(display->device, displayDriver, 0, y, prefix, left);
    nrpck_display_drv_movecursor(display->device, displayDriver, left, y);
    nrpck_display_drv_setcursor(display->device, displayDriver, true);
    while (1) {
        nrpck_device_remap(input);
        if (key = nrpck_device_drv_nextkey(input->device, inputDriver)) {
            nrpck_device_remap(display);
            switch (key) {
                case 13:
                    buffer[len] = 0;
                    nrpck_display_drv_setcursor(display->device, displayDriver, false);
                    return len;

                case 8:
                    if (len > 0) {
                        nrpck_display_drv_set(display->device, displayDriver, --x, y, ' ', 1);
                        nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
                        len--;
                    }
                    break;

                default:
                    if (len >= bufferLimit || !cantype(key, buffer, len))
                        break;
                    buffer[len++] = key;
                    nrpck_display_drv_set(display->device, displayDriver, x++, y, key, 1);
                    if (x >= 80)
                        x--;
                    nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
            }
        }
        tick();
    }
}