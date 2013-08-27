#ifndef UBUPGRADETOOEFDELEGATE_H
#define UBUPGRADETOOEFDELEGATE_H


//#define DRY_RUN true

#include <QObject>
#include <QThread>
#include <QMap>
#include "UBDirectories.h"

#ifdef DRY_RUN
class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};
#endif


class UBUpgradeToOefDelegate;

class SizeCalculator : public QObject
{
    Q_OBJECT

public:
    SizeCalculator();
    ~SizeCalculator();

public slots:
    void process();

signals:
    void returnNecessarySize(qint64 totalSize);
    void finished();
    void error(QString err);
};


class Renamer : public QObject
{
    Q_OBJECT

public:
    Renamer();
    ~Renamer();

    void setIdsToImport(QList<PossiblesImport> ids) {mIdsToImport << ids;}
    void setDelegate(UBUpgradeToOefDelegate* del){mDelegate = del;}

public slots:
    void process();

private:
    QList<PossiblesImport> mIdsToImport;

    UBUpgradeToOefDelegate* mDelegate;

signals:
    void finished();
    void error(QString err);
    void log(QString s);
};

class UBUpgradeToOefDelegate : public QObject
{
    Q_OBJECT

public:
    explicit UBUpgradeToOefDelegate(QObject *parent = 0);

    int getNumberOfItemToImport(PossiblesImport id);

    void initImportThread();
    void initSizeCalculationThread();


    Renamer* worker(){return mWorker;}
    QThread* thread(){return mThread;}
    SizeCalculator* calculator(){return mCalculator;}
    QThread* calculatorThread(){return mCalculatorThread;}

private:
    Renamer* mWorker;
    QThread* mThread;
    SizeCalculator* mCalculator;
    QThread* mCalculatorThread;
};

#endif // UBUPGRADETOOEFDELEGATE_H
