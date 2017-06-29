#include <nrpck.h>
#include <nrpck_drv_disk.h>
#include <nrpck_drv_ioexpander.h>
#include <nrpck_drv_sortron.h>
#include <nrpck_drv_console.h>
#include <nrpck_drv_matrix.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct BlockEntry {
    uint id;
    uint damage;
    uchar color;
    bool allsubids;
} BlockEntry;

char str[80];

uint sectorpos = 0xFFFF;
uchar sectorsize;
uchar blocklength;
NRPCKDeviceRef* display;
NRPCKDeviceDriver* displayDriver = 0;
BlockEntry mappedBlocks[99];
uchar y = 0, width, height;

void print0(const char* line, uchar len) {
    nrpck_display_print0(display, displayDriver, line, len, &y, width, height);
}

void print(const char* line) {
    print0(line, strlen(line));
}

bool lower_numbers_only(char cha, char*, uchar) {
    return (cha >= '0' && cha <= '9') || (cha >= 'a' && cha <= 'z');
}

bool yn_only(char cha, char*, uchar) {
    return cha == 'y' || cha == 'n';
}

void main() {
    uchar i, a, id;
    SortronItem item;
    NRPCKDeviceRef* sortron, * disk, * disk2, * ioexpander;
    NRPCKDeviceDriver* sortronDriver, * diskDriver, * disk2Driver, * inputDriver, * ioexpanderDriver;
    uint code, found;
    ulong count;

    // Initialize Modules
    nrpck_device_init();

    // Initialize Drivers
    nrpck_init_driver_disk();
    nrpck_init_driver_ioexpander();
    nrpck_init_driver_sortron();
    nrpck_init_driver_console();
    nrpck_init_driver_matrix();

    // Initialize FileSystems

    // Initialize Kernel
    if (nrpck_display_boot("NexusTools Base Sorting Application", 0x1, 0xF, &display, &displayDriver, &width, &height, &y, SORTRON_TYPE, &sortron, &sortronDriver, IO_EXPANDER_TYPE, &ioexpander, &ioexpanderDriver, DISK_TYPE, &disk, &diskDriver, 0) < 0)
        nrpck_hault();

    disk2Driver = 0;
    print("Looking for Second Disk Compatible Device");
    for (i = disk->rbport + 1; i <= 0xF; i++) {
        disk2 = nrpck_device_map(i);
        disk2Driver = nrpck_device_lookupdriver(disk2, DISK_TYPE);
        if (disk2Driver) {
            sprintf(str, "Found %s on port 0x%02X!", nrpck_device_name(disk2->device->ID), disk2->rbport);
            nrpck_device_remap(display);
            print(str);
            print("");
            break;
        }
    }

    if (!disk2Driver) {
        nrpck_device_remap(display);
        print("Could not find second compatible drive");
        nrpck_hault();
    }

    nrpck_device_remap(display);
    print("Resetting IO Expander");
    nrpck_device_remap(ioexpander);
    nrpck_ioexpander_drv_write(ioexpander->device, ioexpanderDriver, 0);

    nrpck_device_remap(disk2);
    sectorsize = nrpck_drive_drv_sectorsize(disk2->device, disk2Driver);
    blocklength = sectorsize / sizeof (BlockEntry);

    nrpck_device_remap(display);
    sprintf(str, "Disk uses sectors of %uB", sectorsize);
    print(str);
    sprintf(str, "Loading up to %u blocks per sector", blocklength);
    print(str);
    print("");

    inputDriver = nrpck_device_lookupdriver(display, TEXT_INPUT_TYPE);

    if (inputDriver) {
        while ((id = nrpck_device_drv_nextkey(display->device, inputDriver)) > 0) {
            if (id == 13) {
readcommand:
                nrpck_display_readline(display, displayDriver, display, inputDriver, "command$ ", str, 9, y, width, lower_numbers_only, 0);
                if (strcmp("list", str) == 0) {
                    sectorpos = 0;
                    found = 0;
                    count = 0;
                    do {
                        nrpck_device_remap(disk2);
                        if (nrpck_drive_drv_read(disk2->device, disk2Driver, sectorpos, 0, (char*) mappedBlocks, sizeof (BlockEntry) * blocklength) < 0) {
                            memset(mappedBlocks, 0, sizeof (BlockEntry) * blocklength);
                            nrpck_device_remap(display);
                            print("Disk Error");
                        }
                        for (a = 0; a < blocklength; a++) {
                            if (mappedBlocks[a].id > 0) {
                                count = count + 1;
                                nrpck_device_remap(display);
                                if (mappedBlocks[a].allsubids)
                                    sprintf(str, "Item %u:* is mapped to chest %u", mappedBlocks[a].id, mappedBlocks[a].color);
                                else
                                    sprintf(str, "Item %u:%u is mapped to chest %u", mappedBlocks[a].id, mappedBlocks[a].damage, mappedBlocks[a].color);
                                print(str);
                            } else {
                                found = sectorpos;
                                sectorpos = 0xFFFF;
                                break;
                            }
                        };
                        sectorpos++;
                    } while (sectorpos > 0);
                    if (found)
                        sectorpos = found;
                    else
                        sectorpos = 0xFFFF;

                    nrpck_device_remap(display);
                    print("");

                    sprintf(str, "Counted %lu entries", count);
                    print(str);
                    print("");

                    goto readcommand;
                }
            }

        }
    }

    // Code
    print("Counting Number of Sortron Slots");
    nrpck_device_remap(sortron);
    code = nrpck_sortron_drv_count(sortron->device, sortronDriver);
    nrpck_device_remap(display);
    sprintf(str, "Has %u slots", code);
    print(str);
    print("");

    //nrpck_hault();

    // Run
    while (true) {
        for (i = 0; i < code; i++) {
            sprintf(str, "Loading Item in Slot %02X", i);
            print(str);

            nrpck_device_remap(sortron);
            if (nrpck_sortron_drv_load(sortron->device, sortronDriver, i, &item) > -1) {
                sprintf(str, "Item: %u:%u", item.id, item.damage, item.damage);
                nrpck_device_remap(display);
                print(str);

                if (item.id > 0) {
                    if (sectorpos > 0) {
                        sectorpos = 0;
                        print("Looking up on Disk");
                        nrpck_device_remap(disk2);
                        if (nrpck_drive_drv_read(disk2->device, disk2Driver, 0, 0, (char*) mappedBlocks, sizeof (BlockEntry) * blocklength) < 0) {
                            memset(mappedBlocks, 0, sizeof (BlockEntry) * blocklength);
                            nrpck_device_remap(display);
                            print("Disk Error");
                        }
                        nrpck_device_remap(display);
                    }
                    do {
                        for (a = 0; a < blocklength; a++) {
                            if (mappedBlocks[a].id == item.id && (mappedBlocks[a].allsubids || mappedBlocks[a].damage == item.damage)) {
                                sprintf(str, "Pushing Item to %u chest", mappedBlocks[a].color);
                                print(str);

                                found = sectorpos;
                                nrpck_device_remap(sortron);
                                nrpck_sortron_drv_pull(sortron->device, sortronDriver, 64, mappedBlocks[a].color);
                                sectorpos = 0xFFFF;
                                break;
                            } else if (mappedBlocks[a].id == 0) {
                                found = sectorpos;
                                sectorpos = 0xFFFF;
                                break;
                            }
                        };
                        sectorpos++;
                        if (sectorpos > 0) {
                            nrpck_device_remap(disk2);
                            if (nrpck_drive_drv_read(disk2->device, disk2Driver, sectorpos, 0, (char*) mappedBlocks, sizeof (BlockEntry) * blocklength) < 0) {
                                memset(mappedBlocks, 0, sizeof (BlockEntry) * blocklength);
                                nrpck_device_remap(display);
                                print("Disk Error");
                            }
                            nrpck_device_remap(display);
                        }
                    } while (sectorpos > 0);

                    if (a < blocklength) {
                        sectorpos = found;
                        if (!mappedBlocks[a].id) {
                            nrpck_device_remap(display);
                            print("Item not in database");
                            nrpck_device_remap(sortron);
                            nrpck_sortron_drv_pull(sortron->device, sortronDriver, 64, SORTRON_PAINT_RED);
                            nrpck_device_remap(display);
                            print("The item has been transferred to the review chest");
                            print("Please enter a chest destination to assign");
                            if (!inputDriver) {
                                print("Your display does not support input.");
                                break;
                            }

getinput:
                            nrpck_device_remap(ioexpander);
                            nrpck_ioexpander_drv_write(ioexpander->device, ioexpanderDriver, 0xFFFF);
                            nrpck_display_readline(display, displayDriver, display, inputDriver, "chest id$ ", str, 9, y, width, lower_numbers_only, 0);
                            if (str[0] == 0)
                                goto getinput;
                            if (strcmp("wood", str) == 0 || strcmp("useful", str) == 0)
                                id = 1;
                            else if (strcmp("extended", str) == 0)
                                id = 2;
                            else if (strcmp("ingot", str) == 0 || strcmp("powder", str) == 0 || strcmp("gem", str) == 0)
                                id = 3;
                            else if (strcmp("mob", str) == 0 || strcmp("drop", str) == 0)
                                id = 4;
                            else if (strcmp("ore", str) == 0)
                                id = 5;
                            else if (strcmp("redstone", str) == 0 || strcmp("lapriz", str) == 0 || strcmp("coal", str) == 0)
                                id = 6;
                            else if (strcmp("vanilla", str) == 0)
                                id = 7;
                            else if (strcmp("junk", str) == 0 || strcmp("cobble", str) == 0 || strcmp("gravel", str) == 0)
                                id = 12;
                            else {
                                if (str[1] == 0 && str[0] >= '0' && str[0] <= '9')
                                    id = atoi(str);
                                else if (str[2] == 0 && str[0] >= '0' && str[0] <= '1' && str[1] >= '0' && str[1] <= '9')
                                    id = atoi(str);
                                else {
                                    print("Unknown chest identifier");
                                    goto getinput;
                                }
                            }
                            if (id == 0)
                                goto getinput;

                            nrpck_display_readline(display, displayDriver, display, inputDriver, "specific to damage [Ny]$ ", str, 2, y, width, yn_only, 0);
                            if (str[0] == 0 || str[1] == 'n')
                                mappedBlocks[a].allsubids = true;
                            else {
                                mappedBlocks[a].allsubids = false;
                                mappedBlocks[a].damage = item.damage;
                            }
                            mappedBlocks[a].id = item.id;
                            mappedBlocks[a].color = id;

                            print("Writing to disk...");
                            nrpck_device_remap(ioexpander);
                            nrpck_ioexpander_drv_write(ioexpander->device, ioexpanderDriver, 0);
                            nrpck_device_remap(disk2);
                            if (nrpck_drive_drv_fastwrite(disk2->device, diskDriver, sectorpos, 0, (char*) mappedBlocks, sizeof (BlockEntry) * blocklength) < 0) {
                                nrpck_device_remap(display);
                                print("Disk Error");
                            }

                            sprintf(str, "Mapped to Chest %u", id);
                            nrpck_device_remap(display);
                            print(str);
                        }
                    } else
                        sectorpos = 0xFFFF;
                }
            } else {
                nrpck_device_remap(display);
                print("An error occured");
            }
        }
    }
    //nrpck_device_remap(display);

    //print0("", 0);
    //print("Haulting");
    //nrpck_hault();
}