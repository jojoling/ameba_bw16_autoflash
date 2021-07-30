# Auto program feature in Arduino Ameba platform.

Here is pathed for auto program flash without push BUN and reset buttons.

# Install ameba
You can reference below link to get started: http://www.amebaiot.com/ameba-arduino-getting-started/

# Pathed your env.

* Windows env.

Download and replace upload_image_tool_windows.exe to your installed folder.
Example: C:\Users\xxxxxx\AppData\Local\Arduino15\packages\realtek\tools\ameba_d_tools\1.0.5

* Linux env. 


* Apple env. 


# Hardware modication:
It needs to add special compoments likes ESP8266/ESP32 auto program circuit.
![](https://i.stack.imgur.com/fMrDh.png?raw=true)

# Other infomations:
Chip CP2012 only support 1M baudrate. It can't work on RTL8720x uart download flash mode speed 1500000.

#
Author:Joe Hsieh
Email:jojoling@gmail.com
