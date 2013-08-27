
#include "UBUpgradeToOefDelegate.h"
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <QStack>

#include "UBDirectories.h"

SizeCalculator::SizeCalculator()
{
}

SizeCalculator::~SizeCalculator()
{
}

void SizeCalculator::process()
{
    qint64 totalSize = 0;

    for(int i = 0; i < MAX_POSSIBLES_IMPORT; i +=1 ){
        if(i != USER_PATHS_COUNTER){
            QString directoryPath = UBDirectories::basicPath((PossiblesImport)i) + QDir::separator() + "Sankore" + QDir::separator() + UBDirectories::subPathFor((PossiblesImport)i);
            qint64 eachSize = UBDirectories::sizeOfDirectory(directoryPath);
            totalSize += eachSize;
        }
    }
    emit returnNecessarySize(totalSize);
    emit finished();
}



Renamer::Renamer()
{
}

Renamer::~Renamer()
{
}

void Renamer::process()
{
    foreach(PossiblesImport eachId, mIdsToImport){

        QString originPath = UBDirectories::basicPath(eachId) + QDir::separator() + "Sankore" + QDir::separator() + UBDirectories::subPathFor(eachId);

        QString destinationPath = UBDirectories::basicPath(eachId) + QDir::separator() + "OpenBoard" + QDir::separator() + UBDirectories::subPathFor(eachId);

        qDebug() << "originPath " << originPath << " destinationPath " << destinationPath;

        if(eachId == UniboardUserConfig){
            emit log(originPath);
            destinationPath = destinationPath.replace("Uniboard","OpenBoard");
#ifdef DRY_RUN
            qDebug() << QString("from %1\nto %2\n\n").arg(originPath).arg(destinationPath);
            SleeperThread::msleep(1*200);
#else
            QFile(destinationPath).remove();
            QFile(originPath).copy(destinationPath);
#endif
            continue;
        }

        foreach(QString eachItem, QDir(originPath).entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)){

            if(eachItem == "folders.xml")
                continue;

            QString eachItemFullPath = originPath + eachItem;
            emit log(eachItem);

            QString absolutePath = QFileInfo(destinationPath).absolutePath();
            if(!QDir(absolutePath).exists()){
                qDebug() << "dir not exists. Creating " << absolutePath;
                QDir().mkpath(absolutePath);
            }

            if(!QFileInfo(eachItemFullPath).isDir()){
                QString destinationFile = destinationPath + eachItem;
#ifdef DRY_RUN
                qDebug() << QString("from %1\nto %2\n\n").arg(eachItemFullPath).arg(destinationFile);
                SleeperThread::msleep(1*200);
#else
                if(QFileInfo(destinationFile).exists()){
                    QFile(destinationFile).remove();
                }
                qDebug() << "eachItemFullPath " << eachItemFullPath << " destinationFile " << destinationFile;
                QFile file(eachItemFullPath);
                if(!file.copy(destinationFile))
                    qDebug() << file.error();
#endif
            }
            else {
                QString itemName = destinationPath + eachItem;
                if(eachId==Documents){
                    itemName = itemName.replace("Sankore Document", "OpenBoard Document");
                    if(QDir(itemName).exists())
                        UBDirectories::removeDir(itemName);
                }
#ifdef DRY_RUN
                qDebug() << QString("from %1\nto %2\n\n").arg(eachItemFullPath).arg(itemName);
                SleeperThread::msleep(1*200);
#else
                qDebug() << "copyFolder eachItemFullPath " << eachItemFullPath << " itemName " << itemName;
                UBDirectories::copyFolder(eachItemFullPath,itemName);
#endif
            }
        }
    }

    emit finished();
}

UBUpgradeToOefDelegate::UBUpgradeToOefDelegate(QObject *parent) :
    QObject(parent)
{
}

int UBUpgradeToOefDelegate::getNumberOfItemToImport(PossiblesImport id)
{
    if(id==UniboardUserConfig)
        return QFile(UBDirectories::basicPath(id) + QDir::separator()+ "Sankore" + QDir::separator() + UBDirectories::subPathFor(id)).exists() ? 1 : 0;

    QString path = UBDirectories::basicPath(id) + QDir::separator() + "Sankore" + QDir::separator() + UBDirectories::subPathFor(id);
    QDir documentDir = QDir(path);

    return documentDir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot).count();
}

void UBUpgradeToOefDelegate::initImportThread()
{
    mThread = new QThread();
    mWorker = new Renamer();
    mWorker->moveToThread(mThread);
    connect(mThread, SIGNAL(started()), mWorker, SLOT(process()));
    connect(mWorker, SIGNAL(finished()), mThread, SLOT(quit()));
    connect(mWorker, SIGNAL(finished()), mWorker, SLOT(deleteLater()));
    connect(mThread, SIGNAL(finished()), mThread, SLOT(deleteLater()));
}


void UBUpgradeToOefDelegate::initSizeCalculationThread()
{
    mCalculatorThread = new QThread();
    mCalculator = new SizeCalculator();
    mCalculator->moveToThread(mCalculatorThread);
    connect(mCalculatorThread, SIGNAL(started()), mCalculator, SLOT(process()));
    connect(mCalculator, SIGNAL(finished()), mCalculatorThread, SLOT(quit()));
    connect(mCalculator, SIGNAL(finished()), mCalculator, SLOT(deleteLater()));
    connect(mCalculatorThread, SIGNAL(finished()), mCalculatorThread, SLOT(deleteLater()));
}
