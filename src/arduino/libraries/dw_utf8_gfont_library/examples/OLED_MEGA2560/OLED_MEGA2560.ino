#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "dw_font.h"

Adafruit_SSD1306 display;

extern dw_font_info_t font_th_sarabunpsk_regular40;
dw_font_t myfont;

void draw_pixel(int16_t x, int16_t y)
{
  display.drawPixel(x, y, WHITE);
}

void clear_pixel(int16_t x, int16_t y)
{
  display.drawPixel(x, y, BLACK);
}

void setup() {
  // put your setup code here, to run once:
  uint16_t width = 0;
 
  Serial.begin(115200);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.display();

  dw_font_init(&myfont,
               128,
               64,
               draw_pixel,
               clear_pixel);

  dw_font_setfont(&myfont, &font_th_sarabunpsk_regular40);
  dw_font_goto(&myfont, 10, 60);
  dw_font_print(&myfont, "สวัสดีครับ");
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:

}
