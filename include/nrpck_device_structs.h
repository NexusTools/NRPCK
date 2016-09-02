#ifndef NRPCK_DEVICE_STRUCTS
#define NRPCK_DEVICE_STRUCTS

#include "nrpck_types.h"

typedef struct Console {
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

	unsigned int padding; 

	char display[0x50];
} Console;

typedef struct IOExpander {
	uint input;
	uint output;
	uchar changes;
} IOExpander;

typedef enum {
	DISK_IDLE 	  	   = 0,
	DISK_READ_NAME 	   = 1,
	DISK_WRITE_NAME    = 2,
	DISK_READ_SERIAL   = 3,
	DISK_READ 		   = 4,
	DISK_WRITE 		   = 5,
	DISK_READ_OFFSET   = 6,
	DISK_WRITE_OFFSET  = 7,
	DISK_FAIL 		   = 0xFF
} DiskCommand;

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
		char all[0xA0];
		struct ModemDuplex {
			char in[0x50];
			char out[0x50];
		} duplex;
	} buffer;
	schar command;
	uchar mode;
	uchar len;
	uchar index;
	uchar rbuffer;
	uchar wbuffer;
} Modem;

#define SORT_SUCCEEDED	= 0
#define SORT_COUNT		= 1
#define SORT_LOAD		= 2
#define SORT_PULL		= 3
#define SORT_MATCH		= 4
#define SORT_FAILED		= 0xFF

typedef enum {
	PAINT_NONE    = 0,
	PAINT_WHITE 	= 1,
	PAINT_ORANGE 	= 2,
	PAINT_MAGENTA = 3,
	PAINT_LBLUE	= 4,
	PAINT_YELLOW 	= 5,
	PAINT_LIME 	= 6,
	PAINT_PINK 	= 7,
	PAINT_GRAY    = 8,
	PAINT_LGRAY   = 9,
	PAINT_CYAN 	= 10,
	PAINT_PURPLE 	= 11,
	PAINT_BLUE 	= 12,
	PAINT_BROWN 	= 13,
	PAINT_GREEN 	= 14,
	PAINT_RED 	= 15,
	PAINT_BLACK 	= 16
} SortItemColor;

typedef struct Sortron {
	uchar command;
	uchar count;
	uint slot;
	uint itemid;
	uint unused;
	uint damage;
	uint maxdamage;
	uchar outputColor;
	uchar inputColor;
} Sortron;

#endif