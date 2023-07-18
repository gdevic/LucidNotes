#ifndef CLASSNOTE_H
#define CLASSNOTE_H

#include <QObject>

class CTextDocument;

/*
 * This class contains a note text, all meta information for the note and operations on that note.
 */
class ClassNote : public QObject
{
    Q_OBJECT
public:
    explicit ClassNote(QObject *parent = nullptr);

    // XXX To be private:
    bool loadBlob(QString blobFileName, bool compress, bool encrypt, QString key);
    bool saveBlob(QString blobFileName, bool compress, bool encrypt, QString key);

signals:

private:
    QString m_uuid8;      // GUID-based file name associated with this document
    CTextDocument *m_doc; // Document container for HTML notes
};

#endif // CLASSNOTE_H
