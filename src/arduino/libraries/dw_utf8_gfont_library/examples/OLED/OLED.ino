#include <Wire.h>
#include <Adafruit_GFX.h>
#include <ESP_Adafruit_SSD1306.h>
#include "dw_font.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

extern dw_font_info_t font_th_sarabunpsk_regular36;
extern dw_font_info_t font_th_sarabunpsk_regular48;

dw_font_t myfont;

static void drawPixel(int16_t x, int16_t y, uint8_t value){
  if (value > 0) {
    display.drawPixel(x, y, WHITE);
  }
}

static void clrPixel(int16_t x, int16_t y)
{
  display.drawPixel(x, y, BLACK);
}

void setup()   {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c, true);
  display.clearDisplay();

            
  dw_font_init(&myfont,
               128,
               64,
               drawPixel,
               clrPixel);

  dw_font_setfont(&myfont, &font_th_sarabunpsk_regular48);
  
  dw_font_goto(&myfont, 5, 50);
  dw_font_print(&myfont, "สวัสดีไทย");
  display.display();
}


void loop() {
  
}


