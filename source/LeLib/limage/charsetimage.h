/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef CHARSETIMAGE_H
#define CHARSETIMAGE_H

#include "multicolorimage.h"
#include "source/LeLib/util/util.h"
#include <QPixmap>


class CharsetImage : public MultiColorImage
{
public:
    int m_charCount = 256;
    QByteArray m_rawData;
    bool m_isMultiColor = false;
    PixelChar m_color;
    CharsetImage(LColorList::Type t);

    int m_skipImportBytes = 0;
    PixelChar m_copy[64];

    int m_colorOrderType = 0; // C64

    enum Mode{ FULL_IMAGE, CHARSET1x1, CHARSET2x2, CHARSET2x2_REPEAT};

    QString GetCurrentModeString() override {
        if (m_currentMode==CHARSET1x1) return "1x1 charset mode";
        if (m_currentMode==CHARSET2x2) return "2x2 charset mode";
        if (m_currentMode==CHARSET2x2_REPEAT) return "2x2 charset repeat mode";

        return "Full image mode";
    }

    int FindClosestChar(PixelChar p);

    Mode m_currentMode = FULL_IMAGE;

    QString GetCurrentDataString() override {
        return "  Character : " + Util::numToHex(m_currencChar);
    }




    void SetColor(uchar col, uchar idx) override;

    void SaveBin(QFile& file) override;
    void LoadBin(QFile& file) override;

    uchar getVariableColor(PixelChar* pc);


    void LoadCharset(QString file, int skipBytes) override;



    CharsetImage* getCharset() override { return this; }
    unsigned int getPixel(int x, int y) override;

    void ImportBin(QFile& f) override;
    void ExportBin(QFile& f) override;
    virtual void FromRaw(QByteArray& arr);
    virtual void ToRaw(QByteArray& arr);
    virtual void ToQPixMaps(QVector<QPixmap>& map);
    virtual QPixmap ToQPixMap(int chr);
    virtual void setPixel(int x, int y, unsigned int color) override;

    virtual unsigned int getCharPixel(int pos, int pal, int x, int y);


    void RenderEffect(QMap<QString, float> params) override;

    void CopyFrom(LImage* mc);
    bool KeyPress(QKeyEvent *e) override;

    void setLimitedPixel(int x, int y, unsigned int color);

    void onFocus() override;

    void FlipHorizontal() override;
    void FlipVertical() override;

    void CopyChar() override;
    void PasteChar() override;

};

#endif // CHARSETIMAGE_H
