<meta property="og:title" content="RTL8720 autoflash" />
<meta property="og:image" content="https://github.com/jojoling/ameba_bw16_autoflash/blob/main/pic/flash_windows.png" />

# Auto program feature in Ameba Arduino platform.

Here is a patch file for auto program flash without pushing the BURN and RESET buttons.

## v2.2  
Bugs fixed.

## v2.0  
Re-write the flash code without calling "amebad_image_tool.exe" (windows/linux/macOS).  
It can update flash correctly without rs232 chip issues.  
  
pass: cp2102/ch340/ft232rl  
platform: Windows 10/ Linux  
macOS: It didn't pass due to macOS VM slowly in my side. It should test on another computer again.  

more command added:  
```
--verbose=[n] 
```
n can be 3,5 and 7.  
display more information on the screen.  

```
--baudrate=[speed]  
```
force RS232 Baudrate as you want.  
* windows:
  1500000,1444400,1382400,1000000,921600,500000,460800,380400,230400,380400,128000 and 115200.  
* linux:
  1500000,1000000,921600,500000,460800,230400 and 115200.  
* macOS:
  1500000,1444400,1382400,1000000,921600,500000,460800,380400,230400,380400,128000 and 115200.  

```
--auto=[0|1]  
```
enable auto into the flash mode or not.  

## v1.0
Currently this tool only performs DTR/RTS control for auto reset function.

# Install Ameba
You can reference the link below to get started: http://www.amebaiot.com/ameba-arduino-getting-started/

# Patch your Environment

* Windows
  
  Download and replace `upload_image_tool_windows.exe` to your installed folder.  
  ex: `C:\Users\xxxxxx\AppData\Local\Arduino15\packages\realtek\tools\ameba_d_tools\1.0.5`

* Linux
  
  Download and replace `upload_image_tool_linux` to your installed folder.  
  ex: `~/snap/arduino/61/.arduino15/packages/realtek/tools/ameba_d_tools/1.0.5`

* MacOS (not test yet)
  
  Download and replace `upload_image_tool_macos` to your installed folder.  
  ex: `~/Library/Arduino15/packages/realtek/tools/ameba_d_tools/1.0.5`

# Snapshot
* Windows 10:  
  ![windows 10](pic/flash_windows.png)

* Linux mint:
  cp2102  
  ![](pic/flash_cp2102.png)

* verbose = 5  
  ![](pic/flash_verbose_5.png)

* verbose = 7, with recevied packets for debug purpose.  
  ![](pic/flash_verbose_7.png)

* baudrate = 115200, force to low speed 115200 to program flash.  
  ![](pic/flash_115200.png)

# Hardware Modication
Similarly to ESP8266/ESP32 auto program circuit, additional components is required to be added: 

nRST  => EN , BW16_Pin3  
GPIO0 => LOG_TX ,BW16_Pin11  

![](https://i.stack.imgur.com/fMrDh.png?raw=true)

# Other Infomation

## V2.0
CP2102 use baudrate 921600 instead of 1000000 for response packets correctly.

All information was retrieved from `amebad_image_tool.exe` and `ImageTool.exe` using tools below:  
SerialMon, IO Ninja, Wireshark and Tshark.  

## V1.0
~~Chip CP2012 only supports speed of 1M baudrate. It can't work on RTL8720x UART download flash mode speed 1500000.~~  

# Known Issues

## V2.0
None.

## V1.0
1) ~~First time uploading will be failed due to `amebad_image_tool.exe`(no source) did something and let the processor went back to normal mode.
   It happended just onece in the case when usb-serial just plugged-in.~~

#
 Author:Joe Hsieh  
 Email:jojoling@gmail.com  
