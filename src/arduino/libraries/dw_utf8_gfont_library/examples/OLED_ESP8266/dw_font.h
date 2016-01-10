/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Deaware System (http://www.deaware.com)
 * Author: Surawut Aonjaiaow
 *         Burin Sapsiri
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _DW_FONT_H
#define _DW_FONT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
    #define NULL                    0
#endif

typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t data[];
}dw_font_bitmap_t;

typedef struct {
    uint32_t utf8;
    int16_t offset_x;
    int16_t offset_y;
    uint16_t cur_dist;
    const dw_font_bitmap_t *bitmap;
}dw_font_symbol_t;

typedef struct {
    uint16_t count;
    uint16_t font_size;
    uint16_t height;
    dw_font_symbol_t symbols[];
}dw_font_info_t;

typedef void (dw_font_drawpixel_t)(int16_t x, int16_t y);
typedef void (dw_font_clrpixel_t)(int16_t x, int16_t y);
typedef void (dw_font_draw_bimap_t)(int16_t x, int16_t y, dw_font_bitmap_t *bitmap);

typedef struct {
    dw_font_drawpixel_t *draw_pixel_cb;
    dw_font_clrpixel_t *clear_pixel_cb;
    dw_font_info_t *font;

    int16_t current_x;
    int16_t current_y;
    uint16_t x_res;
    uint16_t y_res;
}dw_font_t;

void dw_font_init(dw_font_t *inst,
                  uint16_t x_res,
                  uint16_t y_res,
                  dw_font_drawpixel_t *draw_pixel_cb,
                  dw_font_clrpixel_t *clear_pixel_cb);

void dw_font_setfont(dw_font_t *inst,
                     dw_font_info_t *font);

void dw_font_goto(dw_font_t *inst,
                  int16_t x,
                  int16_t y);

void dw_font_print(dw_font_t *inst,
                   char *str);

#ifdef __cplusplus
}
#endif

#endif /*_DW_FONT_H*/

