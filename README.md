# UTF-8 Graphic Font Library for Microcontroller.
<table border="0">
    <td width="50%">
        <img src="http://i.imgur.com/WlAtoPe.jpg"></img>
    </td>
    <td width="50%">
        <img src="http://i.imgur.com/tTaVG7u.jpg"></img>
    </td>
</table>

## Description
This library is made for render UTF-8 text. Rendered font can be exported from **.ttf** and **.otf** file using **dwExporter** in **generator_app**. 

This library is not platform dependent, it can be used on every platform.

Now we implement in case of Arduino and ESP8266 for example.

## How to install example in Arduino on ESP8266
1. Copy all folders in **src/arduino/libraries** to **Documents/Arduino/libraries**
3. Open up **Arduino**.
4. Open example **File -> Examples -> dw_utf8_gfont_library -> OLED_ESP8266** for use OLED on ESP8266, **File -> Examples -> dw_utf8_gfont_library -> OLED_MEGA2560** for use OLED on Arduino MEGA 2560.

## Generator
Windows : [Win32_Release_v1.0](https://github.com/deaware/dw_utf8_gfont_library/blob/master/generator_app/dwFontExporter/dwFontExporter_win32_release_v1.0.zip?raw=true)

Mac : Not yet built, but you can compile using Qt direct from source code. Please install freetype library before build.

Linux : Not yet built, but you can compile using Qt direct from source code. Please install freetype library before build.


## Limitation
- Some Thai font has issue with SARA and WANAYUK.

## Next version
- Fix all limitation.
- Add international font example (ex. Japan, China ...).)
