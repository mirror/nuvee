//=================================================================
//
//     raw_mouse.h - Windows XP implementation of multi-mouse input 
//
//=================================================================

#ifndef __RAW_MOUSE_H
#define __RAW_MOUSE_H

//============================================================
//	PROTOTYPES
//============================================================
// init raw mouse by creating the array of raw mice (include sysmouse, include rawmouse, include individual mice)

// Number of mice stored in pRawMice array
//   NOTE: init_raw_mouse must be called before this can return the actual number of mice
int raw_mouse_count();

BOOL init_raw_mouse(BOOL, BOOL, BOOL);

// Free up the memory allocated for the raw mouse array
void destroy_raw_mouse(void);

// Every time the WM_INPUT message is received, the lparam must be passed to this function to keep a running tally of
//     every mouse move to maintain accurate results for get_raw_mouse_x_delta() & get_raw_mouse_y_delta().
BOOL add_to_raw_mouse_x_and_y(HANDLE); // device handle, x val, y val

// Fetch the relative position of the mouse since the last time get_raw_mouse_x_delta() or get_raw_mouse_y_delta 
//    was called
ULONG get_raw_mouse_x_delta(int);
ULONG get_raw_mouse_y_delta(int);
ULONG get_raw_mouse_z_delta(int);

// pass the mousenumber, button number, returns 0 if the button is up, 1 if the button is down
BOOL is_raw_mouse_button_pressed(int, int);
char *get_raw_mouse_button_name(int, int);

// Used to determine if the HID is using absolute mode or relative mode
//    The Act Labs PC USB Light Gun is absolute mode (returns screen coordinates)
//    and mice are relative mode (returns delta)
// NOTE: this value isn't updated until the device registers a WM_INPUT message
BOOL is_raw_mouse_absolute(int);

// This indicates if the coordinates are coming from a multi-monitor setup
// NOTE: this value isn't updated until the device registers a WM_INPUT message
BOOL is_raw_mouse_virtual_desktop(int);

#endif /* ifndef __RAW_MOUSE_H */
