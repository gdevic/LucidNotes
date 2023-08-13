#pragma once

#include "CTextDocument.h"
#include "Utils.h"
#include <QDateTime>
#include <QFile>
#include <QXmlStreamReader>

class ClassDatabase;

/*
 * This class contains a note text, all meta information for the note and operations on that note.
 * Normally, such meta data is stored in the database, but this class is also a container for a note
 * when it is being edited, imported or exported.
 */
class ClassNote : public CTextDocument
{
    Q_OBJECT
public:
    ClassNote(QString guid = getUUID8());
    QString guid() { return m_uuid8; }
    QString title() { return m_title; }
    QString summary() { return m_summary; }
    QString author() { return m_author; }
    QString notebook() { return m_notebook; }
    int notebookId() { return m_notebookId; }
    QDateTime &created() { return m_created; }
    QDateTime &updated() { return m_updated; }

    void setNotebook(const QString notebook) { m_notebook = notebook; }
    void setNotebookId(const int notebookId) { m_notebookId = notebookId; }

    bool readNote(QXmlStreamReader &xml);
    bool readSection(QXmlStreamReader &xml, QString sectionName);
    bool loadBlob(QString blobPath) { return loadBlob(blobPath + "/" + m_uuid8, false, false, ""); }
    bool saveBlob(QString blobPath) { return saveBlob(blobPath + "/" + m_uuid8, false, false, ""); }
    void deleteBlob(QString blobPath) { QFile::remove(blobPath + "/" + m_uuid8); }
    bool updateDatabase(ClassDatabase *db);

private:
    QString s;                  // XXX Temp string read by readSection()
    bool loadBlob(QString blobFileName, bool compress, bool encrypt, QString key);
    bool saveBlob(QString blobFileName, bool compress, bool encrypt, QString key);

private:
    QString m_uuid8;     // GUID, a universally unique ID assigned to this note
    QString m_title;     // Document title
    QString m_summary;   // Short note summary, text only
    QString m_author;    // Author of the note (short name or an email)
    QString m_notebook;  // Notebook name
    int m_notebookId;    // Notebook database ID
    QDateTime m_created; // Date/time a note has been created
    QDateTime m_updated; // Date/time a note has been last updated (text/title or other meta changed)
};
