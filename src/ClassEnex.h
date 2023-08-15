#pragma once

#include "ClassNote.h"
#include <QDateTime>
#include <QXmlStreamReader>

struct NotebookAttrStruct
{
    QString name;
    QString stack;
    uint local_uid; // Local notebook uid; zero for "need to create it"
};

struct EnDb
{
    // Parallel lists of note attributes
    QList<QString> title;
    QList<uint> notebook_uid;
    QList<QDateTime> date_created;
    // Notebook info mapped by EN notebook uid
    QHash<uint, NotebookAttrStruct> notebookAttr;
};

class ClassEnex : public QObject
{
    Q_OBJECT
public:
    explicit ClassEnex(QObject *parent = nullptr);
    ~ClassEnex();

    const QString import(const QString fileName);
    QList<ClassNote *> getDuplicateNotes();
    void removeNote(ClassNote *note) { m_notes.remove(m_notes.indexOf(note)); }
    bool readEnDatabase(const QString exbFileName);
    bool saveAll(const QString dataDir);
    bool updateDatabaseAll();

signals:
    void xmlReadTitle(QString title); // Sends the title of each note as they are being imported

private:
    QList<ClassNote *> m_notes; // List of exported notes
    EnDb m_enDb;                // Extra information read from Evernote's SQLite database

private:
    const QString tokenType(QXmlStreamReader &xml);
    bool readExport(QXmlStreamReader &xml);
};
