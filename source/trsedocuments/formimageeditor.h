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

#ifndef FormImageEditor_H
#define FormImageEditor_H

#include <QWidget>
#include "trsedocument.h"
#include "source/toolbox.h"
#include "source/LeLib/limage/limage.h"
#include "source/LeLib/limage/limageqimage.h"
#include "imageworker.h"
#include <QListWidget>
#include <QAbstractItemView>
#include <QListWidgetItem>
#include "source/LeLib/limage/limageio.h"
#include "dialognewimage.h"
#include "dialogimport.h"
#include "source/dialogexport.h"
#include "source/LeLib/util/util.h"
#include "source/LeLib/limage/limageeffects.h"
#include "source/dialogimagehelp.h"
#include "source/qlabellimage.h"
#include "source/LeLib/limage/limagefactory.h"
#include "source/LeLib/limage/limagecontainer.h"
#include "source/dialogcolors.h"
#include "source/LeLib/glwidget.h"
#include <QLineEdit>
#include <QItemDelegate>
namespace Ui {
class Formimageeditor;
}


class ByteDelegate : public QItemDelegate
{
public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                      const QModelIndex & index) const
    {
        QLineEdit *lineEdit = new QLineEdit(parent);
        // Set validator
        QIntValidator *validator = new QIntValidator(0, 255, lineEdit);
        lineEdit->setValidator(validator);
        return lineEdit;
    }
};


class FormImageEditor :  public TRSEDocument
{
    Q_OBJECT

public:
    explicit FormImageEditor(QWidget *parent = 0);
    ~FormImageEditor();
    ImageWorker m_work;

    WorkerThread m_updateThread;
    bool m_ignoreMC = false;
    LImageQImage m_grid;
    Toolbox m_toolBox;
    LColorList* m_currentColorList = nullptr;
    LImageEffects m_imageEffects;
    QMap<QString, QLineEdit*> m_imageEffectsLineEdits;
    LImageEffect* m_currentImageEffect = nullptr;
    QString m_projectPath;
    QVector<int> m_keepSpriteChar;
    QString m_currentFilename = "";

    int m_oldWidth = 600;

    int m_prefMode=1, m_keepMode=0;
//    CharsetImage::Mode m_prefMode = CharsetImage::Mode::CHARSET1x1;
//    CharsetImage::Mode m_keepMode = CharsetImage::Mode::CHARSET1x1;

    void UpdatePalette();
    void updateCharSet();
    void updateSingleCharSet();

    void PrepareImageTypeGUI();

    void SetSingleCharsetEdit();

    void SetFooterData(int pos, uchar val);
    uchar GetFooterData(int pos);


    void SetButton(QPushButton* btn, LImage::GUIType type);
    void SetLabel(QLabel* btn, LImage::GUIType type);

    void PrepareClose() override;

    void SetMCColors();
    void UpdateLevels();
    void FillCMBColors();
    virtual void focusInEvent( QFocusEvent* ) override;


    void OpenSelectCharset();
    void Reload() override;

    bool eventFilter(QObject *ob, QEvent *e) override;

    void resizeEvent(QResizeEvent *event) override;

    void SelectFromLeftClick();

    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void UpdateImage();

    void UpdateGrid();

    void Load(QString filename) override;
    void Save(QString filename) override;

    void FillImageEffect();
    void FillToImageParams();

    Ui::Formimageeditor *getUi() const;

    GLWidget* getLabelImage();

    void UpdateCurrentMode();

    void InitDocument(WorkerThread* t, CIniFile* ini, CIniFile* iniProject) override;


    void GenericExportImage(QString type, QString ext);
    void GenericImportImage(QString type, QString ext);

    void UpdateSpriteImages();

    void UpdateAspect();

    void Update();

/*signals:
    void EmitMouseEvent();
*/
private:
    Ui::Formimageeditor *ui;

private slots:
    void InitAspect();
    void onSwapDisplayMode();
    void onImageMouseEvent();
    void onImageMouseReleaseEvent();

    void on_btnExportAsm_clicked();

    void on_btnGenerate_clicked();

    void showDetailCharButtons();

    void on_btnFlipVert_clicked();
    void on_btnFlipHorisontal_clicked();

    void SelectCharacter(int j);

    void on_cmbEffect_currentIndexChanged(int index);




//    void on_btnLoad_clicked();

 //   void on_btnSave_clicked();

    void on_chkGrid_clicked(bool checked);
    void on_chkBackgroundArea_clicked(bool checked);

    // Actually clear4
    void on_btnNew_clicked();

    void on_btnExportImage_clicked();

    void on_lstImages_clicked(const QModelIndex &index);


    void on_btnImport_clicked();

    void on_btnCharsetFull_clicked();
    void on_btnCharsetCopy_clicked();
    void on_btnCharsetPaste_clicked();







    void on_btnImportBin_clicked();

    void on_btnExportBin_clicked();

    void on_tabMain_currentChanged(int index);

    void on_btnLoadCharmap_clicked();

    void on_lstCharMap_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

    void Destroy();

    void on_cmbMC1_activated(int index);

    void on_cmbMC2_activated(int index);

    void on_cmbBackgroundMain_3_activated(int index);



    void on_btnResizeData_clicked();

    void on_chkDisplayMulticolor_stateChanged(int arg1);
    void on_leHeaders_editingFinished();

    void on_btnHelpImage_clicked();

    void on_btnExportKoala_clicked();
    void on_btnImportKoala_clicked();
    void on_btnNewSprite_clicked();
    void on_btnDeleteSprite_clicked();
    void on_btnCopySprite_clicked();
    void on_btnPasteSprite_clicked();
    void on_btnFlipXSprite_clicked();
    void on_btnFlipYSprite_clicked();
    void on_sliderX_actionTriggered(int action);
    void on_btnPanLeft_clicked();
    void on_btnPanRight_clicked();
    void on_btnPanUp_clicked();
    void on_btnPanDown_clicked();
    void on_lblSprite1_clicked();
    void on_lblSprite3_clicked();
    void on_btnMoveSpriteLeft_clicked();
    void on_btnMoveSpriteRight_clicked();
    void on_btnSpriteMulticolor_clicked();
    void on_btnImportRom_clicked();
    void on_btnExportMovie_clicked();
    void on_leTimeStamp_textChanged(const QString &arg1);
    void on_btnImportC_clicked();
    void on_cmbZoomLevel_activated(const QString &arg1);
    void on_btnExportCompressed_clicked();
    void on_btnPalette_clicked();
    void on_cmbBorderMain_3_currentIndexChanged(int index);
    void on_cmbMC1_currentIndexChanged(int index);
    void on_cmbBorderMain_3_activated(int index);
    void on_cmbNesPalette_currentIndexChanged(int index);
    void on_cmbBank_currentIndexChanged(int index);
    void on_cmbNesPalette_activated(int index);
    void on_btnCharSelect_clicked();
    void on_cmbAspect_currentIndexChanged(int index);
    void on_chkPaintSeparately_stateChanged(int arg1);
    void on_pushButton_clicked();
    void on_cmbCharX_currentIndexChanged(int index);
    void on_cmbCharY_currentIndexChanged(int index);
    void on_btnRepeating_clicked();
    void on_btnShiftLeft_clicked();
    void on_btnShiftRight_clicked();
    void on_btnShiftDown_clicked();
    void on_btnShiftUp_clicked();
};


#endif // FormImageEditor_H
