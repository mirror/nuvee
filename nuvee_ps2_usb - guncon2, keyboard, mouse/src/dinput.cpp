LPDIRECTINPUT8 dinput;

LPDIRECTINPUTDEVICE8 di_joystick[30];
LPDIRECTINPUTDEVICE8 di_mouse[20];
LPDIRECTINPUTDEVICE8 di_keyboard;



HWND di_enum_hwnd;

DIMOUSESTATE di_mousestate;
DIJOYSTATE2 di_joystate;
BYTE di_keystate[256];


int di_mouse_count;
char di_mouse_guid[20][512];

int di_joystick_count;
char di_joystick_guid[20][512];



#define DIMOUSE_LEFTBUTTON   0
#define DIMOUSE_RIGHTBUTTON  1
#define DIMOUSE_MIDDLEBUTTON 2
#define DIMOUSE_4BUTTON      3
#define DIMOUSE_5BUTTON      4


// From WINE
void guid_to_string( const GUID* guid, char *str )
{
	sprintf(str, "{%08x-%04x-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		guid->Data1, guid->Data2, guid->Data3,
		guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3],
		guid->Data4[4], guid->Data4[5], guid->Data4[6], guid->Data4[7]);
}



// NOTE: XP only sees 'SysMouse'
BOOL CALLBACK DInput_Mouse_EnumCallback( const DIDEVICEINSTANCE* instance, VOID* context )
{
	if( FAILED( dinput->CreateDevice( instance->guidInstance, &di_mouse[ di_mouse_count ], NULL )) )
		return DIENUM_CONTINUE;

	if( FAILED( di_mouse[ di_mouse_count ]->SetDataFormat( &c_dfDIMouse )) )
		return DIENUM_CONTINUE;

	if( FAILED( di_mouse[ di_mouse_count ]->SetCooperativeLevel( di_enum_hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND )) )
		return DIENUM_CONTINUE;


	// save name
	guid_to_string( &instance->guidInstance, di_mouse_guid[ di_mouse_count ] );


	di_mouse_count++;
}



BOOL CALLBACK DInput_joystick_enumAxesCallback( const DIDEVICEOBJECTINSTANCE* instance, VOID* context )
{
	HWND hDlg = (HWND) context;

	DIPROPRANGE propRange; 
	propRange.diph.dwSize       = sizeof(DIPROPRANGE); 
	propRange.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	propRange.diph.dwHow        = DIPH_BYID; 
	propRange.diph.dwObj        = instance->dwType;

	// Mouse range = 8-bit values
	//propRange.lMin              = -127;
	//propRange.lMax              = +127;
	propRange.lMin              = -64;
	propRange.lMax              = +64;

	if( FAILED( di_joystick[ di_joystick_count ]->SetProperty( DIPROP_RANGE, &propRange.diph )) ) {
		return DIENUM_CONTINUE;
	}


	return DIENUM_CONTINUE;
}



BOOL CALLBACK DInput_Joystick_EnumCallback( const DIDEVICEINSTANCE* instance, VOID* context )
{
	if( FAILED( dinput->CreateDevice( instance->guidInstance, &di_joystick[ di_joystick_count ], NULL )) )
		return DIENUM_CONTINUE;

	if( FAILED( di_joystick[ di_joystick_count ]->SetDataFormat( &c_dfDIJoystick2 )) )
		return DIENUM_CONTINUE;

	if( FAILED( di_joystick[ di_joystick_count ]->SetCooperativeLevel( di_enum_hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND )) )
		return DIENUM_CONTINUE;


	/*
	DIDEVCAPS capabilities;

	// Determine how many axis the joystick has (so we don't error out setting
	// properties for unavailable axis)

	capabilities.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(hr = joystick->GetCapabilities(&capabilities))) {
			return hr;
	}
	*/


	if( FAILED( di_joystick[ di_joystick_count ]->EnumObjects( DInput_joystick_enumAxesCallback, NULL, DIDFT_AXIS )) )
	  return DIENUM_CONTINUE;



	// save name
	guid_to_string( &instance->guidInstance, di_joystick_guid[ di_joystick_count ] );

	di_joystick_count++;
}



void Init_DInput( HINSTANCE hInstance, HWND hWnd )
{
	di_enum_hwnd = hWnd;


	if( FAILED( DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**) &dinput, NULL)) ) {
		return;
	}


	if( hidmouse_api == HIDMOUSE_API_DIRECTINPUT )
	{
		di_mouse_count = 0;

		dinput->EnumDevices( DI8DEVCLASS_POINTER, DInput_Mouse_EnumCallback, NULL, DIEDFL_ATTACHEDONLY );
	}




	di_joystick_count = 0;

	dinput->EnumDevices( DI8DEVCLASS_GAMECTRL, DInput_Joystick_EnumCallback, NULL, DIEDFL_ATTACHEDONLY );




	// XP = system keyboard only (!)
	if( FAILED( dinput->CreateDevice( GUID_SysKeyboard, &di_keyboard, NULL)) )
		return;

	if( FAILED( di_keyboard->SetDataFormat( &c_dfDIKeyboard )) )
		return;

	if( FAILED( di_keyboard->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND )) )
		return;
}



void Poll_Mouse_DInput()
{
	HRESULT hr;

	if( !di_mouse[0] ) return;
	if( hidmouse_api != HIDMOUSE_API_DIRECTINPUT ) return;



	for( int lcv = 0; lcv < di_mouse_count; lcv++ )
	{
		device_x[ lcv ] = 0;
		device_y[ lcv ] = 0;
		device_z[ lcv ] = 0;


		device_buttons[ lcv ][0] = 0;
		device_buttons[ lcv ][1] = 0;
		device_buttons[ lcv ][2] = 0;
		device_buttons[ lcv ][3] = 0;
		device_buttons[ lcv ][4] = 0;

		
		device_absolute[ lcv ] = 0;



		// assume lost input (alt-tab)
		hr = DIERR_INPUTLOST;
		while( hr == DIERR_INPUTLOST )
			hr = di_mouse[ lcv ]->Acquire();
		if( FAILED(hr) ) continue;


		hr = di_mouse[ lcv ]->GetDeviceState( sizeof(DIMOUSESTATE), (LPVOID)&di_mousestate );
		if( FAILED(hr) ) continue;


		device_x[ lcv ] = di_mousestate.lX;
		device_y[ lcv ] = di_mousestate.lY;
		device_z[ lcv ] = di_mousestate.lZ;


		for( int lcv2 = 0; lcv2 < 5; lcv2++ )
		{
			if( di_mousestate.rgbButtons[ lcv2 ] & 0x80 )
				device_buttons[ lcv ][ lcv2 ] = 1;
		}
	}
}



void Poll_Joystick_DInput()
{
	HRESULT hr;


	if( !di_joystick[0] ) return;



	for( int lcv = 0; lcv < di_joystick_count; lcv++ )
	{
		device_x[ lcv+20 ] = 0;
		device_y[ lcv+20 ] = 0;
		device_z[ lcv+20 ] = 0;


		device_buttons[ lcv+20 ][0] = 0;
		device_buttons[ lcv+20 ][1] = 0;
		device_buttons[ lcv+20 ][2] = 0;
		device_buttons[ lcv+20 ][3] = 0;
		device_buttons[ lcv+20 ][4] = 0;


		// assume lost input (alt-tab)
		hr = DIERR_INPUTLOST;
		while( hr == DIERR_INPUTLOST )
			hr = di_joystick[ lcv ]->Acquire();
		if( FAILED(hr) ) continue;



		hr = di_joystick[ lcv ]->Poll();
		if( FAILED(hr) ) continue;
		
		hr = di_joystick[ lcv ]->GetDeviceState( sizeof(DIJOYSTATE2), (LPVOID)&di_joystate );
		if( FAILED(hr) ) continue;


		device_x[ lcv+20 ] = di_joystate.lX;
		device_y[ lcv+20 ] = di_joystate.lY;
		device_z[ lcv+20 ] = di_joystate.lZ;


		for( int lcv2 = 0; lcv2 < 5; lcv2++ ) {
			if( di_joystate.rgbButtons[ lcv2 ] & 0x80 )
				device_buttons[ lcv+20 ][ lcv2 ] = 1;
		}
	}
}



void Poll_Keyboard_DInput()
{
	HRESULT hr;


	if( !di_keyboard ) return;


	// creates poll errors
	//memset( keyboard_state[0], 0, sizeof(keyboard_state[0]) );
	//memset( keyboard_state[1], 0, sizeof(keyboard_state[1]) );


	// assume lost input (alt-tab)
	hr = DIERR_INPUTLOST;
	while( hr == DIERR_INPUTLOST )
		hr = di_keyboard->Acquire();

	if( FAILED(hr) ) return;


	di_keyboard->GetDeviceState( 256, (LPVOID)di_keystate );
}



void Done_DInput()
{
	for( int lcv = 0; lcv < di_mouse_count; lcv++ ) {
		if( di_mouse[ lcv ] ) {
			di_mouse[ lcv ]->Unacquire();
			di_mouse[ lcv ]->Release();
			di_mouse[ lcv ] = NULL;
		}
	}


	for( int lcv = 0; lcv < di_joystick_count; lcv++ ) {
		if( di_joystick[ lcv ] ) {
			di_joystick[ lcv ]->Unacquire();
			di_joystick[ lcv ]->Release();
			di_joystick[ lcv ] = NULL;
		}
	}


	if( di_keyboard ) {
		di_keyboard->Unacquire();
		di_keyboard->Release();
		di_keyboard = NULL;
	}


	if( dinput ) {
		dinput->Release();
		dinput = NULL;
	}


	di_mouse_count = 0;
}
