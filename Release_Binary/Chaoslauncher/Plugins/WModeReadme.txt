
      _   _   _         _________   _______   ____      _______
     | | | | | |       |  _   _  | |  ___  | |  _  \   |  _____|
     | | | | | |  ___  | | | | | | | |   | | | |  \ \  | |_____
     | | | | | | |___| | | | | | | | |   | | | |   | | |  _____|
     | |_| |_| |       | | | | | | | |___| | | |___| | | |_____
     |_________|       |_| |_| |_| |_______| |_______| |_______| v1.02

     for StarCraft & BroodWar                       © XeN (2008)


W-MODE is a windowed mode plugin for all versions of StarCraft and BroodWar.
It was tested on many of the previous versions up to 1.15.1 but but it was
designed to be compatible with ANY StarCraft version.

The plugin started as a simple utility to help me in StarCraft bugging and
debugging. Some plus work on cursor handling made it a cool plugin that
turns StarCraft into your favorite desktop game. :)
You can get it in 3 forms:
- Pure injectable DLL for developers (wmode.dll)
- BWL4 plugin (wmode.bwl)
- XMOD plugin (wmode.xmod)

The original BWL4 (4.0.3.3 and below) has a bug: it does not accept the (-1)
StarCraft build number, so it does not load this plugin. :( You can use it
with BWL4 clones. I have a home grown one our you can download the free and
opensource ChaosLauncher that works fine with this plug.
You can inject the DLL version before the main thread of StarCraft has reached
the entrypoint of the main module (starcraft.exe), but then you must put the
following line the your wmode.ini file:
HijackInstall=1
If you want to inject it in another time then you can do that any time before
sc uses any of the functions that are hooked by W-MODE (the 2 most important
functions are CreateWindowEx and LoadLibrary/GetProcaddress because of ddraw.dll).

Now some more about the plugin, let's have a go!
You must have a utility that has XMOD plugin loading capability.
You can load W-MODE with scloader2b by typeing the following command:
scloader2b -xmod wmode.xmod
I assume that you copied the plug to your sc directory (and not elswhere)
and you got it work succesfully. Voila! Your sc runs in a window...


About the resolution an color depth of your desktop:
It would not be fun to play sc in windowed mode when your destop resolution
is not bigger than 640x480. I recommend you at least 800x600. You can start
sc + wmode with any resolution / color depth setting but it does not support
palettized modes, that means that you should have at least 16bit/pixel color
depth. On 2, 16, and 256 color modes sc will not look sexy. In 256 color
modes a game can control all 256 palette indices if it uses DDraw in
(exclusive) fullscreen mode, but when windows come to the scene your
operating system preserves 20 palette indices for itself, and your 256 color
sc screen should adapt to it. It could be solved with octree resolution or
median cut or with something similar, but most of the home comps run using
32bit/pixel mode today so I don't want to tire myself with palettes.

About cursor handling:
In order to scroll your StarCraft map you must move your cursor to the
edge of the StarCraft screen. It would be really hard to aim the 2 pixel with
edge of the sc screen with a cursor that can be pulled out from the StarCraft
window, so the cursor should not be allowed to move out of the client area of
your sc window.

There is a function built into W-MODE that handles the cursor, it is called
"cursor clipping". The hotkeys provided to control this:

 * ALT+F1   Turns on/off cursor clip.
 * ALT      Temporarily inverts the current state of cursor clip while it
            is held down.

So, if you switch on clipping you can not move out the cursor from the sc
screen and you can scroll your map with the cursor. If you hold down the
ALT key your clip is turned off temporarily and you can pull out the cursor
from the sc screen. When the cursor is outside the sc screen you can
release the ALT key so clip is on again but it will be actually activeted
only when you move the cursor back to your sc screen. The cursor clip
becomes activated/deactivated automatically in some circumstances. It is
automatically deactivated for example when you switch to another window
with ALT+TAB or your cursor moves over another window that partially covers
the sc screen (for example a winamp with always-on-top mode or a hint window
of the system tray that says "You have not enough free space on drive E:" :p).
Clip is automatically activated when you hold down the left mouse button to
use rectangle selection of more units or to scroll the map using the minimap.

When cursor clip is off you can use the ALT key to temporarily switch on
the clip so as to make it possible to scroll your map in the game. That's
all about cursor clipping of W-MODE, let's try it!

Other hotkeys:

 * ALT+F9      Toggle doublesize mode.

 * ALT+F10     Enables/disables moving the StarCraft window. If you turn this
               functionality off you can not move the StarCraft window to
               elsewhere on the screen by grabbing it and holding down the
               left mouse button. It can be useful when you use other
               windows (msn...) and you click here and there like a zealot
               and don't want to move the sc window accidently.

 * ALT+F11     Turn on/off always-on-top mode. This option is like that of
               Winamp's. When turned on, your sc window will be always on
               top of other windows that are not in always-on-top mode even
               if you interact with a window other than StarCraft's.

 * ALT+F12     Enables/disables all buttons on the caption of the sc window.
               This is another security option like switching off window
               movement for hardcore gamers. The buttons become grey, but
               you can still switch them with hotkeys. This option does
               a bit more when turned on than greying buttons of the
               caption. Switching on this functionality involves that you
               can not use ALT+F4 to close the window and the screensaver
               is disabled when the StarCraft window is in foreground.

You can switch some of these functionalities not only with hotkey but with
mouse by clicking the cool icons on the window if they are not grayed.


To set the options you can find a sample wmode.ini file with comments
about the possible ini keys. W-MODE will search wmode.ini in the StarCraft
directory. Thats all about the plug, using it is very simple.


Version history:
----------------

v1.02: January 4, 2008
 - Added doublesize mode. The BNET interface hasn't been resized. W-MODE automatically
   changes back to original size for the time you are in the NET interface.

v1.02a: October 10, 2007 (this version was not released)
 - StarCraft didn't update some parts of the screen when the window was minimized.
   The update of these parts started only after restoring the window and doing
   something that restarts the update (for example moving the cursor above this
   part of the window). This bug affected for example the ingame build progress
   indicator, the unit portrait, and the lobby chat panel that lists the posted
   messages.
 - After joining a game on BNET you had to select another window than StarCraft's
   and than switch back to be able to type in messages. This happened only when
   you joined the game using mouse clicks. Joining with keyboard did not cause this
   problem (at least on XP), thats why this small uncomfortable bug escaped my notice.

v1.01: September 24, 2007
 - A transparent green square appeared on the cursor when it was over the caption
   or the border of the StarCraft window. Old video drivers caused this bug. Fixed.
 - More usable BNET interface because of some cosmetic changes.
   For example the refresh speed of the chat window is quite OK.
 - Now the game sound isn't muted when the StarCraft window is not focused.
   You won't swallow more nukes for this reason and watching reps is more
   enjoyable even if you are clicking on other windows (msn, winamp...). :)
 - Many people complained about the StarCraft window that it can not be moved
   to other monitor than the primary on multi-monitor systems.
   This should not be a problem with v1.01. If the window caption can not be
   reached on one of the monitors to move the window then the game screen will be
   centered on the desktop. If you regularly unplug your secondary monitor
   then it is recommended to set the SaveWindowClientX and SaveWindowClientY keys
   to zero in the wmode.ini file after positioning the window on the secondary monitor
   and exiting the game. This way the window will always appear on the secondary
   screen except when it is unplugged.
   Thanks to InterClaw for his suggestions and beta testing related to better multiple
   monitor support.
 - From now W-MODE has a BWL4 compatible and an injectable plugin version too for the
   sake of BWL fans and debunkers. Unfortunately the plugin does not work with
   BWL4 v4.0.3.3 and older because BWL4 does not accept the -1 (all version) build
   number due to a bug. Maybe BWL4 clones...

v1.00: June 2, 2007
 - Click on the StarCraft screen with middle or right mouse button, hold
   it down and move out the cursor from the sc screen and then release
   the button. After it you can not click on the StarCraft screen with
   other mouse buttons until you make a click on the sc screen with the
   button that caused the error. Bug fixed.

v1.00 beta: May 9, 2007
   Initial release.
   Notes:
   - Getting work the BNET interface while keeping W-MODE StarCraft version
     independent was not too easy. It can still contain bugs. One known
     "bug" is the slow display of listboxes. This can be annoying since
     the chat window was implemented using a listbox and you will see
     flickering when you send/receive messages.
   - W-MODE has nothing to do with game music/sound (yet). When you switch
     from your StarCraft window to another all StarCraft sound mutes. It can
     be worked around, maybe in the future...
   - Your StarCraft game isn't paused automatically in single player mode
     when you switch to other window than StarCraft's. This is actually
     not a bug, I wanted sc to do so :D Just bring up the menus with F10
     in single player mode to pause the game.


Times are hard and I'am rarely online but you can try to reach me at:

- www.bwprogrammers.com
- xenomails|at|freemail|dot|hu

Regards,
XeNotRoN
