#pragma once

#include <QObject>
#include <QMap>
#include <QLockFile>

class ClassNote;

/*
 * This class contains the current workspace; the collection of notebooks and operations on it.
 */
class ClassWorkspace : public QObject
{
    Q_OBJECT
public:
    ClassWorkspace(QString wksDir);
    bool init();
    bool addNote(ClassNote *note);
    const QString getDataDir() { return m_wksDataDir; }

signals:
    void mainEditorLoadNote(ClassNote &note);
    void auxEditorLoadNote(ClassNote &note);

public slots:
    void onNoteOpen(QString guid, bool useMainEditor = true);

private:
    QString m_wksDir; // Workspace root directory
    QString m_wksDataDir; // Workspace data directory (for document blobs)
    QLockFile m_lockFile; // Workspace lock file (in the workspace root directory)

    QMap<QString, ClassNote *> m_notes; // A list of instantiated notes
};
