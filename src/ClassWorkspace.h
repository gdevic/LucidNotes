#ifndef CLASSWORKSPACE_H
#define CLASSWORKSPACE_H

#include <QObject>
#include <QLockFile>

/*
 * This class contains the current workspace; the collection of notebooks and operations on it.
 */
class ClassWorkspace : public QObject
{
    Q_OBJECT
public:
    ClassWorkspace(QString wksDir);
    bool tryInit();

signals:

private:
    QString m_wksDir; // Workspace root directory
    QLockFile m_lockFile; // Workspace lock file (in the workspace root directory)
};

#endif // CLASSWORKSPACE_H
