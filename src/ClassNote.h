#ifndef CLASSNOTE_H
#define CLASSNOTE_H

#include "CTextDocument.h"
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
    QString author() { return "Anonymous"; }

    bool readNote(QXmlStreamReader &xml);
    bool readSection(QXmlStreamReader &xml, QString sectionName);
    bool saveBlob(QString blobPathName) { return saveBlob(blobPathName + "/" + m_uuid8, false, false, ""); }

private:
    QString s;                  // XXX Temp string read by readSection()
    bool loadBlob(QString blobFileName, bool compress, bool encrypt, QString key);
    bool saveBlob(QString blobFileName, bool compress, bool encrypt, QString key);

private:
    QString m_uuid8;     // GUID, a universally unique ID assigned to this note
    CTextDocument m_doc; // Document container for HTML notes
    QString m_title;     // Document title (duplicated in m_doc.metaInformation)
    QString m_summary;   // Short note summary, text only
};

#endif // CLASSNOTE_H
