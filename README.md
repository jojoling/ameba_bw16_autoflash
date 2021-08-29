# Auto program feature in Arduino Ameba platform.

Here is patched for auto program flash without push BUN and RESET buttons.

v2.0:
Re-write flash code without calling "amebad_image_tool.exe" (windows/linux/macOS).
It can update flash correct without rs232 chip issues.

pass: cp2102/ch340/ft232rl
platform: windows 10/ linux
macOS: It didn't pass due to macOS VM slowly in my side. It should test on read computer again.

more command added:
--verbose=[n] , n can be 3,5 and 7.
display more information on screen.

--baudrate=[speed]
force rs232 baudrate as you want.
windows:1500000,1444400,1382400,1000000,921600,500000,460800,380400,230400,380400,128000 and 115200.
linux:1500000,1000000,921600,500000,460800,230400 and 115200.
macOS:1500000,1444400,1382400,1000000,921600,500000,460800,380400,230400,380400,128000 and 115200.

--auto=[0|1]
enable auto into flash mode or not.

v1.0:
Tool only do DTR/RTS control for auto reset function.

# Install ameba
You can reference below link to get started: http://www.amebaiot.com/ameba-arduino-getting-started/

# Patch your environment

* Windows

Download and replace upload_image_tool_windows.exe to your installed folder.<br/>
ex: C:\Users\xxxxxx\AppData\Local\Arduino15\packages\realtek\tools\ameba_d_tools\1.0.5

* Linux

Download and replace upload_image_tool_linux to your installed folder.<br/>
ex: ~/snap/arduino/61/.arduino15/packages/realtek/tools/ameba_d_tools/1.0.5

* MacOS (not test yet)

Download and replace upload_image_tool_macos to your installed folder.<br/>
ex: ~/Library/Arduino15/packages/realtek/tools/ameba_d_tools/1.0.5

# snapshot:
winidows 10:
![](pic/flash_windows.png)

linux mint:
ft232rl
![](pic/flash_ft232rl.png)

ch340
![](pic/flash_ch340.png)

cp2102
![](pic/flash_cp2102.png)



# Hardware modication:
It needs to add special compoments likes ESP8266/ESP32 auto program circuit.
<br/>nRST => EN , BW16_Pin3
<br/>GPIO0 => LOG_TX ,BW16_Pin11

![](https://i.stack.imgur.com/fMrDh.png?raw=true)

# Other infomations:
V2.0:
CP2102 use baudrate 921600 instead of 1000000 for response packets correctly.

V1.0:
Chip CP2012 only supports speed of 1M baudrate. It can't work on RTL8720x uart download flash mode speed 1500000.
# Known issues
V2.0:
None.

V1.0:
1) First time upload will be failure due to amebad_image_tool.exe(no source) do something and let processor back to normal mode.
   It only happend and just onece in case when usb-serial just plug-in.

#
 Author:Joe Hsieh
 Email:jojoling@gmail.com
