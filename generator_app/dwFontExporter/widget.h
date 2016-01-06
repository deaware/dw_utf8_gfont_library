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

#ifndef WIDGET_H
#define WIDGET_H

#include <stdint.h>
#include <QWidget>
#include <QFileDialog>
#include <QTextCodec>
#include <QMessageBox>
#include "wrapfreetype.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_btnExport_clicked(void);

    void on_btnBrowse_clicked();

private:
    QList<QChar> getCharList();
    QString renderBitmap(const wrap_freetype_glyph_t& glyph);
    QString generateHeader(void);
    QString generateFooter(void);
    QString generateBitmapList(const QList<wrap_freetype_glyph_t>& glyph_list);
    QString generateStruct(const QList<wrap_freetype_glyph_t>& glyph_list);

private:
    Ui::Widget      *ui;
    wrapFreeType    *p_freetype;
};

#endif // WIDGET_H
