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

#define CONSOLE_ROUTER_EFFECT_NONE		0x00
#define CONSOLE_ROUTER_EFFECT_CENTERED	0x01
#define CONSOLE_ROUTER_EFFECT_FILLBG	0x02
#define CONSOLE_ROUTER_EFFECT_ALL		0xFF