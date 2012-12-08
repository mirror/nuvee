A win32 ps1 / ps2 controller plugin that provides basic emulation
- Dual PSX Mouse (PS1)
- Dual PSX Guncon (PS1)

- Dual PSX G-Con 45 (PS2)



Devices:
- Dual mice (USB + Serial)
- Dual PC lightgun




For use with win32 emulators like
- PCSX-reloaded (cmdline = lightgun build only)

- PCSX-reloaded (any)
- PCSX2 (any)




News:
- 10-21-11  Fix more stuff (USB mouse poll rate, USB keyboard poll, misc emulation)
- 10-21-11  Fix lots of stuff (alt-tab focus, snap cursor, PC lightgun + mouse, other regressions)
- 10-21-11  Support joystick devices (PPJoy or else)
- 08-09-11  Changed lightgun ps2 custom profile to make Endgame (U) work
- 08-09-11  Added Vampire Night (E), Time Crisis 3/Zone (E)
- 08-07-11  Added several PS2 profiles
- 08-07-11  Added PS2 G-Con 45 support (PCSX2)
- 08-07-11  Made nuvee emu-independent (RawInput changes)
- 08-07-11  Added Moorhuhn 2/3 profiles, fixed Moorhuhn X profile (delete old profiles.ini)
- 07-24-11  Release 1
- 07-24-11  Rewrote Guncon engine, game presets, default file, lots of fixes (huge thanks killervin, paubi)
- 07-05-11  Lots of fixes to PC lightgun support (thanks killervin, paubi)
- 07-01-11  True dual mouse support, fix lightgun trigger holding bug
- 06-30-11  RawInput API by Jake Stookey, True dual lightgun support (custom pcsx-r emu only), GUI sensitivity
- 06-29-11  Guncon - hide cursor, allow 0.000001 sensitivity (edit via ini)
- 05-28-11  Guncon calibration / offscreen shots
- 05-26-11  Add basic guncon support
- 05-25-11  Add basic mouse 43,45,46,47,4c / fix 2x mouse errors / fix mouse1 unplugged error
- 05-25-11  Mouse support (directinput)




INSTRUCTIONS:
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
Command-line options available

ex.
pcsxr-lightgun -controller_plugin1 PadSSSPSX.dll -controller_plugin2 nuvee_psx_controller.dll -nuvee_port2 guncon -nuvee_guncon2_model custom -nuvee_guncon2_profile ghostbusters




TODO?
- (?) Add justifier support
- (?) Add multitap support
