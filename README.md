# Auto program feature in Arduino Ameba platform.

## How to add realtek ameba core files:
You can reference below link to get started: http://www.amebaiot.com/ameba-arduino-getting-started/

Once you have installed. You need to do as below action:

# Windows env.
1.Download and replace upload_image_tool_windows.exe to your installed path.
ex: C:\Users\xxxxxx\AppData\Local\Arduino15\packages\realtek\tools\ameba_d_tools\1.0.5

# Linux env.
not yet.
# Apple env.
not yet.

# hardware modication:
It needs to add special compoments likes ESP8266/ESP32 auto program circuit.

![](https://i.stack.imgur.com/fMrDh.png?raw=true)

![](https://github.com/crystal-ameba/vscode-crystal-ameba/blob/master/assets/demo.png?raw=true)

#Other inofrmations:
Chip CP2012 only support 1M baudrate. It can't work on RTL8720x uart download flash mode speed 1500000.

# Author
Joe Hsieh
Email:jojoling@gmail.com
