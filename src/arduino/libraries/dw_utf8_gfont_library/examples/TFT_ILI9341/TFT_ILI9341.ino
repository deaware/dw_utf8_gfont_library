#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include "dw_font.h"

#define TFT_DC 4
#define TFT_CS 5

extern dw_font_info_t font_th_sarabunpsk_regular36;
extern dw_font_info_t font_th_sarabunpsk_regular48;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
dw_font_t myfont;

uint16_t color;

void drawPixel(int16_t x, int16_t y, uint8_t value)
{
  if (value > 0) {
    tft.drawPixel(x,y,color);
  }
}

void clrPixel(int16_t x, int16_t y)
{
  tft.drawPixel(x,y,ILI9341_BLACK);
}

void setup() {
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);


  dw_font_init(&myfont,
               240,
               320,
               drawPixel,
               clrPixel);

  color = ILI9341_WHITE;
  dw_font_setfont(&myfont, &font_th_sarabunpsk_regular48);
  dw_font_goto(&myfont, 10, 50);
  dw_font_print(&myfont, "สวัสดีภาษาไทย");

  color = ILI9341_GREEN;

  dw_font_setfont(&myfont, &font_th_sarabunpsk_regular36);
  dw_font_goto(&myfont, 10, 100);
  dw_font_print(&myfont, "ฟอนต์อะไรก็ได้");

  color = ILI9341_BLUE;
  dw_font_goto(&myfont, 100, 150);
  dw_font_print(&myfont, "UTF-8");

  color = ILI9341_WHITE;
  dw_font_goto(&myfont, 10, 200);
  dw_font_print(&myfont, "โอเพ่นซอร์สโดย");

  color = ILI9341_RED;
  dw_font_setfont(&myfont, &font_th_sarabunpsk_regular48);
  dw_font_goto(&myfont, 100, 260);
  dw_font_print(&myfont, "Deaware");
}

void loop() {
  // put your main code here, to run repeatedly:

}