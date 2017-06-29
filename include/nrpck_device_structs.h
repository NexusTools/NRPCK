#ifndef NRPCK_DEVICE_STRUCTS
#define NRPCK_DEVICE_STRUCTS

#include "nrpck_types.h"

typedef struct Console {
    unsigned char line;
    unsigned char cursor_x;
    unsigned char cursor_y;
    unsigned char cursor_mode;

    char kb_start;
    char kb_pos;
    char kb_key;

    char blit_mode;
    char blit_start_x;
    char blit_start_y;
    char blit_offset_x;
    char blit_offset_y;
    char blit_width;
    char blit_height;

    unsigned char blit_colour;
    unsigned char special_command;

    char display[0xA0];
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char blit_bgcolour;
    unsigned char mouseX;
    unsigned char mouseY;
    unsigned char pressedX;
    unsigned char pressedY;
    unsigned char charset;
    unsigned char releasedX;
    unsigned char releasedY;
    char scrollWheel;
} Console;

typedef struct HiFiConsole {
    char line;
    char cursor_x;
    char cursor_y;
    char cursor_mode;

    char kb_start;
    char kb_pos;
    char kb_key;

    char blit_mode;
    char blit_start_x;
    char blit_start_y;
    char blit_offset_x;
    char blit_offset_y;
    char blit_width;
    char blit_height;

    uchar blit_colour;
    uchar special_command;

    char display[0xA0];
    uchar red, green, blue, blit_bgcolour;

    uchar mouseX;
    uchar mouseY;
    uchar clickedX;
    uchar clickedY;
    uchar charset;
} HiFiConsole;

typedef struct IOExpander {
    uint input;
    uint output;
    uchar changes;
} IOExpander;

typedef struct Disk {
    char sector[0x80];
    unsigned int sector_num;
    unsigned char command;
    unsigned char offset;
    unsigned char length;
} Disk;

typedef struct Activator {
    char buffer[0x80];
    unsigned char command;
    long x;
    unsigned char y;
    long z;
    char slot;
    unsigned long charge;
    unsigned char nbttype;
    unsigned char side;
} Activator;

typedef struct Matrix {
    char buffer[0x80];
    unsigned char command;
    unsigned char width;
    unsigned char height;
    unsigned char blitx;
    unsigned char blity;
} Matrix;

typedef struct ConsoleRouter {
    unsigned char length;
    unsigned char indexes[0xf];
    unsigned char width[0xf];
    unsigned char height[0xf];
    unsigned char left[0xf];
    unsigned char top[0xf];
    unsigned char right[0xf];
    unsigned char bottom[0xf];
    unsigned char cwidth;
    unsigned char cheight;
    unsigned char effect;
} ConsoleRouter;

typedef struct Modem {
    union ModemBuffer {
        char raw[0xF0];

        struct DuplexBuffer {
            char in[0x78];
            char out[0x78];
        } duplex;
    } buffer;
    schar incmd;
    schar outcmd;
    uchar inlen;
    uchar outlen;
    uchar index;
    uchar inout; // 0 - in, 1 - out, 2 - duplex
} Modem;

typedef struct SortronItem {
    uint id;
    uint damage;
    uint maxdamage;
} SortronItem;

typedef struct Sortron {
    uchar command;
    uchar count;
    uint slot;
    uint item_id;
    uint unused;
    uint item_damage;
    uint item_maxdamage;
    uchar outputColor;
    uchar inputColor;
} Sortron;

typedef struct CCMonitor {
    char buffer[0xF0];
    char cmd;
    char color;
    char background;
    char scale;
    int x;
    int y;
    char cblink;
    int clickedx;
    int clickedy;
    char click_counter;
} CCMonitor;

typedef struct CBlock {
    char cmd;
    long x;
    uchar y;
    long z;
    uint blockId;
    uint blockMeta;
} CBlock;

#endif
