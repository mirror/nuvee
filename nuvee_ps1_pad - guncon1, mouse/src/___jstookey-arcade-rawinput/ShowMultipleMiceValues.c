/**********************************************************************************************
ShowMultipleMiceValues.c: A simple demonstration of how to use Rawinput to access multiple mice 
         in Windows XP.

------------------  COMPILING WITH GCC ------------------------------

This program was compiled using MinGW/GCC

In order to recreate the compile environment:

  1) Install the "Win32 compile utilities" (MinGW/GCC) for Mame from http://www.mame.net/downmain.html
  2) From the command line, run "gcc ShowMultipleMiceValues.c"

-------------------  COMPILING WITH MS VISUAL C++ --------------------

This example can also be compiled with Microsoft Visual C++ version 6, but first you need to download
        and install the Microsoft Platform SDK.  In order to access the Microsoft Platform SDK from VStudio, 
	I had to go to tools->options->directories and add the SDK's bin, include, lib, and src folders.
	In the same window, I had drag each of the SDK's folders to the top of the respective list
	to give those folders priority over the others.

	To create your project for Visual Studio (after you prepare VStudio to work with the MS
	     Platform SDK), do the following:

	1) Open Visual C++ and start a new Win32 Application as a "blank project"
	   (This is necessary because if you simply open the .c file in VStudio
	   VStudio will assume it's a "console application" and it won't compile properly.)
	2) Copy the .c file into the new project's root folder
	3) In the left pane, click "File View" and left click "Source Files" and select
	     "add files to folder" from the popup menu, and add your file.
	4) You're all set!  Press f5 to build the program.

**********************************************************************************************/

#define _WIN32_WINNT 0x0501   // Identify this as a Windows XP application.  This is necessary
                              //    to pull in RawInput (which is an XP exclusive)
#define WM_INPUT 0x00FF
char mousemessage[256]; // this is the string we draw to the screen for the first mouse 
char mousemessage2[256]; // this is the string we draw to the screen for the second mouse 
char rawinputdevices[256]; // string with number of raw input devices
							  //	so we can access rawinput
#include <windows.h>
#include <stdlib.h>
#include <string.h>

void InitRawInput() {
	RAWINPUTDEVICE Rid[50]; // allocate storage for 50 device (not going to need this many :) )

	UINT nDevices;
	PRAWINPUTDEVICELIST pRawInputDeviceList;
	if (GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0) { 
		return ;
	}
	pRawInputDeviceList = (RAWINPUTDEVICELIST *)malloc(sizeof(RAWINPUTDEVICELIST) * nDevices);
	GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST));
	// do the job...
	wsprintf(rawinputdevices,"Number of raw input devices: %i\n\n", nDevices);


	// after the job, free the RAWINPUTDEVICELIST
	free(pRawInputDeviceList);


	Rid[0].usUsagePage = 0x01; 
	Rid[0].usUsage = 0x02; 
	Rid[0].dwFlags =0;// RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
	Rid[0].hwndTarget = NULL;

	if (RegisterRawInputDevices(Rid, 1, sizeof (Rid [0])) == FALSE) {
		wsprintf(mousemessage,"RawInput init failed:\n");
		//registration failed. 
	}
	return ;
}

LRESULT CALLBACK
MainWndProc (HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
        static HWND   hwndButton = 0;
        static int    cx, cy;          /* Height and width of our button. */

        HDC           hdc;             /* A device context used for drawing */
        PAINTSTRUCT   ps;              /* Also used during window drawing */
        RECT          rc;              /* A rectangle used during drawing */
	LPBYTE lpb;// = new BYTE[dwSize];//LPBYTE lpb = new BYTE[dwSize];
	UINT dwSize;
	RAWINPUT *raw;
	long tmpx, tmpy;
	
	static long maxx = 0;
        switch (nMsg)
        {

                case WM_DESTROY:
                        PostQuitMessage (0);
                        return 0;
                        break;

                case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);

			GetClientRect(hwnd, &rc);
			DrawText(hdc, mousemessage, strlen(mousemessage), &rc, DT_CENTER);
			OffsetRect(&rc,0,200); // move the draw position down a bit
			DrawText(hdc, rawinputdevices, strlen(rawinputdevices), &rc, DT_CENTER);
			EndPaint(hwnd, &ps);
                        return 0;
                        break;
		case WM_INPUT: 
		{
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, 
							sizeof(RAWINPUTHEADER));

			lpb = malloc(sizeof(LPBYTE) * dwSize);
			if (lpb == NULL) 
			{
				return 0;
			} 

			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, 
				 sizeof(RAWINPUTHEADER)) != dwSize )
				 OutputDebugString (TEXT("GetRawInputData doesn't return correct size !\n")); 

			raw = (RAWINPUT*)lpb;

			if (raw->header.dwType == RIM_TYPEMOUSE) 
			{
			  tmpx = raw->data.mouse.lLastX;
			  tmpy = raw->data.mouse.lLastY;
			  //if (tmpx > maxx) 
			    maxx = tmpx;
			  /* Convert to Directx format */
			  //if ((tmpx > 0xff00))
			    //{ 
			      //tmpx = -(0xffff - tmpx);
			    //wsprintf(mousemessage, "TMPX WAS HUGE!!!  %ld", tmpx);
			    //  }
			  //if (tmpy > 0xff00) tmpy = -(0xffff - tmpy);

				wsprintf(mousemessage,"Mouse:hDevice %d \n usFlags=%04x \nulButtons=%04x \nusButtonFlags=%04x \nusButtonData=%04x \nulRawButtons=%04x \nlLastX=%ld \nlLastY=%ld \nulExtraInformation=%04x\r, %ld\n",					
					raw->header.hDevice,
					raw->data.mouse.usFlags, 
					raw->data.mouse.ulButtons, 
					raw->data.mouse.usButtonFlags, 
					raw->data.mouse.usButtonData, 
					raw->data.mouse.ulRawButtons, 
					tmpx, //raw->data.mouse.lLastX, 
					tmpy, //raw->data.mouse.lLastY, 
					raw->data.mouse.ulExtraInformation),
				        maxx;
			} 
			InvalidateRect(hwnd,0,TRUE);
			SendMessage(hwnd,WM_PAINT,0,0);
						
			free(lpb); 
			return 0;
		} 
        }
        return DefWindowProc (hwnd, nMsg, wParam, lParam);
}

int APIENTRY 
WinMain (HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow)
{
        HWND         hwndMain;        /* Handle for the main window. */
        MSG          msg;             /* A Win32 message structure. */
        WNDCLASSEX   wndclass;        /* A window class structure. */
        char*        szMainWndClass = "WinTestWin";
                                      /* The name of the main window class */

        memset (&wndclass, 0, sizeof(WNDCLASSEX));
        wndclass.lpszClassName = szMainWndClass;
        wndclass.cbSize = sizeof(WNDCLASSEX);
        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc = MainWndProc;
        wndclass.hInstance = hInst;
        wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
        wndclass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
        wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
        RegisterClassEx (&wndclass);
        hwndMain = CreateWindow (
                szMainWndClass,             /* Class name */
                "Hello",                    /* Caption */
                WS_OVERLAPPEDWINDOW,        /* Style */
                CW_USEDEFAULT,              /* Initial x (use default) */
                CW_USEDEFAULT,              /* Initial y (use default) */
                CW_USEDEFAULT,              /* Initial x size (use default) */
                CW_USEDEFAULT,              /* Initial y size (use default) */
                NULL,                       /* No parent window */
                NULL,                       /* No menu */
                hInst,                      /* This program instance */
                NULL                        /* Creation parameters */
                );
        
        ShowWindow (hwndMain, nShow);
        UpdateWindow (hwndMain);

		InitRawInput();

        while (GetMessage (&msg, NULL, 0, 0))
        {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
        }
        return msg.wParam;
}
