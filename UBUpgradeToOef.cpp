/*
 * Copyright (C) 2013-2014 Open Education Foundation
 *
 *
 * This file is part of OpenBoard.
 *
 * OpenBoard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License,
 * with a specific linking exception for the OpenSSL project's
 * "OpenSSL" library (or with modified versions of it that use the
 * same license as the "OpenSSL" library).
 *
 * OpenBoard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenBoard. If not, see <http://www.gnu.org/licenses/>.
 */



#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QThread>
#include <QDebug>
#include <QPainter>
#include <QBitmap>
#include <QFont>
#include <QFileDialog>
#include "UBUpgradeToOef.h"

UBUpgradeToOEF::UBUpgradeToOEF(QWidget *parent, UBUpgradeToOefDelegate* delegate) :
    QStackedWidget(parent)
  , mDelegate(delegate)
{
    setAcceptDrops(false);
    setObjectName("OEFUpgradeControlWidget");

    setLayout(new QHBoxLayout());

    //setWindowModality(Qt::WindowModal);
#ifdef Q_OS_MACX
    setWindowFlags(Qt::FramelessWindowHint);
#else
    //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
#endif
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    setStyleSheet(QString("QWidget#OEFUpgradeControlWidget {border-width: 10px;  border-style : solid; border-color: lightgrey;  background: white;}"));

    init();

    setMinimumSize(600,480);
    move(QApplication::desktop()->availableGeometry().center() - rect().center());
    show();


    QBitmap objBitmap(size());
    QPainter painter(&objBitmap);

    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(rect(),9,9);
    setMask(objBitmap);


    connect(mImportingWidget,SIGNAL(actionTrig()),this,SLOT(onShowImportingWidget()));

}

void UBUpgradeToOEF::init()
{
    int i=0;
    insertWidget(i++,initialWidget());
    mDelegate->initSizeCalculationThread();
    connect(mDelegate->calculator(), SIGNAL(returnNecessarySize(qint64)),
            this, SLOT(onNecessarySizeCalculated(qint64)));
    mDelegate->calculatorThread()->start();

    insertWidget(i++,chooseImportsWidget());
    mImportingWidget = loggerWidget(tr("Importing ..."), &mImportingLogger);
    insertWidget(i++,mImportingWidget);
    insertWidget(i++,finalWidget());
    setCurrentWidget(mInitialWidget);
}


QWidget* UBUpgradeToOEF::initialWidget()
{
    mInitialWidget = new QWidget(this);

    mInitialWidget->setObjectName("InitialWidget");

    QVBoxLayout* vBoxLayout = new QVBoxLayout(mInitialWidget);
    mInitialWidget->setLayout(vBoxLayout);

    QLabel* title = new QLabel(mInitialWidget);
    title->setText(tr("OpenBoard Document Importer"));
    title->setStyleSheet("font-size : 24px; font-weight : bold;");
    vBoxLayout->addWidget(title);

    QTextEdit* description = new QTextEdit(mInitialWidget);
    description->setStyleSheet("border-style : none; font-size : 14px;");
    description->setReadOnly(true);
    description->setText(tr("Please choose Open-Sankore documents you want to import to OpenBoard."));
    vBoxLayout->addWidget(description);

    mButtonGroup = new QButtonGroup(this);

    QRadioButton* mPerformBasicImport = new QRadioButton(mInitialWidget);
    mPerformBasicImport->setStyleSheet("border-style : none; font-size : 14px;");
    mPerformBasicImport->setText(tr("Basic importation.\nImport all your data to OpenBoard."));

    QRadioButton* mPerformAdvancedImport = new QRadioButton(mInitialWidget);
    mPerformAdvancedImport->setStyleSheet("border-style : none; font-size : 14px;");
    mPerformAdvancedImport->setText(tr("Advanced importation.\nChoose the documents you want to import to OpenBoard."));

    mButtonGroup->addButton(mPerformBasicImport, 0);
    mButtonGroup->addButton(mPerformAdvancedImport, 1);

    mPerformBasicImport->setChecked(true);

    vBoxLayout->addWidget(mPerformBasicImport);
    vBoxLayout->addWidget(mPerformAdvancedImport);
    vBoxLayout->addStretch();

    mNecessaryDiskSpaceInfo = new QTextEdit(mInitialWidget);
    mNecessaryDiskSpaceInfo->setStyleSheet("border-style : none; font-size : 14px;");
    mNecessaryDiskSpaceInfo->setText(QString(tr("Importing your documents will require at least 0 MB of disk space, make sure this space is available on your hard drive.")));
    mNecessaryDiskSpaceInfo->setReadOnly(true);

    vBoxLayout->addWidget(mNecessaryDiskSpaceInfo);
    vBoxLayout->addStretch();

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* cancelButton = new QPushButton(mInitialWidget);
    cancelButton->setText(tr("Cancel"));
    connect(cancelButton,SIGNAL(clicked()),qApp,SLOT(quit()));

    QPushButton* continueButton = new QPushButton(mInitialWidget);
    continueButton->setText(tr("Continue"));
    connect(continueButton,SIGNAL(clicked()),this,SLOT(onContinueClick()));

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(continueButton);

    vBoxLayout->addLayout(buttonLayout);


    return mInitialWidget;
}

void UBUpgradeToOEF::onNecessarySizeCalculated(qint64 size)
{

    qint64 totalSize = size;
    totalSize /= (1024 * 1024);
    QString text = QString(tr("Importing your documents will require at least %1 MB of disk space, make sure this space is available on your hard drive.")).arg(totalSize);
    mNecessaryDiskSpaceInfo->setText(text);
}

void UBUpgradeToOEF::onFileChooserClicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Backup directory"),UBDirectories::homePath());

    if(fileName.length())
        mCurrentPath->setText(fileName);
}

QCheckBox* UBUpgradeToOEF::createCheckBox(PossiblesImport id, QString label)
{
    QCheckBox* checkBox = new QCheckBox(mChooseImportsWidget);
    int numberOfItemToImport = mDelegate->getNumberOfItemToImport(id);
    if(numberOfItemToImport)
        checkBox->setChecked(true);
    else
        checkBox->setDisabled(true);

    checkBox->setStyleSheet("border-style : none; font-size : 14px;");

    checkBox->setText(label);

    return checkBox;
}

QWidget* UBUpgradeToOEF::chooseImportsWidget()
{
    mChooseImportsWidget = new UBActionWidget(this);
    mChooseImportsWidget->setObjectName("ChooseImportsWidget");

    QVBoxLayout* vBoxLayout = new QVBoxLayout(mChooseImportsWidget);
    mChooseImportsWidget->setLayout(vBoxLayout);

    QLabel* title = new QLabel(mChooseImportsWidget);
    title->setText(tr("Select the Items to Import"));
    title->setStyleSheet("font-size : 24px; font-weight : bold;");
    vBoxLayout->addWidget(title);
    vBoxLayout->addStretch();

    QGridLayout* gridLayout = new QGridLayout(0);

    checkBoxes.insert(Documents,createCheckBox(Documents,tr("Documents")));
    checkBoxes.insert(AnimationUserDirectory, createCheckBox(AnimationUserDirectory,tr("Flash")));
    checkBoxes.insert(Applications, createCheckBox(Applications,tr("Application")));
    checkBoxes.insert(InteractiveContents, createCheckBox(InteractiveContents,tr("Interactivities")));
    checkBoxes.insert(InteractiveFavorites, createCheckBox(InteractiveFavorites,tr("Interactive Favorites")));
    checkBoxes.insert(Library, createCheckBox(Library,tr("Library")));
    checkBoxes.insert(UniboardUserConfig, createCheckBox(UniboardUserConfig,tr("User Configuration")));
    checkBoxes.insert(Audios, createCheckBox(Audios,tr("Audios")));
    checkBoxes.insert(Movies, createCheckBox(Movies,tr("Movies")));
    checkBoxes.insert(Pictures, createCheckBox(Pictures,tr("Pictures")));
    int counter = 0;
    foreach(QCheckBox* eachBox, checkBoxes.values()){
        gridLayout->addWidget(eachBox, counter/2,counter%2);
        counter += 1;
    }

    vBoxLayout->addLayout(gridLayout);
    vBoxLayout->addStretch();

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* cancelButton = new QPushButton(mInitialWidget);
    cancelButton->setText(tr("Cancel"));
    connect(cancelButton,SIGNAL(clicked()),qApp,SLOT(quit()));

    QPushButton* importButton = new QPushButton(mInitialWidget);
    importButton->setText(tr("Import"));
    connect(importButton,SIGNAL(clicked()),this,SLOT(nextWidgetInStack()));

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(importButton);

    vBoxLayout->addLayout(buttonLayout);


    return mChooseImportsWidget;
}


UBActionWidget *UBUpgradeToOEF::loggerWidget(QString loggerTitle, QTextEdit** logger)
{
    UBActionWidget* result = new UBActionWidget(this);
    result->setObjectName(loggerTitle);

    QVBoxLayout* vBoxLayout = new QVBoxLayout(result);
    result->setLayout(vBoxLayout);

    QLabel* title = new QLabel(result);
    title->setText(loggerTitle);
    title->setStyleSheet("font-size : 24px; font-weight : bold;");
    vBoxLayout->addWidget(title);

    *logger = new QTextEdit(result);
    (*logger)->setAcceptDrops(false);
    (*logger)->ensureCursorVisible();
    (*logger)->setContextMenuPolicy(Qt::NoContextMenu);
    (*logger)->setCursorWidth(0);
    (*logger)->setReadOnly(true);
    (*logger)->setStyleSheet("border-style : none; font-size : 14px;");
    vBoxLayout->addWidget(*logger);

    return result;
}


QWidget* UBUpgradeToOEF::finalWidget()
{
    mFinalWidget = new QWidget(this);
    mFinalWidget->setObjectName("FinalWidget");

    QVBoxLayout* vBoxLayout = new QVBoxLayout(mFinalWidget);
    mFinalWidget->setLayout(vBoxLayout);

    QLabel* title = new QLabel(mFinalWidget);
    title->setStyleSheet("font-size : 24px; font-weight : bold;");
    title->setText(tr("Import Finished"));
    vBoxLayout->addWidget(title);

    QTextEdit* description = new QTextEdit(mFinalWidget);
    description->setReadOnly(true);
    description->setStyleSheet("border-style : none; font-size : 14px;");
    description->setText(tr("All the documents have been imported. You can now launch OpenBoard to access your documents."));
    vBoxLayout->addWidget(description);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* finishedButton = new QPushButton(mFinalWidget);
    finishedButton->setText(tr("Finished"));
    connect(finishedButton ,SIGNAL(clicked()),qApp,SLOT(quit()));

    buttonLayout->addStretch();
    buttonLayout->addWidget(finishedButton);

    vBoxLayout->addLayout(buttonLayout);
    return mFinalWidget;
}


void UBUpgradeToOEF::onShowImportingWidget()
{
    mDelegate->initImportThread();
    connect(mDelegate->thread(),SIGNAL(finished()),this,SLOT(nextWidgetInStack()));
    connect(mDelegate->worker(),SIGNAL(log(QString)),this,SLOT(appendImportingLog(QString)));
    QList<PossiblesImport> ids;

    foreach(PossiblesImport id, checkBoxes.keys()){
        if(checkBoxes.value(id)->isEnabled() && checkBoxes.value(id)->isChecked())
            ids.append(id);
    }
    mDelegate->worker()->setDelegate(mDelegate);
    mDelegate->worker()->setIdsToImport(ids);
    mDelegate->thread()->start();

}

void UBUpgradeToOEF::appendImportingLog(QString log)
{
    mImportingLogger->append(log);
}

void UBUpgradeToOEF::onContinueClick()
{
    if(mButtonGroup->checkedId() == 0)
        setCurrentWidget(mImportingWidget);
    else
        nextWidgetInStack();

}

void UBUpgradeToOEF::nextWidgetInStack()
{
    setCurrentIndex(currentIndex()+1);
}
