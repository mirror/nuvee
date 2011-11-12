#define s8 signed char
#define s16 signed short
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int





const char* PS2_LibraryName = "nuvee ps2 usb";

// USB = 0x0003
const unsigned char version = 0x0003;
const unsigned char revision = 0;
const unsigned char build = 2;

const unsigned char PS2_VERSION = ((0<<8) | 2 | (0<<24));



// PCSX2

// PS2EgetLibType returns (may be OR'd)
#define PS2E_LT_USB				0x20		// -=[ OBSOLETE ]=-
#define PS2E_USB_VERSION  0x03	// -=[ OBSOLETE ]=-



#define HIDMOUSE_API_RAWINPUT 0
#define HIDMOUSE_API_DIRECTINPUT 1

int hidmouse_api;
int snap_mouse_cursor = 1;




// Blade_Arma - pcsxr interface
void (CALLBACK *gpuCursor)(int, int, int) = 0;

void USBhandleHID( LPARAM in_device_handle );
LRESULT WINAPI nuvee_key_winproc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);



HINSTANCE hInstance_app;
HWND hWnd_app;
HWND hWnd_ps1_app;

WNDPROC pOldWinProc;


char ini_path[1024] = "inis";
char temp_str[1024];



// PCSX1 - two plugins = 1 shared dll
static int init_done;
static int rawinput_active;
static int mouse_active;
static int rawinput_count;

// PCSX2 - one plugin = 2 ports
FILE *fp_debug;
FILE *fp_emu;


typedef void (*USBcallback)(int cycles);
typedef int  (*USBhandler)(void);

// ==================================
// ==================================
// ==================================

struct ohci_hcca *hcca;
struct ohci_regs *ohci;
struct usb_device_list *usb_dev;


u8 *ram;
USBcallback USBirq;


u8 usbregs[ 0x10000 ];
u8 usbdevice[ 0x20000 ];


int usb_logging;
int lightgun_swap;
int offscreen_shot;

// freeze modes:
#define FREEZE_LOAD			0
#define FREEZE_SAVE			1
#define FREEZE_SIZE			2

typedef struct {
	int size;
	char *data;
} freezeData;


typedef struct {
	u32 version;

	u8 usbregs[ 0x10000 ];
	u8 usbdevice[ 0x20000 ];
} USBfreezeData;

// ==================================
// ==================================
// ==================================

// USB PADs

#define PAD_TYPE_NONE					0
#define PAD_TYPE_GUNCON				1
#define PAD_TYPE_KEYBOARD			2
#define PAD_TYPE_MOUSE				3


#define THIS_PAD (pad_active-1)


int pad_active;

int port_type[2] = { PAD_TYPE_NONE, PAD_TYPE_NONE };

int cmdline_port_type[2] = { -1, -1 };

// ==================================
// ==================================
// ==================================

#define NUM_MOUSE_BUTTONS 5


int gpu_screen_x, gpu_screen_y;


// 0-19 = mouse, 20-39 = joystick
static int device_buttons[20*2][ NUM_MOUSE_BUTTONS ];

static int device_x[20*2];
static int device_y[20*2];
static int device_z[20*2];



static int device_hid[2] = { 0,0 };
static int device_absolute[20*2] = { 0,0 };

// ==================================
// ==================================
// ==================================

bool MouseOver(HWND hWnd_app);

int mouse_sensitivity[2] = { 100, 100 };
int mouse_threshold[2] = { 512, 512 };
int mouse_deadzone[2] = { 0,0 };

char mouse_name[512][2] = { 0,0 };


int mouse_x[2], mouse_y[2], mouse_z[2];
int mouse_buttons[2][ NUM_MOUSE_BUTTONS ];

// ==================================
// ==================================
// ==================================

#define GUNCON_RELOAD 0
#define GUNCON_TRIGGER 1
#define GUNCON_A 2
#define GUNCON_B 3
#define GUNCON_C 4
#define GUNCON_START 5
#define GUNCON_SELECT 6
#define GUNCON_DPAD_UP 7
#define GUNCON_DPAD_DOWN 8
#define GUNCON_DPAD_LEFT 9
#define GUNCON_DPAD_RIGHT 10

#define GUNCON_DPAD_A_SELECT 11
#define GUNCON_DPAD_B_SELECT 12
#define GUNCON_DPAD_UP_SELECT 13
#define GUNCON_DPAD_DOWN_SELECT 14
#define GUNCON_DPAD_LEFT_SELECT 15
#define GUNCON_DPAD_RIGHT_SELECT 16
#define GUNCON_NONE 17

#define GUNCON_RELOAD_MANUAL 0
#define GUNCON_RELOAD_SEMI 1

#define GUNCON_ALIGNMENT_DEFAULT 0

#define GUNCON_SHOW_CURSOR 0
#define GUNCON_HIDE_CURSOR 1

#define GUNCON_MODEL_NAMCO 0




/*
take __average__ of data for y-center

- Point Blank + Time Crisis vs
  Resident Evil Survivor + Extreme Ghostbusters
(tight bullseye variation)

- Extreme Ghostbusters vs RES suggests x-center
(tight bullseye variation)
*/

int guncon_screen_center_x[2] = { 274, 274 };
int guncon_screen_center_y[2] = { 168, 168 };



int GUNCON_WIDTH;
int GUNCON_HEIGHT;

int GUNCON_LEFT;
int GUNCON_TOP;
int GUNCON_RIGHT;
int GUNCON_BOTTOM;




float guncon_sensitivity[2] = { 100, 100 };
int guncon_threshold[2] = { 512, 512 };
int guncon_deadzone[2] = { 0,0 };

int guncon_left[2] = { GUNCON_TRIGGER, GUNCON_TRIGGER };
int guncon_right[2] = { GUNCON_A, GUNCON_A };
int guncon_middle[2] = { GUNCON_B, GUNCON_B };

int guncon_aux1[2] = { GUNCON_NONE, GUNCON_NONE };
int guncon_aux2[2] = { GUNCON_NONE, GUNCON_NONE };
int guncon_wheelup[2] = { GUNCON_NONE, GUNCON_NONE };
int guncon_wheeldown[2] = { GUNCON_NONE, GUNCON_NONE };

int guncon_keyboard_dpad[2] = { 0,0 };
int guncon_start_hotkey[2] = { 0,0 };

int guncon_analog_x[2] = { 640 / 2, 640 / 2 };
int guncon_analog_y[2] = { 480 / 2, 480 / 2 };
int guncon_buttons[2][ NUM_MOUSE_BUTTONS ];

int guncon_reload[2] = { GUNCON_RELOAD_MANUAL, GUNCON_RELOAD_MANUAL };
int guncon_calibrate[2];

int guncon_alignment[2] = { GUNCON_ALIGNMENT_DEFAULT, GUNCON_ALIGNMENT_DEFAULT };
int guncon_alignment_auto[2] = { 0,0 };

int guncon_cursor[2] = { GUNCON_SHOW_CURSOR, GUNCON_SHOW_CURSOR };
char guncon_guid_name[2][512] = { 0,0 };

int guncon_lightgun_left[2] = { 1,1 };
int guncon_lightgun_top[2] = { 1,1 };
int guncon_lightgun_right[2] = { 65534, 65534 };
int guncon_lightgun_bottom[2] = { 65534, 65534 };

int guncon_model[2] = { GUNCON_MODEL_NAMCO, GUNCON_MODEL_NAMCO };

int guncon_aiming_model[2] = { GUNCON_MODEL_NAMCO, GUNCON_MODEL_NAMCO };
float guncon_aiming_x[2] = { 100.0, 100.0 };
float guncon_aiming_y[2] = { 100.0, 100.0 };
char guncon_aiming_id[2][512] = { 0,0 };

float guncon_scale_x[2];
float guncon_scale_y[2];

int guncon_width[2] = { 0,0 };
int guncon_height[2] = { 0,0 };



char cmdline_guncon_id[2][512] = { 0,0 };
int cmdline_guncon_model[2] = { -1, -1 };




int guncon_num_profiles = 0;

char guncon_profile_id[250][512];
char guncon_profile_name[250][512];

float guncon_profile_aiming_x[250];
float guncon_profile_aiming_y[250];
int guncon_profile_center_x[250];
int guncon_profile_center_y[250];

int guncon_profile_model[250];
int guncon_profile_width[250];
int guncon_profile_height[250];

int guncon_profile_swap[250];

// ==================================
// ==================================
// ==================================

u8 keyboard_state[2][256];

