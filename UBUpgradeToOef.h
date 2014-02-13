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


#ifndef UBUPGRADETOOEF_H
#define UBUPGRADETOOEF_H

#include <QStackedWidget>
#include <QWidget>
#include <Qt>
#include <QTextEdit>


#include "UBUpgradeToOefDelegate.h"
#include "UBActionWidget.h"

class QLabel;
class QPushButton;
class QRadioButton;
class QCheckBox;
class QButtonGroup;

class UBUpgradeToOEF : public QStackedWidget
{
    Q_OBJECT
public:
    explicit UBUpgradeToOEF(QWidget *parent = 0,UBUpgradeToOefDelegate* delegate = 0);

signals:

private slots:
    void nextWidgetInStack();
    void onShowImportingWidget();
    void onContinueClick();
    void appendImportingLog(QString log);
    void onFileChooserClicked();
    void onNecessarySizeCalculated(qint64);


private:
    void init();

    UBActionWidget* loggerWidget(QString loggerTitle, QTextEdit** logger);

    QWidget* mInitialWidget;
    QWidget* initialWidget();
    QTextEdit* mNecessaryDiskSpaceInfo;

    UBActionWidget* mChooseImportsWidget;
    QWidget* chooseImportsWidget();
    QLabel* mCurrentPath;

    UBActionWidget* mImportingWidget;
    QTextEdit* mImportingLogger;

    QWidget* mFinalWidget;
    QWidget* finalWidget();

    UBUpgradeToOefDelegate* mDelegate;

    QCheckBox* createCheckBox(PossiblesImport id, QString label);

    QMap<PossiblesImport,QCheckBox*> checkBoxes;

    QButtonGroup* mButtonGroup;

};

#endif // UBUPGRADETOOEF_H
