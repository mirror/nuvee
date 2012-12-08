/**********************************************************************************************
testmouse.c: A demonstration of how to use Rawinput to access multiple mice in Windows XP.

------------------  COMPILING WITH GCC ------------------------------

This program was compiled using MinGW/GCC

In order to recreate the compile environment:

  1) Install the "Win32 compile utilities" (MinGW/GCC) for Mame from http://www.mame.net/downmain.html
  2) From the command line, run "gcc HelloRawInput.c"

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
							  //	so we can access rawinput
#include <stdio.h>
#include <conio.h>			//  Provides getch()
#include <windows.h>		//  Provides rawinput

main()
{
	UINT nInputDevices;

	// 1st call to GetRawInputDeviceList: Pass NULL to get the size of the list.
	if (GetRawInputDeviceList(NULL, &nInputDevices, sizeof(RAWINPUTDEVICELIST)) != 0) return 0; 
	printf("Number of raw input devices: %i\n\n", nInputDevices);

	printf("Press any key...");
	getch();

	return 1;
}
