A win32 ps2 controller plugin that provides basic lightweight emulation
- Dual Guncon 2
- Dual Mouse
- Single keyboard



Devices:
- Dual mice (USB + Serial)
- Dual PC lightgun
- Single keyboard
- Dual joysticks (Real + PPJoy, WiiMote)



For use with win32 emulators like
- PCSX2 (special 0.9.8 usb-emu only)
- PCSX2 (SVN)



News:
- 10-21-11  Other misc stuff
- 10-21-11  Upgrade RES2 to working (latest PCSX2 SVN)
- 10-21-11  Create Resident Evil Survivor 2 Guncon2 profile
- 10-21-11  Fix more stuff (USB mouse poll rate, USB keyboard poll, misc emulation)
- 10-21-11  Fix lots of stuff (alt-tab focus, snap cursor, PC lightgun + mouse, other regressions)
- 10-21-11  Support joystick devices (PPJoy or else)
- 10-13-11  Add DirectInput (WiiMote)
- 10-13-11  (PS2) Improve game profile list
- 10-13-11  (PS2) Improve device reset speed (Gunfighter 2 timeout)
- 10-13-11  (PS2) Fix device connection order (not both at same time)
- 10-13-11  (PS2) Improve lightgun swap feature
- 08-21-11  Add mouse, keyboard usb emulation
- 08-19-11  Add C button to keyboard map
- 08-19-11  Fix lightgun swapping
- 08-19-11  Add profile: Endgame
- 08-19-11  Add hotplugging usb devices, hotswapping devices
- 08-18-11  Add profiles: Starsky & Hutch, Gunfighter 2
- 08-18-11  Add profiles: Dead Aim, Dino Stalker, Gunvari Collection
- 08-18-11  Add keyboard d-pad, extra guncon2 buttons, latch guncon2 near VBlank times (fixes trigger)
- 08-17-11  Added PS2 game profiles
- 08-17-11  Added PS2 Guncon 2 support (PCSX2)
- 08-15-11  Added USB OHCI support



INSTRUCTIONS:
0- Install official PCSX2 0.9.8 first
1- Copy dll to your emu's 'plugins' folder
2- Set plugin to nuvee
3- Set nuvee device and configure
4- Hit acquire. choose game profile
5- Play game




NOTES:

(1)
You can exit the emu, change the profiles settings and continue your game
(tweak aiming values on-the-fly)


(2)
Lightguns work best at 640x480 but higher resolutions like 1366x768 work well also


(3)
Real lightguns will work much better than using a mouse
(calibration, Time Crisis Zone submachine gun auto-fire)



(4)
Keyboard D-pad mapping:
W = up
A = Left
S = Down
D = Right

Q = Start
E = Select
F = Button C


Note that using Lilypad or SSSPSX pads can possibly interfere if WASD is
mapped to a joypad.


You can use nuvee_ps2_pad and set controllers to 'None'. Except in the
case of Starsky and Hutch (pad1 = driver, usb1 = shooter)

Or use Lilypad to set controller to 'Unplugged'



(5)
Take note that Lilypad can cause conflicts with USB plugin
- Keyboard = Windows Messaging / DirectInput (best)
- Mouse = None (best)
