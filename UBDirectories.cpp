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


#include <QDesktopServices>

#include "UBDirectories.h"
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QStack>
#include <QDebug>

UBDirectories::UBDirectories(QObject *parent) :
    QObject(parent)
{
}

QString UBDirectories::subPathFor(PossiblesImport id)
{
    switch (id) {
    case Documents:
        return "document" + QString(QDir::separator());
    case AnimationUserDirectory:
        return "animationUserDirectory" + QString(QDir::separator());
    case Applications:
        return "application" + QString(QDir::separator());
    case InteractiveContents:
        return "interactive content" + QString(QDir::separator());
    case InteractiveFavorites:
        return "interactive favorites" + QString(QDir::separator());
    case Library:
        return "library" + QString(QDir::separator());
    case UniboardUserConfig:
        return "UniboardUser.config";
    default:
        return "";
    }
}


QString UBDirectories::basicPath(PossiblesImport id)
{
    if(id == Audios)
        return QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    if(id == Movies)
        return QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    if(id == Pictures)
        return QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);

    QString path;
#ifdef Q_OS_WIN
    path = QStandardPaths::writableLocation(QStandardPaths::DataLocation).replace("\\" + qApp->organizationName() + "\\"+ qApp->applicationName(),"");
#else
    path = QStandardPaths::writableLocation(QStandardPaths::DataLocation).replace("/" + qApp->organizationName() + "/"+ qApp->applicationName(),"");
#endif
    return path;
}


QString UBDirectories::homePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
}


bool UBDirectories::removeDir(const QString & dirName)
{
    bool result = false;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }
    return result;
}

void UBDirectories::copyFolder(QString sourceFolder, QString destFolder)
{
    QDir sourceDir(sourceFolder);
    if(!sourceDir.exists())
        return;
    QDir destDir(destFolder);

    if(!destDir.exists())
        destDir.mkpath(destFolder);

	QStringList files = sourceDir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

    for(int i = 0; i < files.count(); i++)
    {
		QString srcName = sourceFolder + QDir::separator() + files[i];
        QString destName = destFolder + QDir::separator() + files[i];
		if(QFileInfo(srcName).isDir())
			copyFolder(srcName, destName);
		else{
			if(!QFile::copy(srcName, destName))
				copyFolder(srcName, destName);
		}
    }
}

bool UBDirectories::deleteDir(const QString& pDirPath)
{
    if (pDirPath == "" || pDirPath == "." || pDirPath == "..")
        return false;

    QDir dir(pDirPath);

    if (dir.exists())
    {
        foreach(QFileInfo dirContent, dir.entryInfoList(QDir::Files | QDir::Dirs
                | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System , QDir::Name))
        {
            if (dirContent.isDir())
            {
                deleteDir(dirContent.absoluteFilePath());
            }
            else
            {
                if (!dirContent.dir().remove(dirContent.fileName()))
                {
                    return false;
                }
            }
        }
    }

    return dir.rmdir(pDirPath);
}


qint64 UBDirectories::sizeOfDirectory(QString path)
{
    qint64 size = 0;

    QDir dir(path);
    QStack<QString> stack;
    stack.push(dir.absolutePath());
    while (!stack.isEmpty()) {
        QString sSubdir = stack.pop();
        QDir subdir(sSubdir);
        QFileInfoList infoList = subdir.entryInfoList(QDir::Files);
        for (int i = 0; i < infoList.size(); i+=1) {
            size += infoList.at(i).size();
        }

        QStringList entries = subdir.entryList(QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for (int i = 0; i < entries.size(); i+=1) {
            stack.push(subdir.path() + QDir::separator() + entries.at(i));
        }
    }

    return size;
}
