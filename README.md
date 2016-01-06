# UTF-8 Graphic Font Library for Microcontroller.
<img src="http://128.199.176.236/dw_utf8_gfont_library/images/banner.jpg"></img>

## Description
This library is made for render UTF-8 text. Rendered font can be exported from **.ttf** file using **dwExporter** in **generator_app**. 

This library is not platform dependent, it can be used on every platform.

Now we implement in case of Arduino on ESP8266 for example.

## How to install example in Arduino on ESP8266
1. Copy folder **dw_utf8_gfont_library** in **src/arduino/libraries** to **Documents/Arduino/libraries**
2. Install dependency library **Adafruit_GFX** and library for ILI9341 display **(Adafruit_ILI9341)** and OLED display **(Adafruit_SSD1306)**.
3. Open up **Arduino**.
4. Open example **File -> Examples -> dw_utf8_gfont_library -> TFT_ILI9341** for ILI9341 display, **File -> Examples -> dw_utf8_gfont_library -> OLED** for OLED display.

## Limitation
- Exporter cannot rendering small font (ex. less than 24 on THSarabun).
- Some Thai font has issue with SARA and WANAYUK.
