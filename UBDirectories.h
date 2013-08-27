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
