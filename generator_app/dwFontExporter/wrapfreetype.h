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

#ifndef WRAPFREETYPE_H
#define WRAPFREETYPE_H

#include <QObject>
#include <stdint.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_SFNT_NAMES_H

enum {
    WRAP_FREEFONT_FALSE = 0,
    WRAP_FREEFONT_TRUE
};

typedef struct {
    uint16_t unicode;
    uint16_t pixel_size;
    uint16_t bitmap_width;
    uint16_t bitmap_height;
    QList<uint8_t> bitmap_data;
    int16_t offset_x;
    int16_t offset_y;
    int16_t cur_dist;
}wrap_freetype_glyph_t;

class wrapFreeType : public QObject
{
    Q_OBJECT
public:
    explicit wrapFreeType(QObject *parent = 0);
    ~wrapFreeType();
    int8_t loadFont(const QString& filename);
    QString getFamilyName(void);
    int8_t setPixelSize(uint16_t size);
    uint16_t getPixelSize(void);
    uint16_t getHeight(void);
    int8_t renderGlyph(QChar c, wrap_freetype_glyph_t *glyph);

signals:

public slots:

private:
    void initGlyph(wrap_freetype_glyph_t *glyph);

private:
    FT_Library  library;
    FT_Face     face;
    uint16_t    pixel_size;
    uint16_t    height;
};

#endif // WRAPFREETYPE_H
