#include <Wire.h>
#include "dw_font.h"
#include "SSD1306.h"

SSD1306   display(0x3c, 4, 5);

extern dw_font_info_t font_th_sarabunpsk_regular40;
dw_font_t myfont;

void draw_pixel(int16_t x, int16_t y)
{
  display.setColor(WHITE);
  display.setPixel(x, y);
}

void clear_pixel(int16_t x, int16_t y)
{
  display.setColor(BLACK);
  display.setPixel(x, y);
}

void setup() {
  // put your setup code here, to run once:
  uint16_t width = 0;
 
  Serial.begin(115200);
  
  display.init();
  display.flipScreenVertically();

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
