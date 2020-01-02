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

#include "imageworker.h"
#include <QDebug>
#include <QStandardItemModel>

ImageWorker::ImageWorker()
{

    m_types.append(ImageType("Fake C64 png Palette 1", LImage::Type::QImageBitmap, LColorList::Type::C64));
    m_types.append(ImageType("Fake C64 png Palette 2", LImage::Type::QImageBitmap, LColorList::Type::C64_ORG));
    m_types.append(ImageType("CGA png Palette 1", LImage::Type::CGA, LColorList::Type::CGA1_HIGH));
    m_types.append(ImageType("Amiga 320x256", LImage::Type::AMIGA320x256, LColorList::Type::C64));
    m_types.append(ImageType("Amiga 320x200", LImage::Type::AMIGA320x200, LColorList::Type::C64));
    m_types.append(ImageType("C64 MultiColor bitmap", LImage::Type::MultiColorBitmap,LColorList::Type::C64));
    m_types.append(ImageType("C64 Hires Bitmap", LImage::Type::HiresBitmap,LColorList::Type::C64));
    m_types.append(ImageType("C64 Multicolor Charmap", LImage::Type::CharMapMulticolor,LColorList::Type::C64));
    m_types.append(ImageType("C64 Regular Charmap", LImage::Type::CharmapRegular,LColorList::Type::C64));
    m_types.append(ImageType("Screen animation", LImage::Type::FullScreenChar,LColorList::Type::C64));
    m_types.append(ImageType("C64 Level Editor", LImage::Type::LevelEditor,LColorList::Type::C64));
    m_types.append(ImageType("NES Level Editor", LImage::Type::LevelEditorNES,LColorList::Type::NES));
    m_types.append(ImageType("C64 Sprite Editor", LImage::Type::Sprites2,LColorList::Type::C64));
    m_types.append(ImageType("VIC20 Multicolor bitmap", LImage::Type::VIC20_MultiColorbitmap,LColorList::Type::VIC20));
    m_types.append(ImageType("OK64 Image", LImage::Type::OK64_256x256,LColorList::Type::OK64));
    m_types.append(ImageType("X16 640x480 Image", LImage::Type::X16_640x480,LColorList::Type::X16));
    m_types.append(ImageType("PICO 8", LImage::Type::QImageBitmap, LColorList::Type::PICO8));
    m_types.append(ImageType("NES CHR", LImage::Type::NES, LColorList::Type::NES));
    m_types.append(ImageType("NES Meta chunk editor", LImage::Type::LMetaChunk, LColorList::Type::NES));
    m_types.append(ImageType("NES Sprite editor", LImage::Type::SpritesNES, LColorList::Type::NES));
    m_types.append(ImageType("C64 Sprite Editor (deprecated type)", LImage::Type::Sprites,LColorList::Type::C64));
//    m_types.append(ImageType("C64 Multicolor Charmap Fixed Colors", LImage::Type::CharMapMultiColorFixed,LColorList::Type::C64));

    /*    m_types.append(ImageType("CGA Palette 1 Lo", LImage::Type::QImageBitmap,LColorList::Type::CGA1_LOW));
    m_types.append(ImageType("CGA Palette 1 Hi", LImage::Type::QImageBitmap,LColorList::Type::CGA1_HIGH));
    m_types.append(ImageType("CGA Palette 2 Lo", LImage::Type::QImageBitmap,LColorList::Type::CGA2_LOW));
    m_types.append(ImageType("CGA Palette 2 Hi", LImage::Type::QImageBitmap,LColorList::Type::CGA2_HIGH));
    m_types.append(ImageType("TIFF", LImage::Type::Tiff,LColorList::Type::TIFF));
*/

    //New(5,);
}

ImageWorker::~ImageWorker()
{
    for (ImageEdit* ie: m_images)
        delete ie;
    m_images.clear();
}

ImageType *ImageWorker::findType(LImage::Type imageType, LColorList::Type colType)
{
    for (int i=0;i<m_types.count();i++) {
        ImageType* it=&m_types[i];
        if (it->colorType==colType && it->type == imageType)
            return it;
    }
    return nullptr;//&m_types[0];
}

void ImageWorker::UpdateListView(QListView *lst)
{
    QStandardItemModel *model = new QStandardItemModel( m_images.count(), 1, nullptr);
    for (int i=0;i<m_images.count();i++) {
        QStandardItem *item = new QStandardItem(m_images[i]->m_name);
        model->setItem(i,0, item);
    }

    lst->setModel(model);

}


QStringList ImageWorker::getImageTypes()
{
    QStringList l;
    for (ImageType it : m_types)
        l<< it.name;
    return l;
}

void ImageWorker::New(LImage *image, QString name)
{
    ImageType* imageType = findType(image->m_type, image->m_colorList.m_type);

    if (imageType == nullptr) {
        qDebug() << "WTF NULLPTR imagetype ";
        return;
    }
    m_currentImage = new ImageEdit(image, imageType, name);
    //m_currentImage = new ImageEdit(imageType, "New Image");
    m_images.append(m_currentImage);
    Data::data.redrawFileList = true;
    Data::data.Redraw();
}

void ImageWorker::New(LImage* img, int image)
{
//    exit(1);

    m_currentImage = new ImageEdit(&m_types[image], "New Image");

    m_currentImage->Initialize(img);
    m_images.append(m_currentImage);
    m_currentImage->m_fileName="";

    Data::data.redrawFileList = true;
}

void ImageWorker::SetImage(int cur)
{
    if (cur>=0 && cur<m_images.count())
        m_currentImage = m_images[cur];

}



