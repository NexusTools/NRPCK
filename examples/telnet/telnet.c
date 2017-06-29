#include <nrpck.h>
#include <nrpck_drv_modem.h>
#include <nrpck_drv_console.h>
#include <nrpck_drv_ccmonitor.h>
#include <nrpck_drv_matrix.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TCC_IAC     0xFF
#define TCC_DONT    0xFE
#define TCC_DO      0xFD
#define TCC_WONT    0xFC
#define TCC_WILL    0xFB
#define TCC_SB      0xFA
#define TCC_GA      0xF9
#define TCC_EL      0xF8
#define TCC_EC      0xF7
#define TCC_AYT     0xF6
#define TCC_A0      0xF5
#define TCC_IP      0xF4
#define TCC_BRK     0xF3
#define TCC_DM      0xF2
#define TCC_NOP     0xF1
#define TCC_SE      0xF0

#define TCC_REQ_LOGOUT      0x12
#define TCC_REQ_TYPE        0x18
#define TCC_REQ_SIZE        0x1F
#define TCC_REQ_DET         0x20
#define TCC_REQ_OUTMRK      0x27

#define OVERCLOCK 255
#define DEBUG_MONITOR 31

extern char nrpck_bootloader[0xFF];

char str[80], address[54], temp[3];
NRPCKDeviceRef* display, *input, *modem, *display2;
NRPCKDeviceDriver* displayDriver, * modemDriver, * inputDriver, * displayDriver2;
uchar x, y, width, height, i, portindex, len, a, bright, reverse, blink, z, y2 = 0, fg, bg, sx, sy, wrap = 1, width2, height2, ry1, ry2, rh;
int read, read2;

void forward_key() {
#ifdef DEBUG_MONITOR
    char str[80];
#endif
    uchar key;
    nrpck_device_remap(input);
    key = nrpck_device_drv_nextkey(input->device, inputDriver);
    nrpck_device_remap(modem);
    switch(key) {
        case 0:
            break;
            
        case 130: // Left
#ifdef DEBUG_MONITOR
            nrpck_device_remap(display2);
            nrpck_display_drv_print(display2->device, displayDriver2, "Key typed: \\e[D", &y2, width2, height2);
            nrpck_device_remap(modem);
#endif
            nrpck_socket_drv_write(modem->device, modemDriver, "\x1B[D", 3, 0);
            break;
            
        case 128: // Up
#ifdef DEBUG_MONITOR
            nrpck_device_remap(display2);
            nrpck_display_drv_print(display2->device, displayDriver2, "Key typed: \\e[A", &y2, width2, height2);
            nrpck_device_remap(modem);
#endif
            nrpck_socket_drv_write(modem->device, modemDriver, "\x1B[A", 3, 0);
            break;
            
        case 131: // Right
#ifdef DEBUG_MONITOR
            nrpck_device_remap(display2);
            nrpck_display_drv_print(display2->device, displayDriver2, "Key typed: \\e[C", &y2, width2, height2);
            nrpck_device_remap(modem);
#endif
            nrpck_socket_drv_write(modem->device, modemDriver, "\x1B[C", 3, 0);
            break;
            
        case 129: // Down
#ifdef DEBUG_MONITOR
            nrpck_device_remap(display2);
            nrpck_display_drv_print(display2->device, displayDriver2, "Key typed: \\e[B", &y2, width2, height2);
            nrpck_device_remap(modem);
#endif
            nrpck_socket_drv_write(modem->device, modemDriver, "\x1B[B", 3, 0);
            break;
            
        default:
#ifdef DEBUG_MONITOR
            sprintf(str, "Key typed: %u", key);
            nrpck_device_remap(display2);
            nrpck_display_drv_print(display2->device, displayDriver2, str, &y2, width2, height2);
            nrpck_device_remap(modem);
#endif
            nrpck_socket_drv_write(modem->device, modemDriver, &key, 1, 0);
    }
}

void moveDown() {
    y++;
    if (y >= ry2) {
        nrpck_display_drv_scroll(display->device, displayDriver, 0, ry1+1, 0, ry1, width, rh - 1);
        nrpck_display_drv_set(display->device, displayDriver, 0, --y, ' ', width);
    }
}

void moveForward() {
    x++;
    if (x >= width) {
        x = 0;
        moveDown();
    }
}

void moveBackward() {
    if(x == 0) {
        if(y > 0) {
            y --;
            x = width - 1;
        }
    } else
        x--;
}

void main() {
    // Initialize Modules
    nrpck_reboot_init();
    nrpck_device_init();

    // Initialize Drivers
    nrpck_init_driver_modem();
    nrpck_init_driver_console();
    nrpck_init_driver_ccmonitor();
    nrpck_init_driver_matrix();

    // Initialize FileSystems

    // Initialize Kernel
    if (nrpck_display_boot("NexusTools 6502 Telnet Client", 0x1, 0xF, &display, &displayDriver, &width, &height, &y, MODEM_TYPE, &modem, &modemDriver, 0) < 0) {
#ifdef DEBUG_MONITOR
        display2 = nrpck_device_map(DEBUG_MONITOR);
        displayDriver2 = nrpck_device_lookupdriver(display2, TEXT_DISPLAY_TYPE);
        nrpck_display_drv_size(display2->device, displayDriver2, &width2, &height2);
        nrpck_display_drv_clear(display2->device, displayDriver2, a, width2, height2);
        nrpck_display_drv_print(display2->device, displayDriver2, "Boot Failed... Couldn't Find Monitor or Modem...", &y2, width2, height2);
#endif
        nrpck_hault();
    }
    
    input = display;

#ifdef OVERCLOCK
    display2 = nrpck_device_map(0);
    display2->device->data.mmu.crystal = 33;
    display2->device->data.mmu.crystal = 41;
    display2->device->data.mmu.crystal = OVERCLOCK;
#endif

    // Initialize Debug
#ifdef DEBUG_MONITOR
    display2 = nrpck_device_map(31);
    displayDriver2 = nrpck_device_lookupdriver(display2, TEXT_DISPLAY_TYPE);
    nrpck_display_drv_size(display2->device, displayDriver2, &width2, &height2);
    nrpck_display_drv_clear(display2->device, displayDriver2, a, width2, height2);
    nrpck_display_drv_print(display2->device, displayDriver2, "Connected!", &y2, width2, height2);
#endif

    nrpck_device_remap(modem);
    nrpck_socket_drv_disconnect(modem->device, modemDriver, 0);
    nrpck_device_remap(display);

    inputDriver = nrpck_device_lookupdriver(display, TEXT_INPUT_TYPE);
    if (!inputDriver) {
        nrpck_display_drv_setcursorcolour(display->device, displayDriver, 15);
        nrpck_display_print(display, displayDriver, "Scanning for Input Device", &y, width, height);
        if(nrpck_device_scanfordriver(0x01, 0x0F, TEXT_INPUT_TYPE, &input, &inputDriver) == 0) {
            sprintf(str, "Found %s #%u", nrpck_device_name(input->device->ID), input->rbport);
            nrpck_device_remap(display);
            nrpck_display_print(display, displayDriver, str, &y, width, height);
        } else
            nrpck_hault();
    }

getconnect:
    nrpck_display_readline(display, displayDriver, input, inputDriver, "telnet connect$ ", address, 50, y, width, 0, 0);
    if (address[0] == 0)
        goto getconnect;

    if (strcmp(address, "return") == 0 || strcmp(address, "exit") == 0) {
        sprintf(address, "Returning to Boot Origin... 0x%02X", nrpck_bootloader[0xFF]);
        nrpck_display_print(display, displayDriver, address, &y, width, height);
        nrpck_reboot_return();
        nrpck_display_print(display, displayDriver, "This is the Boot Origin", &y, width, height);
        goto getconnect;
    }

    portindex = 0;
    len = strlen(address);
    for (i = 0; i < len; i++) {
        if (address[i] == 0)
            break;
        if (address[i] == ':') {
            portindex = i;
            break;
        }
    }
    if (!portindex) {
        address[len] = ':';
        address[len + 1] = '2';
        address[len + 2] = '3';
        address[len + 3] = 0;
    }
    sprintf(str, "Connecting to %s", address);
    nrpck_display_print(display, displayDriver, "", &y, width, height);
    nrpck_display_print(display, displayDriver, str, &y, width, height);

    nrpck_device_remap(modem);
    nrpck_socket_drv_disconnect(modem->device, modemDriver, 0);
    if (nrpck_socket_drv_connect(modem->device, modemDriver, address, 0, 0) != 0) {
        nrpck_device_remap(display);
        nrpck_display_print(display, displayDriver, "Failed to connect to specified address", &y, width, height);
        goto getconnect;
    }

    nrpck_device_remap(display);
    //display->device->data.hificonsole.charset = 2;
    nrpck_display_print(display, displayDriver, "Connected!", &y, width, height);

    x = 0;
    ry1 = 0;
    blink = false;
    bright = false;
    reverse = false;
    ry2 = rh = height;
    nrpck_display_drv_setcursor(display->device, displayDriver, true);
    nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
    nrpck_display_drv_setcursorcolour(display->device, displayDriver, fg = 15);
    nrpck_display_drv_setcursorbgcolour(display->device, displayDriver, bg = 0);
    while (true) {
        nrpck_device_remap(modem);
        read = nrpck_socket_drv_read(modem->device, modemDriver, str, 6, forward_key);
        nrpck_device_remap(display);
        if (read < 0) {
            nrpck_display_drv_setcursorcolour(display->device, displayDriver, 15);
            nrpck_display_drv_setcursorbgcolour(display->device, displayDriver, 0);

            if(x > 0) {
                x = 0;
                moveDown();
            }
            nrpck_display_print(display, displayDriver, "Connection reset.", &y, width, height);
            goto getconnect;
        }
        for (i = 0; i < read; i++) {
            switch (str[i]) {
                case '\t':
                    x = (x + 3) % 4;
                    break;

                case 0:
                case 0xB:
                case 0xE:
                case 0xF:
                    break;

                case 0x0D:
                case 0x0C:
                    x = 0;
                    break;

                case 0x0A:
                    x = 0;
                    moveDown();
                    break;

                case TCC_IAC:
                    if (i + 1 >= read) {
                        nrpck_device_remap(modem);
                        read2 = nrpck_socket_drv_read(modem->device, modemDriver, str + read, 1, forward_key);
                        nrpck_device_remap(display);
                        if (read2 < 0) {
                            nrpck_display_drv_setcursorcolour(display->device, displayDriver, 0);
                            nrpck_display_drv_setcursorbgcolour(display->device, displayDriver, 15);
                            nrpck_display_drv_clear(display->device, displayDriver, y, width, height);

                            y = 0;
                            nrpck_display_print(display, displayDriver, "Connection reset.", &y, width, height);
                            goto getconnect;
                        }
                        read += read2;
                    }
                    portindex = i++;
                    switch (str[i]) {
                        case TCC_DO:
                            if (i + 1 >= read) {
                                nrpck_device_remap(modem);
                                read2 = nrpck_socket_drv_read(modem->device, modemDriver, str + read, 1, forward_key);
                                nrpck_device_remap(display);
                                if (read2 < 0) {
                                    nrpck_display_drv_setcursorcolour(display->device, displayDriver, 0);
                                    nrpck_display_drv_setcursorbgcolour(display->device, displayDriver, 15);
                                    nrpck_display_drv_clear(display->device, displayDriver, y, width, height);

                                    y = 0;
                                    nrpck_display_print(display, displayDriver, "Connection reset.", &y, width, height);
                                    goto getconnect;
                                }
                                read += read2;
                            }
                            switch (str[++i]) {
                                case TCC_REQ_SIZE:
                                    memcpy(address, "\xFF\xFB\x1F\xFF\xFA\x1F\x00", 7);
                                    address[7] = width;
                                    address[8] = 0;
                                    address[9] = height;
                                    memcpy(address + 10, "\xFF\xF0", 2);
                                    nrpck_device_remap(modem);
                                    nrpck_socket_drv_write(modem->device, modemDriver, address, 12, 0);
                                    break;
                                    
                                case TCC_REQ_TYPE:
                                    memcpy(address, "\xFF\xFB\x18\xFF\xFA\x18\x00xterm\xFF\xF0", 14);
                                    nrpck_device_remap(modem);
                                    nrpck_socket_drv_write(modem->device, modemDriver, address, 14, 0);
                                    break;
                                    
                                default:
                                    str[i-1] = 0xFC;
                                    nrpck_device_remap(modem);
                                    nrpck_socket_drv_write(modem->device, modemDriver, str+(i-2), 3, 0);
#ifdef DEBUG_MONITOR
                                    sprintf(address, "Unhandled Telnet Do: %X", str[i]);
                                    nrpck_device_remap(display2);
                                    nrpck_display_drv_print(display2->device, displayDriver2, address, &y2, width2, height2);
                                    nrpck_device_remap(display);
#endif
                            }
                            break;

                        case TCC_DONT:
                            if (i + 1 >= read) {
                                nrpck_device_remap(modem);
                                read2 = nrpck_socket_drv_read(modem->device, modemDriver, str + read, 1, forward_key);
                                nrpck_device_remap(display);
                                if (read2 < 0) {
                                    nrpck_display_drv_setcursorcolour(display->device, displayDriver, 0);
                                    nrpck_display_drv_setcursorbgcolour(display->device, displayDriver, 15);
                                    nrpck_display_drv_clear(display->device, displayDriver, y, width, height);

                                    y = 0;
                                    nrpck_display_print(display, displayDriver, "Connection reset.", &y, width, height);
                                    goto getconnect;
                                }
                                read += read2;
                            }
#ifdef DEBUG_MONITOR
                            switch (str[++i]) {
                                default:
                                    sprintf(address, "Unhandled Telnet Dont: %X", str[i]);
                                    nrpck_device_remap(display2);
                                    nrpck_display_drv_print(display2->device, displayDriver2, address, &y2, width2, height2);
                                    nrpck_device_remap(display);
                            }
#endif
                            break;

                        case TCC_WONT:
                            if (i + 1 >= read) {
                                nrpck_device_remap(modem);
                                read2 = nrpck_socket_drv_read(modem->device, modemDriver, str + read, 1, forward_key);
                                nrpck_device_remap(display);
                                if (read2 < 0) {
                                    nrpck_display_drv_setcursorcolour(display->device, displayDriver, 0);
                                    nrpck_display_drv_setcursorbgcolour(display->device, displayDriver, 15);
                                    nrpck_display_drv_clear(display->device, displayDriver, y, width, height);

                                    y = 0;
                                    nrpck_display_print(display, displayDriver, "Connection reset.", &y, width, height);
                                    goto getconnect;
                                }
                                read += read2;
                            }
#ifdef DEBUG_MONITOR
                            switch (str[++i]) {
                                default:
                                    sprintf(address, "Unhandled Telnet Wont: %X", str[i]);
                                    nrpck_device_remap(display2);
                                    nrpck_display_drv_print(display2->device, displayDriver2, address, &y2, width2, height2);
                                    nrpck_device_remap(display);
                            }
#endif
                            break;

                        case TCC_WILL:
                            if (i + 1 >= read) {
                                nrpck_device_remap(modem);
                                read2 = nrpck_socket_drv_read(modem->device, modemDriver, str + read, 1, forward_key);
                                nrpck_device_remap(display);
                                if (read2 < 0) {
                                    nrpck_display_drv_setcursorcolour(display->device, displayDriver, 0);
                                    nrpck_display_drv_setcursorbgcolour(display->device, displayDriver, 15);
                                    nrpck_display_drv_clear(display->device, displayDriver, y, width, height);

                                    y = 0;
                                    nrpck_display_print(display, displayDriver, "Connection reset.", &y, width, height);
                                    goto getconnect;
                                }
                                read += read2;
                            }
#ifdef DEBUG_MONITOR
                            switch (str[++i]) {
                                default:
                                    sprintf(address, "Unhandled Telnet Will: %X", str[i]);
                                    nrpck_device_remap(display2);
                                    nrpck_display_drv_print(display2->device, displayDriver2, address, &y2, width2, height2);
                                    nrpck_device_remap(display);
                            }
                            break;

                        default:
                            sprintf(address, "Unhandled Telnet Command: %X", str[i]);
                            nrpck_device_remap(display2);
                            nrpck_display_drv_print(display2->device, displayDriver2, address, &y2, width2, height2);
#endif
                    }
                    break;

                case 0x1B:
                    if (i + 2 >= read) {
                        nrpck_device_remap(modem);
                        read2 = nrpck_socket_drv_read(modem->device, modemDriver, str + read, 2, forward_key);
                        nrpck_device_remap(display);
                        if (read2 < 0) {
                            nrpck_display_drv_clear(display->device, displayDriver, y, width, height);
                            nrpck_display_drv_setcursorbgcolour(display->device, displayDriver, 15);
                            nrpck_display_drv_setcursorcolour(display->device, displayDriver, 0);

                            y = 0;
                            nrpck_display_print(display, displayDriver, "Connection reset.", &y, width, height);
                            goto getconnect;
                        }
                        read += read2;
                    }
                    i++;
                    switch (str[i]) {
                        case '>':
                        case '=':
                            // TODO: Implement?
                            break;
                        
                        case '(':
                        case ')':
                        case '*':
                        case '+':
                            if(++i >= read) {
                                nrpck_device_remap(modem);
                                if(!nrpck_socket_drv_read(modem->device, modemDriver, str, 1, forward_key)) {
                                    nrpck_device_remap(display);
                                    nrpck_display_drv_clear(display->device, displayDriver, y, width, height);
                                    nrpck_display_drv_setcursorbgcolour(display->device, displayDriver, 15);
                                    nrpck_display_drv_setcursorcolour(display->device, displayDriver, 0);

                                    y = 0;
                                    nrpck_display_print(display, displayDriver, "Connection reset.", &y, width, height);
                                    goto getconnect;
                                }
                                nrpck_device_remap(display);
                                read = 1;
                                i = 0;
                            }
                                
                            break;
                            
                        case 'M':
                            y--;
                            if(y < ry1) {
                                y ++;
                                nrpck_display_drv_scroll(display->device, displayDriver, 0, ry1, 0, ry1+1, width, rh - 1);
                                nrpck_display_drv_set(display->device, displayDriver, 0, ry1, ' ', width);
                            }
                            break;
                        
                        case '[':
                            portindex = ++i;
                            nrpck_device_remap(modem);
                            while (str[i] == ';' || str[i] == '!' || str[i] == '?' || (str[i] >= '0' && str[i] <= '9')) {
                                i++;
                                if (i >= read) {
                                    read2 = nrpck_socket_drv_read(modem->device, modemDriver, str + read, 1, forward_key);
                                    if (read2 < 0) {
                                        nrpck_device_remap(display);
                                        nrpck_display_drv_setcursorcolour(display->device, displayDriver, 0);
                                        nrpck_display_drv_setcursorbgcolour(display->device, displayDriver, 15);
                                        nrpck_display_drv_clear(display->device, displayDriver, y, width, height);

                                        y = 0;
                                        nrpck_display_print(display, displayDriver, "Connection reset.", &y, width, height);
                                        goto getconnect;
                                    }
                                    read += read2;
                                }
                            }
                            nrpck_device_remap(display);
                            switch (str[i]) {
                                case 'K':
                                    switch (str[portindex]) {
                                        case 'K':
                                        case '0':
                                            nrpck_display_drv_set(display->device, displayDriver, x, y, ' ', width - x);
                                            break;

                                        case '1':
                                            nrpck_display_drv_set(display->device, displayDriver, 0, y, ' ', x);
                                            break;

                                        case '2':
                                            nrpck_display_drv_set(display->device, displayDriver, 0, y, ' ', width);
                                            break;
                                    }
                                    break;
                                case 'J':
                                    switch (str[portindex]) {
                                        case 'J':
                                        case '0':
                                            for (a = y; a < height; a++)
                                                nrpck_display_drv_set(display->device, displayDriver, x, a, ' ', width - x);
                                            break;

                                        case '1':
                                            for (a = 0; a <= y; a++)
                                                nrpck_display_drv_set(display->device, displayDriver, x, a, ' ', width - x);
                                            break;

                                        case '2':
                                            nrpck_display_drv_clear(display->device, displayDriver, a, width, height);
                                            x = 0;
                                            y = 0;
                                            break;
                                    }
                                    break;
                                case 'f':
                                case 'H':
                                    x = 0;
                                    y = 0;
                                    if (i > portindex) {
                                        for (a = portindex; a <= i; a++) {
                                            if (str[a] == ';') {
                                                str[a] = 0;
                                                y = atoi(str + portindex) - 1;
                                                portindex = a + 1;
                                            } else if (str[a] == 'H' || str[a] == 'f') {
                                                str[a] = 0;
                                                x = atoi(str + portindex) - 1;
                                            }
                                        }
                                    }
                                    nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
                                    break;
                                case 'r':
                                    ry1 = 0;
                                    ry2 = height;
                                    if (i > portindex) {
                                        for (a = portindex; a <= i; a++) {
                                            if (str[a] == ';') {
                                                str[a] = 0;
                                                ry1 = atoi(str + portindex) - 1;
                                                portindex = a + 1;
                                            } else if (str[a] == 'r') {
                                                str[a] = 0;
                                                ry2 = atoi(str + portindex);
                                            }
                                        }
                                    }
                                    rh = ry2 - ry1;
#ifdef DEBUG_MONITOR
                                    sprintf(address, "Changed range to %u - %u (%u)", ry1+1, ry2, rh);
                                    nrpck_device_remap(display2);
                                    nrpck_display_print(display2, displayDriver2, address, &y2, width2, height2);
                                    nrpck_device_remap(display);
#endif
                                    break;
                                case 'D':
                                    z = 1;
                                    if (i > portindex) {
                                        str[i] = 0;
                                        z = atoi(str + portindex);
                                    }
#ifdef DEBUG_MONITOR
                                    sprintf(address, "Moving cursor backward by %u", z);
                                    nrpck_device_remap(display2);
                                    nrpck_display_print(display2, displayDriver2, address, &y2, width2, height2);
                                    nrpck_device_remap(display);
#endif
                                    while(z --)
                                        moveBackward();
                                    nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
                                    break;
                                case 'C':
                                    z = 1;
                                    if (i > portindex) {
                                        str[i] = 0;
                                        z = atoi(str + portindex);
                                    }
#ifdef DEBUG_MONITOR
                                    sprintf(address, "Moving cursor forward by %u", z);
                                    nrpck_device_remap(display2);
                                    nrpck_display_print(display2, displayDriver2, address, &y2, width2, height2);
                                    nrpck_device_remap(display);
#endif
                                    while(z --)
                                        moveForward();
                                    nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
                                    break;
                                case 'l':
                                    if(memcmp(str + portindex, "?7l", 3) == 0) {
#ifdef DEBUG_MONITOR
                                        nrpck_device_remap(display2);
                                        nrpck_display_print(display2, displayDriver2, "Disabled Line Wrapping", &y2, width2, height2);
                                        nrpck_device_remap(display);
#endif
                                        wrap = false;
                                    } else if(memcmp(str + portindex, "?25l", 4) == 0)
                                        nrpck_display_drv_setcursor(display->device, displayDriver, false);
#ifdef DEBUG_MONITOR
                                    else {
                                        str[i] = 0;
                                        sprintf(address, "Unhandled <ESC>[%sl", str + portindex);
                                        nrpck_device_remap(display2);
                                        nrpck_display_print(display2, displayDriver2, address, &y2, width2, height2);
                                        nrpck_device_remap(display);
                                    }
#endif
                                    break;
                                case 'h':
                                    if(memcmp(str + portindex, "?7h", 3) == 0) {
#ifdef DEBUG_MONITOR
                                        nrpck_device_remap(display2);
                                        nrpck_display_print(display2, displayDriver2, "Enabled Line Wrapping", &y2, width2, height2);
                                        nrpck_device_remap(display);
#endif
                                        wrap = true;
                                    } else if(memcmp(str + portindex, "?25h", 4) == 0)
                                        nrpck_display_drv_setcursor(display->device, displayDriver, true);
#ifdef DEBUG_MONITOR
                                    else {
                                        str[i] = 0;
                                        sprintf(address, "Unhandled <ESC>[%sh", str + portindex);
                                        nrpck_device_remap(display2);
                                        nrpck_display_print(display2, displayDriver2, address, &y2, width2, height2);
                                        nrpck_device_remap(display);
                                    }
#endif
                                    break;
                                case 'd':
                                    str[i] = 0;
                                    y = atoi(str + portindex) - 1;
                                    nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
                                    break;
                                case 'G':
                                    str[i] = 0;
                                    x = atoi(str + portindex) - 1;
                                    nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
                                    break;
                                case '@':
                                    z = 1;;
                                    if(i > portindex) {
                                        str[i] = 0;
                                        z = atoi(str + portindex);
                                    }
                                    // TODO: Optimize
                                    while(z--) {
                                        nrpck_display_drv_set(display->device, displayDriver, x, y, ' ', 1);
                                        moveForward();
                                    }
                                    nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
                                    break;
                                case 's':
#ifdef DEBUG_MONITOR
                                    nrpck_device_remap(display2);
                                    nrpck_display_print(display2, displayDriver2, "Saving cursor position", &y2, width2, height2);
                                    nrpck_device_remap(display);
#endif
                                    sx = x;
                                    sy = y;
                                    break;
                                case 'u':
#ifdef DEBUG_MONITOR
                                    nrpck_device_remap(display2);
                                    nrpck_display_print(display2, displayDriver2, "Restoring cursor position", &y2, width2, height2);
                                    nrpck_device_remap(display);
#endif
                                    x = sx;
                                    y = sy;
                                    nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
                                    break;
                                case 'A':
                                    z = 1;
                                    if (i > portindex) {
                                        str[i] = 0;
                                        z = atoi(str + portindex);
                                    }
#ifdef DEBUG_MONITOR
                                    sprintf(address, "Moving cursor up by %u", z);
                                    nrpck_device_remap(display2);
                                    nrpck_display_print(display2, displayDriver2, address, &y2, width2, height2);
                                    nrpck_device_remap(display);
#endif
                                    if(z > y)
                                        y = 0;
                                    else
                                        y -= z;
                                    nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
                                    break;
                                case 'B':
                                    z = 1;
                                    if (i > portindex) {
                                        str[i] = 0;
                                        z = atoi(str + portindex);
                                    }
#ifdef DEBUG_MONITOR
                                    sprintf(address, "Moving cursor down by %u", z);
                                    nrpck_device_remap(display2);
                                    nrpck_display_print(display2, displayDriver2, address, &y2, width2, height2);
                                    nrpck_device_remap(display);
#endif
                                    while(z --)
                                        moveDown();
                                    nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
                                    break;
                                case 'M':
                                    z = 1;
                                    if (i > portindex) {
                                        str[i] = 0;
                                        z = atoi(str + portindex);
                                    }
                                    if(z + y > height)
                                        z = height - y;
#ifdef DEBUG_MONITOR
                                    sprintf(address, "Deleting %u lines", z);
                                    nrpck_device_remap(display2);
                                    nrpck_display_print(display2, displayDriver2, address, &y2, width2, height2);
                                    nrpck_device_remap(display);
#endif
                                    for(a=0; a<z; a++)
                                        nrpck_display_drv_set(display->device, displayDriver, 0, y+a, ' ', width);
                                    break;
                                case 'S':
                                    z = 1;
                                    if (i > portindex) {
                                        str[i] = 0;
                                        z = atoi(str + portindex);
                                    }
#ifdef DEBUG_MONITOR
                                    sprintf(address, "Scrolling up %u lines", z);
                                    nrpck_device_remap(display2);
                                    nrpck_display_print(display2, displayDriver2, address, &y2, width2, height2);
                                    nrpck_device_remap(display);
#endif
                                    nrpck_display_drv_scroll(display->device, displayDriver, 0, ry1+z, 0, ry1, width, rh - z);
                                    for(a=ry1+(rh-z); a<ry2; a++)
                                        nrpck_display_drv_set(display->device, displayDriver, 0, a, ' ', width);
                                    y -= z;
                                    break;
                                case 'T':
                                    z = 1;
                                    if (i > portindex) {
                                        str[i] = 0;
                                        z = atoi(str + portindex);
                                    }
#ifdef DEBUG_MONITOR
                                    sprintf(address, "Scrolling down %u lines", z);
                                    nrpck_device_remap(display2);
                                    nrpck_display_print(display2, displayDriver2, address, &y2, width2, height2);
                                    nrpck_device_remap(display);
#endif
                                    nrpck_display_drv_scroll(display->device, displayDriver, 0, ry1, 0, ry1+z, width, rh - z);
                                    z += ry1;
                                    for(a=ry1; a<z; a++)
                                        nrpck_display_drv_set(display->device, displayDriver, 0, a, ' ', width);
                                    y -= z;
                                    break;
                                case 'm':
                                    if (portindex == i) {
                                        blink = false;
                                        bright = false;
                                        reverse = false;
                                        nrpck_display_drv_setcursorcolour(display->device, displayDriver, fg = 15);
                                        nrpck_display_drv_setcursorbgcolour(display->device, displayDriver, bg = 0);
                                    } else
                                        for (a = portindex; a <= i; a++) {
                                            if (str[a] == 'm' || str[a] == ';') {
                                                str[a] = 0;
                                                z = atoi(str + portindex);
//#ifdef DEBUG_MONITOR
//                                                sprintf(address, "Changing: %s %u", str + portindex, z);
//                                                nrpck_device_remap(display2);
//                                                nrpck_display_drv_print(display2->device, displayDriver2, address, &y2, width2, height2);
//                                                nrpck_device_remap(display);
//#endif
                                                switch (z) {
                                                    case 0: // reset
                                                        reverse = false;
                                                        bright = false;
                                                        blink = false;
                                                        nrpck_display_drv_setcursorbgcolour(display->device, displayDriver, bg = 0);
                                                    case 37: //white
                                                    case 39: //white
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 8 : 7])(display->device, fg = 15);
                                                        break;

                                                    case 1:
                                                        bright = true;
                                                        break;

                                                    case 2:
                                                        bright = false;
                                                        break;

                                                    case 5:
                                                        blink = true;
                                                        break;

                                                    case 7:
                                                        z = bg;
                                                        bg = fg;
                                                        nrpck_display_drv_setcursorbgcolour(display->device, displayDriver, bg);
                                                        nrpck_display_drv_setcursorcolour(display->device, displayDriver, fg = z);
                                                        reverse = true;
                                                        break;

                                                    case 8: // hidden
                                                        break;

                                                    case 30: // Black
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 8 : 7])(display->device, z = (bright ? 8 : 0));
                                                        if(reverse)
                                                            bg = z;
                                                        else
                                                            fg = z;
                                                        break;

                                                    case 31: // Red
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 8 : 7])(display->device, z = (bright ? 12 : 4));
                                                        if(reverse)
                                                            bg = z;
                                                        else
                                                            fg = z;
                                                        break;

                                                    case 32: // Green
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 8 : 7])(display->device, z = (bright ? 10 : 2));
                                                        if(reverse)
                                                            bg = z;
                                                        else
                                                            fg = z;
                                                        break;

                                                    case 33: // Yellow
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 8 : 7])(display->device, z = (bright ? 14 : 6));
                                                        if(reverse)
                                                            bg = z;
                                                        else
                                                            fg = z;
                                                        break;

                                                    case 34: // Blue
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 8 : 7])(display->device, z = (bright ? 9 : 1));
                                                        if(reverse)
                                                            bg = z;
                                                        else
                                                            fg = z;
                                                        break;

                                                    case 35: // Magenta
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 8 : 7])(display->device, z = (bright ? 13 : 5));
                                                        if(reverse)
                                                            bg = z;
                                                        else
                                                            fg = z;
                                                        break;

                                                    case 36: // Cyan
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 8 : 7])(display->device, z = (bright ? 11 : 3));
                                                        if(reverse)
                                                            bg = z;
                                                        else
                                                            fg = z;
                                                        break;

                                                    case 40: // Black
                                                    case 49: // Black
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 7 : 8])(display->device, z = (bright ? 8 : 0));
                                                        if(reverse)
                                                            fg = z;
                                                        else
                                                            bg = z;
                                                        break;

                                                    case 41: // Red
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 7 : 8])(display->device, z = (bright ? 12 : 4));
                                                        if(reverse)
                                                            fg = z;
                                                        else
                                                            bg = z;
                                                        break;

                                                    case 42: // Green
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 7 : 8])(display->device, z = (bright ? 10 : 2));
                                                        if(reverse)
                                                            fg = z;
                                                        else
                                                            bg = z;
                                                        break;

                                                    case 43: // Yellow
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 7 : 8])(display->device, z = (bright ? 14 : 6));
                                                        if(reverse)
                                                            fg = z;
                                                        else
                                                            bg = z;
                                                        break;

                                                    case 44: // Blue
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 7 : 8])(display->device, z = (bright ? 9 : 1));
                                                        if(reverse)
                                                            fg = z;
                                                        else
                                                            bg = z;
                                                        break;

                                                    case 45: // Magenta
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 7 : 8])(display->device, z = (bright ? 13 : 5));
                                                        if(reverse)
                                                            fg = z;
                                                        else
                                                            bg = z;
                                                        break;

                                                    case 46: // Cyan
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 7 : 8])(display->device, z = (bright ? 11 : 3));
                                                        if(reverse)
                                                            fg = z;
                                                        else
                                                            bg = z;
                                                        break;

                                                    case 47:
                                                        ((void(*)(NRPCKDevice*, uchar))displayDriver->methods[reverse ? 7 : 8])(display->device, z = 15);
                                                        if(reverse)
                                                            fg = z;
                                                        else
                                                            bg = z;
                                                        break;

#ifdef DEBUG_MONITOR
                                                    default:
                                                        str[i] = 0;
                                                        sprintf(address, "Unhandled <ESC>[m Command: %s", str + portindex);
                                                        nrpck_device_remap(display2);
                                                        nrpck_display_drv_print(display2->device, displayDriver2, address, &y2, width2, height2);
                                                        nrpck_device_remap(display);
#endif
                                                }
                                                portindex = a+1;
                                            }
                                        }
//#ifdef DEBUG_MONITOR
//                                        sprintf(address, "Changed foreground/background: %u %u", fg, bg);
//                                        nrpck_device_remap(display2);
//                                        nrpck_display_drv_print(display2->device, displayDriver2, address, &y2, width2, height2);
//                                        nrpck_device_remap(display);
//#endif
                                    break;
                                    
                                case 'P':
                                    str[i] = 0;
                                    z = atoi(str + portindex);
#ifdef DEBUG_MONITOR
                                    sprintf(address, "Erasing %u characters", z);
                                    nrpck_device_remap(display2);
                                    nrpck_display_drv_print(display2->device, displayDriver2, address, &y2, width2, height2);
                                    nrpck_device_remap(display);
#endif
                                    nrpck_display_drv_set(display->device, displayDriver, x, y, ' ', z);
                                    break;

                                case 'n':
                                    switch (str[portindex]) {
                                        case '6':
                                            len = sprintf(address, "\x1B[%u;%uR", x, y);
                                            nrpck_device_remap(modem);
                                            nrpck_socket_drv_write(modem->device, modemDriver, address, len, 0);
                                            nrpck_device_remap(display);
                                            break;
                                    }
                                    break;
#ifdef DEBUG_MONITOR
                                default:
                                    temp[0] = str[i];
                                    temp[1] = 0;
                                    str[i] = 0;
                                    sprintf(address, "Unhandled <ESC>[%s%s", str + portindex, temp);
                                    nrpck_device_remap(display2);
                                    nrpck_display_drv_print(display2->device, displayDriver2, address, &y2, width2, height2);
                                    nrpck_device_remap(display);
#endif


                            }
                            break;

                        case ']':
                            nrpck_device_remap(modem);
                            do {
                                if(++i >= read) {
                                    if(!nrpck_socket_drv_read(modem->device, modemDriver, str, 1, forward_key)) {
                                        nrpck_device_remap(display);
                                        nrpck_display_drv_clear(display->device, displayDriver, y, width, height);
                                        nrpck_display_drv_setcursorbgcolour(display->device, displayDriver, 15);
                                        nrpck_display_drv_setcursorcolour(display->device, displayDriver, 0);

                                        y = 0;
                                        nrpck_display_print(display, displayDriver, "Connection reset.", &y, width, height);
                                        goto getconnect;
                                    }
                                    read = 1;
                                    i = 0;
                                }
                            } while (str[i] != 7);
                            nrpck_device_remap(display);
                            break;
                            
                        case '7':
#ifdef DEBUG_MONITOR
                            nrpck_device_remap(display2);
                            nrpck_display_print(display2, displayDriver2, "Saving cursor position", &y2, width2, height2);
                            nrpck_device_remap(display);
#endif
                            sx = x;
                            sy = y;
                            break;
                        case '8':
#ifdef DEBUG_MONITOR
                            nrpck_device_remap(display2);
                            nrpck_display_print(display2, displayDriver2, "Restoring cursor position", &y2, width2, height2);
                            nrpck_device_remap(display);
#endif
                            x = sx;
                            y = sy;
                            nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
                            break;

#ifdef DEBUG_MONITOR
                        default:
                            temp[0] = str[i];
                            temp[1] = 0;
                            sprintf(address, "Unhandled <ESC> Command: %X %s", str[i], temp);
                            nrpck_device_remap(display2);
                            nrpck_display_drv_print(display2->device, displayDriver2, address, &y2, width2, height2);
                            nrpck_device_remap(display);
#endif
                    }
                    break;
                    
                case 8:
                    if(x == 0) {
                        if(y > 0) {
                            x = width-1;
                            y --;
                        } else
                            x = 0;
                    } else 
                        x--;
                    //nrpck_display_drv_set(display->device, displayDriver, x, y, ' ', 1);
                    nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
                    break;
                case 7:
#ifdef DEBUG_MONITOR
                    nrpck_device_remap(display2);
                    nrpck_display_drv_print(display2->device, displayDriver2, "!!BELL!!", &y2, width2, height2);
                    nrpck_device_remap(display);
#endif
                    break;

                default:
                    if(str[i] < 32 || str[i] > 128) {
#ifdef DEBUG_MONITOR
                        sprintf(address, "Unhandled code: %u", str[i]);
                        nrpck_device_remap(display2);
                        nrpck_display_drv_print(display2->device, displayDriver2, address, &y2, width2, height2);
                        nrpck_device_remap(display);
#endif
                    } else {
                        nrpck_display_drv_set(display->device, displayDriver, x, y, str[i], 1);
                        moveForward();
                        nrpck_display_drv_movecursor(display->device, displayDriver, x, y);
                    }
            }
        }
    }
}