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

#include "dw_font.h"

#define UNDER_SYMBOL_LEN                    3
static const uint32_t under_symbol[] = {0xe0b8b8, 0xe0b8b9, 0xe0b8ba};

#define OVERHEAD_LV1_SYMBOL_LEN             6
static const uint32_t overhead_lv1_symbol[] = {0xe0b8b1, 0xe0b8b4, 0xe0b8b5, 0xe0b8b6, 0xe0b8b7, 0xe0b987};

#define OVERHEAD_LV2_SYMBOL_LEN             7
static const uint32_t overhead_lv2_symbol[] = {0xe0b988, 0xe0b989, 0xe0b98a, 0xe0b98b, 0xe0b98c, 0xe0b98d, 0xe0b98e};

#define PADDING_SYMBOL_LEN              1
static const uint32_t padding_symbol[] = {0xe0b8b3};

#define FALSE               0
#define TRUE                !FALSE

#define PROGMEM_READ_CONST_16BIT(x)          pgm_read_word(x)
#define PROGMEM_READ_CONST_8BIT(x)           pgm_read_byte(x)

//#define AVR_OPTIMIZED

#if defined(__AVR__)
    #include <avr/pgmspace.h>
    #define READ_CONST_16BIT(x)             PROGMEM_READ_CONST_16BIT(x)
    #define READ_CONST_8BIT(x)              PROGMEM_READ_CONST_8BIT(x)
#elif defined(__XTENSA__)
    #include <pgmspace.h>
    #define READ_CONST_16BIT(x)             PROGMEM_READ_CONST_16BIT(x)
    #define READ_CONST_8BIT(x)              PROGMEM_READ_CONST_8BIT(x)
#else
    #define READ_CONST_16BIT(x)             *x
    #define READ_CONST_8BIT(x)              *x
#endif

static void* binary_search (const void* key, const void* base0,
                      int16_t num, int16_t size,
                      int (*compar)(const void*,const void*))
{
    const char *base = (const char*)base0;
    int16_t lim;
    int cmp;
    const void *p;

    for (lim = num; lim != 0; lim >>= 1) {
        p = base + (lim >> 1) * size;
        cmp = (*compar)(key, p);
        if (cmp == 0)
            return ((void *)p);
        if (cmp > 0) {  /* key > p: move right */
            base = (char *)p + size;
            lim--;
        }   /* else move left */
    }
    return (NULL);
}

static int utf8_compare(const void *a, const void *b)
{
    dw_font_symbol_t *sa = (dw_font_symbol_t*)a;
    dw_font_symbol_t *sb = (dw_font_symbol_t*)b;

    if (sa->utf8 > sb->utf8) {
        return 1;
    } else if (sa->utf8 == sb->utf8) {
        return 0;
    } else {
        return -1;
    }
}

static uint8_t is_utf8(const char *str)
{
    if ((*str & 0x80) > 0) {
        return 1;
    } else {
        return 0;
    }
}

static uint8_t get_utf8_format(const char *str, uint32_t *data)
{
    /* Implement getting utf8 from char array here. */
    uint32_t val=0;
    char utf_data = *str;
    uint8_t size = 0;

    if(utf_data & 0x80){
      do {
          val<<=8;
          val |= 0x000000ff & (uint32_t)*str++;
          utf_data<<=1;
          size++;
      }while(utf_data & 0x80);
    }else{
      val = utf_data;
      size = 1;
    }

    *data = val;

    return size;
}

static uint8_t findsymbol(dw_font_t *inst, const char *str, dw_font_symbol_t **output)
{
    uint32_t data = 0;
    uint8_t size = 0;

    if (is_utf8(str) > 0) {
        /* In case of multi byte utf-8. */
        size = get_utf8_format(str, &data);
    } else {
        /* In case of single byte utf-8. */
        data = ((uint32_t)*str & 0x000000ff);
        size = 1;
    }

    *output = binary_search(&data, inst->font->symbols, inst->font->count, sizeof(dw_font_symbol_t), utf8_compare);

    return size;
}

static void internal_draw_bitmap(dw_font_t *inst,
                                 int16_t x,
                                 int16_t y,
                                 dw_font_bitmap_t *bitmap)
{
    int16_t current_x = 0;
    int16_t current_y = 0;

    uint8_t current_bit = 7;
    uint8_t *data_ptr = bitmap->data;

    for (current_y = y; current_y < (y + READ_CONST_16BIT(&bitmap->height)); current_y++) {
        for (current_x = x; current_x < (x + READ_CONST_16BIT(&bitmap->width)); current_x++) {
            if ((READ_CONST_8BIT(data_ptr) & (1 << current_bit)) > 0) {
                inst->draw_pixel_cb(current_x, current_y);
            }

            if (current_bit > 0) {
                current_bit--;
            } else {
                current_bit = 7;
                data_ptr++;
            }
        }
    }
}

static uint8_t is_overhead_lv1_symbol(dw_font_symbol_t *symbol)
{
    uint8_t i = 0;
    for (i=0; i<OVERHEAD_LV1_SYMBOL_LEN; i++) {
        if (overhead_lv1_symbol[i] == symbol->utf8) {
            return TRUE;
        }
    }

    return FALSE;
}

static uint8_t is_overhead_lv2_symbol(dw_font_symbol_t *symbol)
{
    uint8_t i = 0;
    for (i=0; i<OVERHEAD_LV2_SYMBOL_LEN; i++) {
        if (overhead_lv2_symbol[i] == symbol->utf8) {
            return TRUE;
        }
    }

    return FALSE;
}

static uint8_t is_padding_symbol(dw_font_symbol_t *symbol)
{
    uint8_t i = 0;
    for (i=0; i<PADDING_SYMBOL_LEN; i++) {
        if (padding_symbol[i] == symbol->utf8) {
            return TRUE;
        }
    }

    return FALSE;
}

static uint8_t is_under_symbol(dw_font_symbol_t *symbol)
{
    uint8_t i = 0;
    for (i=0; i<UNDER_SYMBOL_LEN; i++) {
        if (under_symbol[i] == symbol->utf8) {
            return TRUE;
        }
    }

    return FALSE;
}

static uint8_t should_padding(dw_font_symbol_t *prev,
                              dw_font_symbol_t *current,
                              dw_font_symbol_t *next)
{
    if ((prev != NULL) && (current != NULL) && (next != NULL)) {
        if (is_overhead_lv2_symbol(current) &&
            (is_overhead_lv1_symbol(prev) || is_padding_symbol(next))) {
            return TRUE;
        } else {
            return FALSE;
        }
    } else if ((prev != NULL) && (current != NULL)) {
        if (is_overhead_lv2_symbol(current) &&
            is_overhead_lv1_symbol(prev)) {
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

void dw_font_init(dw_font_t *inst,
                  uint16_t x_res,
                  uint16_t y_res,
                  dw_font_drawpixel_t *draw_pixel_cb,
                  dw_font_clrpixel_t *clear_pixel_cb)
{
    inst->x_res = x_res;
    inst->y_res = y_res;
    inst->draw_pixel_cb = draw_pixel_cb;
    inst->clear_pixel_cb = clear_pixel_cb;
    inst->current_x = 0;
    inst->current_y = 0;
}

void dw_font_setfont(dw_font_t *inst,
                     dw_font_info_t *font)
{
    inst->font = font;
}

void dw_font_goto(dw_font_t *inst,
                  int16_t x,
                  int16_t y)
{
    inst->current_x = x;
    inst->current_y = y;
}

void dw_font_print(dw_font_t *inst,
                   char *str)
{
    char *current_str = str;
    uint8_t size = 0;
    uint8_t next_size = 0;
    int16_t offset_y = 0;
    int16_t x = 0;
    int16_t y = 0;

    int16_t start_x = 0;
    int16_t start_y = 0;

    dw_font_symbol_t *current_symbol = 0;
    dw_font_symbol_t *prev_symbol = 0;
    dw_font_symbol_t *next_symbol = 0;

    /* Keep start current_x and start current_y. */
    start_x = inst->current_x;
    start_y = inst->current_y;

    /* Clear area to paint to. */
    while (*current_str != '\0') {
        size = findsymbol(inst, current_str, &current_symbol);
        current_str += size;
        if (current_symbol != NULL) {
            for (y=(inst->current_y - inst->font->height); y<(inst->current_y); y++) {
                for (x=(inst->current_x + current_symbol->offset_x); x<(inst->current_x + current_symbol->cur_dist); x++) {
                    inst->clear_pixel_cb(x, y);
                }
            }

            inst->current_x += current_symbol->cur_dist;
        }
    }

    /* Restore current_x, current_y. */
    inst->current_x = start_x;
    inst->current_y = start_y;
    current_str = str;

    /* Draw character. */
    while (*current_str != '\0') {
        if (next_symbol == NULL) {
            size = findsymbol(inst, current_str, &current_symbol);
        } else {
            current_symbol = next_symbol;
            size = next_size;
        }

        current_str += size;
        if (current_symbol != NULL) {
            if ((inst->current_x + READ_CONST_16BIT(&current_symbol->bitmap->width)) >= inst->x_res) {
                /* Implement newline here if needed. */

            }

            next_size = findsymbol(inst, current_str, &next_symbol);

            /* Character over character detection for adjusting to correct position. */
            /* Need to be implemented more. Not good for now. */
            if (is_overhead_lv2_symbol(current_symbol)) {
                if (should_padding(prev_symbol, current_symbol, next_symbol)) {
                    offset_y = current_symbol->offset_y;
                } else {
                    offset_y = current_symbol->offset_y + READ_CONST_16BIT(&current_symbol->bitmap->height);
                }
            } else {
                offset_y = current_symbol->offset_y;
            }

            internal_draw_bitmap(inst, inst->current_x + current_symbol->offset_x, inst->current_y + offset_y, (dw_font_bitmap_t*)current_symbol->bitmap);

            inst->current_x += current_symbol->cur_dist;

            prev_symbol = current_symbol;
        }
    }
}
