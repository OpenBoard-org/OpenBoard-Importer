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


#ifndef UBDIRECTORIES_H
#define UBDIRECTORIES_H

#include <QMap>
#include <QObject>

enum PossiblesImport {Documents, AnimationUserDirectory, Applications, InteractiveContents, InteractiveFavorites, Library, UniboardUserConfig, USER_PATHS_COUNTER, Pictures, Movies, Audios, MAX_POSSIBLES_IMPORT};


class UBDirectories : public QObject
{
    Q_OBJECT
public:
    explicit UBDirectories(QObject *parent = 0);

    static QString subPathFor(PossiblesImport id);

    static QString basicPath(PossiblesImport id);

    static QString homePath();

    static bool removeDir(const QString & dirName);

    static void copyFolder(QString sourceFolder, QString destFolder);

    static bool deleteDir(const QString& pDirPath);

    static qint64 sizeOfDirectory(QString path);

};

#endif // UBDIRECTORIES_H
