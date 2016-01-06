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

#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QTimer>

#define LICENCE_TEXT                "/**\n" \
                                    "* The MIT License (MIT)\n" \
                                    "*\n" \
                                    "* Copyright (c) 2015 Deaware System (http://www.deaware.com)\n" \
                                    "* Author: Surawut Aonjaiaow\n" \
                                    "*         Burin Sapsiri\n" \
                                    "*\n" \
                                    "*\n" \
                                    "* Permission is hereby granted, free of charge, to any person obtaining a copy\n" \
                                    "* of this software and associated documentation files (the \"Software\"), to deal\n" \
                                    "* in the Software without restriction, including without limitation the rights\n" \
                                    "* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n" \
                                    "* copies of the Software, and to permit persons to whom the Software is\n" \
                                    "* furnished to do so, subject to the following conditions:\n" \
                                    "*\n" \
                                    "* The above copyright notice and this permission notice shall be included in\n" \
                                    "* all copies or substantial portions of the Software.\n" \
                                    "*\n" \
                                    "* THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n" \
                                    "* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n" \
                                    "* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n" \
                                    "* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n" \
                                    "* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n" \
                                    "* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN\n" \
                                    "* THE SOFTWARE.\n" \
                                    "*/"

#define HEADER_FILE                 "\"dw_font.h\""

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    p_freetype = new wrapFreeType(this);
}

Widget::~Widget()
{
    delete ui;
}

QList<QChar> Widget::getCharList()
{
    QList<QChar> c_list;
    uint16_t i=0;

    c_list.clear();

    /* Append basic latin character. */
    for (i=0x20; i<=0x7f; i++) {
        c_list.append(QChar(i));
    }

    /* Append Thai character. */
    for (i=0x0e00; i<=0x0e5f; i++) {
        c_list.append(QChar(i));
    }

    return c_list;
}

QString Widget::renderBitmap(const wrap_freetype_glyph_t& glyph)
{
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QString str = "";
    QString utf8 = "";
    uint16_t i = 0;

    utf8 = codec->fromUnicode(QChar(glyph.unicode)).toHex();

    str = QString("static const dw_font_bitmap_t symbol_%1 = {\n")
                  .arg(utf8);

    str += QString("    .width = %1,\n"
                   "    .height = %2,\n"
                   "    .data = {\n")
                   .arg(QString::number(glyph.bitmap_width))
                   .arg(QString::number(glyph.bitmap_height));

    for (i=0; i<glyph.bitmap_data.length(); i++) {
        str += QString().sprintf("        0x%.2x,\n", glyph.bitmap_data.at(i));
    }

    str += "    }\n";
    str += "};\n\n";

    return str;
}

QString Widget::generateHeader(void)
{
    return QString() + LICENCE_TEXT + "\n\n" + "#include " HEADER_FILE + "\n\n";
}

QString Widget::generateFooter(void)
{
    return "";
}

QString Widget::generateBitmapList(const QList<wrap_freetype_glyph_t>& glyph_list)
{
    QString str = "";
    uint16_t i = 0;

    for (i=0; i<glyph_list.length(); i++) {
        str += renderBitmap(glyph_list.at(i));
    }

    return str;
}

QString Widget::generateStruct(const QList<wrap_freetype_glyph_t>& glyph_list)
{
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QString utf8 = "";
    QString font_name = "";
    QString str = "";
    uint16_t i = 0;

    font_name = QString("font_%1%2")
                        .arg(p_freetype->getFamilyName().toLower())
                        .arg(p_freetype->getPixelSize())
                        .replace(' ', '_');

    str = QString("const dw_font_info_t %1 = {\n"
                 "    .count = %2,\n"
                 "    .font_size = %3,\n"
                 "    .height = %4,\n"
                 "    .symbols = {\n")
                 .arg(font_name)
                 .arg(glyph_list.length())
                 .arg(p_freetype->getPixelSize())
                 .arg(p_freetype->getHeight());

    for (i=0; i<glyph_list.length(); i++) {
        utf8 = codec->fromUnicode(QChar(glyph_list.at(i).unicode)).toHex();
        str += QString("        {.utf8=0x%1, .offset_x=%2, .offset_y=%3, .cur_dist=%4, .bitmap=&symbol_%1},\n")
                       .arg(utf8)
                       .arg(glyph_list.at(i).offset_x)
                       .arg(glyph_list.at(i).offset_y)
                       .arg(glyph_list.at(i).cur_dist);
    }

    str += "    }\n";
    str += "};\n\n";

    return str;
}

void Widget::on_btnExport_clicked()
{
    QList<wrap_freetype_glyph_t> glyph_list;
    wrap_freetype_glyph_t glyph;

    QList<QChar> c_list;
    uint16_t i = 0;
    QString filename = "";
    QString str = "";
    QString font_name = "";

    if (p_freetype->loadFont(ui->lblFilename->text()) != WRAP_FREEFONT_TRUE) {
        QMessageBox::warning(this, "Error", "File is not found or invalid file.", QMessageBox::Ok);
        return;
    }

    if ((p_freetype->setPixelSize(ui->lblFontSize->text().toInt()) != WRAP_FREEFONT_TRUE) || (ui->lblFontSize->text().toInt() == 0)) {
        QMessageBox::warning(this, "Error", "Font size is not support.", QMessageBox::Ok);
        return;
    }

    c_list.clear();

    c_list = getCharList();

    for (i=0; i<c_list.length(); i++) {
        p_freetype->renderGlyph(c_list.at(i), &glyph);
        glyph_list.append(glyph);
    }

    str += generateHeader();
    str += generateBitmapList(glyph_list);
    str += generateStruct(glyph_list);
    str += generateFooter();

    font_name = QString("font_%1%2")
                        .arg(p_freetype->getFamilyName().toLower())
                        .arg(p_freetype->getPixelSize())
                        .replace(' ', '_');

    filename = QFileDialog::getSaveFileName(this, tr("Save Font"), font_name + ".c", tr("Source Files (*.c *.cpp)"));

    if (!filename.isEmpty()) {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        out << str;
        file.close();
    }
}


void Widget::on_btnBrowse_clicked()
{
    QString filename = "";

    filename = QFileDialog::getOpenFileName(this, tr("Open Font"), "", tr("Font Files (*.ttf *.otf)"));

    if (!filename.isEmpty()) {
        ui->lblFilename->setText(filename);
    }
}
