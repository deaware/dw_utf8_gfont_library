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

#include "wrapfreetype.h"

wrapFreeType::wrapFreeType(QObject *parent) :
    QObject(parent)
{
    int8_t error = 0;

    error = FT_Init_FreeType( &library );
}

wrapFreeType::~wrapFreeType()
{
    FT_Done_Face(face);
}

int8_t wrapFreeType::loadFont(const QString& filename)
{
    int8_t error = 0;

    error = FT_New_Face(library, filename.toStdString().c_str(), 0, &face);

    if (!error) {
        return WRAP_FREEFONT_TRUE;
    } else {
        return WRAP_FREEFONT_FALSE;
    }
}

QString wrapFreeType::getFamilyName(void)
{
    QString font_name = "";
    FT_SfntName name;

    FT_Get_Sfnt_Name(face, 1, &name);
    font_name += QString::fromLocal8Bit((char*)name.string, name.string_len);

    FT_Get_Sfnt_Name(face, 2, &name);
    font_name += " " + QString::fromLocal8Bit((char*)name.string, name.string_len);

    return  font_name;
}

int8_t wrapFreeType::setPixelSize(uint16_t size)
{
    int8_t error = 0;
    uint16_t glyph_index = 0;

    error = FT_Set_Char_Size(face,    /* handle to face object           */
                             0,       /* char_width in 1/64th of points  */
                             size*64, /* char_height in 1/64th of points */
                             72,      /* horizontal device resolution    */
                             72 );    /* vertical device resolution      */

    glyph_index = FT_Get_Char_Index( face, 0x20 );

    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );

    height = (uint16_t)face->glyph->metrics.vertAdvance/64;

    if ( error ) {
        perror ("Could not load glyph.\r\n");
        return WRAP_FREEFONT_FALSE;
    }

    if (!error) {
        pixel_size = size;
        return WRAP_FREEFONT_TRUE;
    } else {
        return WRAP_FREEFONT_FALSE;
    }
}

uint16_t wrapFreeType::getPixelSize(void)
{
    return pixel_size;
}

uint16_t wrapFreeType::getHeight(void)
{
    return height;
}

int8_t wrapFreeType::renderGlyph(QChar c, wrap_freetype_glyph_t *glyph)
{
    int8_t error = 0;
    uint16_t glyph_index = 0;
    uint16_t x = 0;
    uint16_t y = 0;
    uint32_t i = 0;
    uint8_t b = 0;
    uint8_t b_index = 0;

    initGlyph(glyph);

    glyph->unicode = c.unicode();
    glyph->pixel_size = pixel_size;

    glyph_index = FT_Get_Char_Index( face, c.unicode() );

    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );

    if ( error ) {
        perror ("Could not load glyph.\r\n");
        return WRAP_FREEFONT_FALSE;
    }

    /* convert to an anti-aliased bitmap */
    error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );

    if ( error ) {
        perror ("Could not render glyph.\r\n");
        return WRAP_FREEFONT_FALSE;
    }

    glyph->cur_dist = (int16_t)face->glyph->metrics.horiAdvance/64;
    glyph->offset_x = (int16_t)face->glyph->metrics.horiBearingX/64;
    glyph->offset_y = -(int16_t)face->glyph->metrics.horiBearingY/64;

    glyph->bitmap_width = face->glyph->bitmap.width;
    glyph->bitmap_height = face->glyph->bitmap.rows;

    i = 0;
    b = 0;
    b_index = 7;
    for (y=0; y<face->glyph->bitmap.rows; y++) {
        for (x=0; x<face->glyph->bitmap.width; x++) {
            /* Convert gray-scale to mono. */
            if (face->glyph->bitmap.buffer[i] > 127) {
                b |= (1 << b_index);
            }

            if (b_index <= 0) {
                glyph->bitmap_data.append(b);
                b = 0;
                b_index = 7;
            } else {
                b_index--;
            }
            i++;
        }
    }

    if (b_index < 7) {
        glyph->bitmap_data.append(b);
    }

    return WRAP_FREEFONT_TRUE;
}

void wrapFreeType::initGlyph(wrap_freetype_glyph_t *glyph)
{
    glyph->bitmap_data.clear();
    glyph->bitmap_width = 0;
    glyph->bitmap_height = 0;
    glyph->cur_dist = 0;
    glyph->offset_x = 0;
    glyph->offset_y = 0;
    glyph->unicode = 0;
    glyph->pixel_size = 0;
}

