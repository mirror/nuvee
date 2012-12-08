/**********************************************************************************************
raw_mouse_test.c: A demonstration of how to include raw mouse using raw_mouse.h in your applications.

------------------ CHANGES ---------------------
2004-04-22 - added the ability to detect whether RawMouse is available or not so the application can 
   either use a different multi-mouse system, or exit gracefully (thanks to Mark Healey).
2005-04-24 - Modified the code work with the latest version of MinGW.  The new MinGW incorporates
   rawinput, so my winuser header and library is obsolete.
2006-03-05 - Initialized is_absolute and is_virtual_desktop to work better with newer versions of VStudio.

------------------  COMPILING WITH GCC ------------------------------

This program was compiled using MinGW/GCC

In order to recreate the compile environment:

  1) Install the "Win32 compile utilities" (MinGW/GCC) for Mame from http://www.mame.net/downmain.html
  2) The files that are required are raw_mouse.c, raw_mouse.h, and raw_mouse_test.c
  3) From the command line, run "gcc raw_mouse.c raw_mouse_test.c"

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
	2) The files that are required are raw_mouse.c, raw_mouse.h, and raw_mouse_test.c
	3) Copy all .c and .h files into the new project's root folder
	4) In the left pane, click "File View" and left click "Source Files" and select
	     "add files to folder" from the popup menu, and add your file.
	5) You're all set!  Press f5 to build the program.

**********************************************************************************************/

#define _WIN32_WINNT 0x501
#define WM_INPUT 0x00FF

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "raw_mouse.h"

LRESULT CALLBACK
MainWndProc (HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
        static HWND   hwndButton = 0;

        HDC           hdc;             /* A device context used for drawing */
        PAINTSTRUCT   ps;              /* Also used during window drawing */
        RECT          rc;              /* A rectangle used during drawing */
	    char cBuf[256] = "";
	    int i;

        switch (nMsg)
        {

        case WM_DESTROY:
		  PostQuitMessage (0);
		  return 0;
		  break;

                case WM_PAINT:
		  hdc = BeginPaint(hwnd, &ps);

		  GetClientRect(hwnd, &rc);
		 	
		  for (i = 0; i < raw_mouse_count(); i++) {
		    wsprintf(cBuf, 
			     "Mouse %i X: %ld\nMouse %i Y: %ld\nMouse %i Buttons: %i%i%i\nRelative Mode (lightgun): %i\nVirtual Desktop (Multimonitor): %i", 
			     i, 
			     get_raw_mouse_x_delta(i), 
			     i, 
			     get_raw_mouse_y_delta(i), 
			     i, 
			     is_raw_mouse_button_pressed(i, 0), 
			     is_raw_mouse_button_pressed(i, 1), 
			     is_raw_mouse_button_pressed(i, 2), 
			     is_raw_mouse_absolute(i), 
			     is_raw_mouse_virtual_desktop(i));

		    DrawText(hdc, cBuf, strlen(cBuf), &rc, DT_CENTER);
		    OffsetRect(&rc,0,100); // move the draw position down a bit
		  }
		  EndPaint(hwnd, &ps);
		  return 0;
		  break;

		case WM_INPUT: 
		{
		  add_to_raw_mouse_x_and_y((HRAWINPUT)lParam);
		  
		  InvalidateRect(hwnd,0,TRUE);
		  SendMessage(hwnd,WM_PAINT,0,0);
						
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

	// checks for Windows XP.  If no XP is available, gracefully exit.
	//  A real program might opt to use DirectX or regular mouse input
	//  for versions of Windows that fail this call.
	if (!init_raw_mouse(1, 0, 1)) { // registers for (sysmouse=yes,  terminal services mouse=no, HID_mice=yes)
	  MessageBox(NULL, "RawInput not supported by Operating System.  Exiting." , "Error!" ,MB_OK);
	  return 0; 
	}
        while (GetMessage (&msg, NULL, 0, 0))
        {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
        }

	/* deallocate rawmouse stuff */
	destroy_raw_mouse();

        return msg.wParam;
}
