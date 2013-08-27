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
