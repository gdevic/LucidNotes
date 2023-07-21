#ifndef CLASSWORKSPACE_H
#define CLASSWORKSPACE_H

#include "ClassDatabase.h"
#include <QLockFile>

/*
 * This class contains the current workspace; the collection of notebooks and operations on it.
 */
class ClassWorkspace : public QObject
{
    Q_OBJECT
public:
    ClassWorkspace(QString wksDir);
    bool init();

signals:

private:
    QString m_wksDir; // Workspace root directory
    QLockFile m_lockFile; // Workspace lock file (in the workspace root directory)
    ClassDatabase m_db; // SQLite database connection supporting this workspace
};

#endif // CLASSWORKSPACE_H
