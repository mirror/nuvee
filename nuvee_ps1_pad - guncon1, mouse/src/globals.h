#define s8 signed char
#define s16 signed short
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int





const char* PS1_LibraryName = "nuvee psx pad";
const char* PS2_LibraryName = "nuvee ps2 pad";

const unsigned char version = 0x0002;
const unsigned char revision = 0;
const unsigned char build = 2;

const unsigned char PS2_VERSION = ((0<<8) | 2 | (0<<24));



// PCSX2

// PS2EgetLibType returns (may be OR'd)
#define PS2E_LT_PAD  0x02		// -=[ OBSOLETE ]=-

// PS2EgetLibVersion2 (high 16 bits)
#define PS2E_PAD_VERSION  0x0002	// -=[ OBSOLETE ]=-

// plugin types
#define SIO_TYPE_PAD	0x00000001


typedef struct {
	int size;
	char *data;
} freezeData;

typedef struct {
	u32 key;
	u32 evt;
} keyEvent;





// Blade_Arma - pcsxr interface
void (CALLBACK *gpuCursor)(int, int, int) = 0;

void PADhandleHID( LPARAM in_device_handle );
LRESULT WINAPI nuvee_key_winproc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);



HINSTANCE hInstance_app;
HWND hWnd_app;
HWND hWnd_ps1_app;
HWND hWnd_app_key;

WNDPROC pOldWinProc;


char ini_path[1024] = "plugins";
char temp_str[1024];



#define HIDMOUSE_API_RAWINPUT 0
#define HIDMOUSE_API_DIRECTINPUT 1

int hidmouse_api;
int snap_mouse_cursor = 1;



// PCSX1 - two plugins = 1 shared dll
static int init_done;
static int rawinput_active;
static int rawinput_count;
static int mouse_active;

// PCSX2 - one plugin = 2 ports
static int emu_type = 1;
static struct
{
	keyEvent ev;
	u8 state[2][256];
} save;


FILE *fp_debug;
FILE *fp_emu;

// ==================================
// ==================================
// ==================================

#define PAD_TYPE_NONE				0
#define PAD_TYPE_MOUSE			1
#define PAD_TYPE_GUNCON			2
#define PAD_TYPE_JUSTIFIER	3
#define PAD_TYPE_MULTITAP		4


#define THIS_PAD (pad_active-1)


int pad_active;
int pad_cmd;
int pad_count;


int port_type[2] = { PAD_TYPE_NONE, PAD_TYPE_NONE };

int pad_config_mode[2] = { 0,0 };
int pad_controller_mode[2] = { 0,0 };



int cmdline_port_type[2] = { -1, -1 };



u8 pad_out[32+32];

// ==================================
// ==================================
// ==================================

int gpu_screen_x, gpu_screen_y;


// 00-19 = mouse
// 20-39 = joystick
static int device_buttons[20*2][3];

static int device_x[20*2];
static int device_y[20*2];
static int device_z[20*2];



static int device_hid[2] = { 0,0 };
static int device_absolute[20*2] = { 0,0 };

// ==================================
// ==================================
// ==================================

int mouse_sensitivity[2] = { 100, 100 };
int mouse_threshold[2] = { 512, 512 };
int mouse_deadzone[2] = { 0,0 };

char mouse_name[512][2] = { 0,0 };


int mouse_usb_converter[2];

// ==================================
// ==================================
// ==================================

#define GUNCON_RELOAD 0
#define GUNCON_TRIGGER 1
#define GUNCON_A 2
#define GUNCON_B 3

#define GUNCON_RELOAD_MANUAL 0
#define GUNCON_RELOAD_SEMI 1

#define GUNCON_ALIGNMENT_DEFAULT 0

#define GUNCON_SHOW_CURSOR 0
#define GUNCON_HIDE_CURSOR 1

#define GUNCON_MODEL_HOMEBREW 0
#define GUNCON_MODEL_CUSTOM 1
#define GUNCON_MODEL_UNLICENSED 2
#define GUNCON_MODEL_NAMCO 3



/*
Screen modes
0 = 256x240
1 = 320x240
2 = 512x240

unused?
3 = 640x240
4 = 384x240
*/

int guncon_screen_mode = 0;



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

int guncon_analog_x[2] = { 640 / 2, 640 / 2 };
int guncon_analog_y[2] = { 480 / 2, 480 / 2 };

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

int guncon_model[2] = { GUNCON_MODEL_HOMEBREW, GUNCON_MODEL_HOMEBREW };

int guncon_aiming_model[2] = { GUNCON_MODEL_HOMEBREW, GUNCON_MODEL_HOMEBREW };
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

// ==================================
// ==================================
// ==================================

#define JUSTIFIER_RELOAD 0
#define JUSTIFIER_TRIGGER 1
#define JUSTIFIER_AUX 2
#define JUSTIFIER_START 3

#define JUSTIFIER_RELOAD_MANUAL 0
#define JUSTIFIER_RELOAD_SEMI 1

#define JUSTIFIER_SHOW_CURSOR 0
#define JUSTIFIER_HIDE_CURSOR 1

#define JUSTIFIER_ALIGNMENT_DEFAULT 0

#define JUSTIFIER_MODEL_HOMEBREW 0
#define JUSTIFIER_MODEL_CUSTOM 1
#define JUSTIFIER_MODEL_UNLICENSED 2
#define JUSTIFIER_MODEL_KONAMI 3




int justifier_screen_center_x[2] = { 320, 320 };
int justifier_screen_center_y[2] = { 120, 120 };



int JUSTIFIER_WIDTH;
int JUSTIFIER_HEIGHT;

int JUSTIFIER_LEFT;
int JUSTIFIER_TOP;
int JUSTIFIER_RIGHT;
int JUSTIFIER_BOTTOM;




float justifier_sensitivity[2] = { 100, 100 };
int justifier_threshold[2] = { 512, 512 };
int justifier_deadzone[2] = { 0,0 };

int justifier_left[2] = { JUSTIFIER_TRIGGER, JUSTIFIER_TRIGGER };
int justifier_right[2] = { JUSTIFIER_AUX, JUSTIFIER_AUX };
int justifier_middle[2] = { JUSTIFIER_START, JUSTIFIER_START };

int justifier_analog_x[2] = { 640 / 2, 640 / 2 };
int justifier_analog_y[2] = { 480 / 2, 480 / 2 };

int justifier_reload[2] = { JUSTIFIER_RELOAD_MANUAL, JUSTIFIER_RELOAD_MANUAL };
int justifier_calibrate[2];

int justifier_alignment[2] = { JUSTIFIER_ALIGNMENT_DEFAULT, JUSTIFIER_ALIGNMENT_DEFAULT };
int justifier_alignment_auto[2] = { 0,0 };

int justifier_cursor[2] = { JUSTIFIER_SHOW_CURSOR, JUSTIFIER_SHOW_CURSOR };
char justifier_guid_name[2][512] = { 0,0 };

int justifier_lightgun_left[2] = { 1,1 };
int justifier_lightgun_top[2] = { 1,1 };
int justifier_lightgun_right[2] = { 65534, 65534 };
int justifier_lightgun_bottom[2] = { 65534, 65534 };

int justifier_model[2] = { JUSTIFIER_MODEL_HOMEBREW, JUSTIFIER_MODEL_HOMEBREW };

int justifier_aiming_model[2] = { JUSTIFIER_MODEL_HOMEBREW, JUSTIFIER_MODEL_HOMEBREW };
float justifier_aiming_x[2] = { 100.0, 100.0 };
float justifier_aiming_y[2] = { 100.0, 100.0 };
char justifier_aiming_id[2][512] = { 0,0 };

float justifier_scale_x[2];
float justifier_scale_y[2];

int justifier_width[2] = { 0,0 };
int justifier_height[2] = { 0,0 };



char cmdline_justifier_id[2][512] = { 0,0 };
int cmdline_justifier_model[2] = { -1, -1 };




int justifier_num_profiles = 0;

char justifier_profile_id[250][512];
char justifier_profile_name[250][512];

float justifier_profile_aiming_x[250];
float justifier_profile_aiming_y[250];
int justifier_profile_center_x[250];
int justifier_profile_center_y[250];

int justifier_profile_model[250];
int justifier_profile_width[250];
int justifier_profile_height[250];


int justifier_trigger_latch[2];

// ==================================
// ==================================
// ==================================

int multitap1_index;
int multitap2_index;
