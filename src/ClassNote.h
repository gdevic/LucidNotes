#pragma once

#include "CTextDocument.h"
#include <QDateTime>
#include <QFile>
#include <QXmlStreamReader>

/*
 * This class contains a note text, all meta information for the note and operations on that note.
 * Normally, such meta data is stored in the database, but this class is also a container for a note
 * when it is being imported or exported.
 */
class ClassNote : public QObject
{
    Q_OBJECT
public:
    explicit ClassNote(QObject *parent = nullptr);
    QString guid() { return m_uuid8; }
    QString title() { return m_title; }
    QString summary() { return m_summary; }
    QString author() { return m_author; }
    QDateTime &created() { return m_created; }
    QDateTime &updated() { return m_updated; }

    bool readNote(QXmlStreamReader &xml);
    bool readSection(QXmlStreamReader &xml, QString sectionName);
    bool saveBlob(QString blobPath) { return saveBlob(blobPath + "/" + m_uuid8, false, false, ""); }
    void deleteBlob(QString blobPath) { QFile::remove(blobPath + "/" + m_uuid8); }

private:
    QString s;                  // XXX Temp string read by readSection()
    bool loadBlob(QString blobFileName, bool compress, bool encrypt, QString key);
    bool saveBlob(QString blobFileName, bool compress, bool encrypt, QString key);

private:
    QString m_uuid8;     // GUID, a universally unique ID assigned to this note
    CTextDocument m_doc; // Document container for HTML notes
    QString m_title;     // Document title (duplicated in m_doc.metaInformation)
    QString m_summary;   // Short note summary, text only
    QString m_author;    // Author of the note (short name or an email)
    QDateTime m_created; // Date/time a note has been created
    QDateTime m_updated; // Date/time a note has been last updated (text/title or other meta changed)
};
