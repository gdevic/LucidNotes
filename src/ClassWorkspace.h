#ifndef CLASSWORKSPACE_H
#define CLASSWORKSPACE_H

#include "ClassNotebook.h"
#include "ClassRSync.h"

#include <QObject>

/*
 * This class contains the current workspace, its collection of notebooks and operations on the workspace.
 */
class ClassWorkspace : public QObject
{
    Q_OBJECT
public:
    explicit ClassWorkspace(QObject *parent = nullptr);

signals:

public slots:
    // Manage a workspace
    bool Create(const QString &path);
    bool Load(const QString &path);
    bool Save();

    // Manage remote sync mechanisms
    bool CreateRSync(const QString &name);

private:
    QList<ClassNotebook *> m_notebooks;
    QList<ClassRSync *> m_rsyncs;
};

#endif // CLASSWORKSPACE_H
