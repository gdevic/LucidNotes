#include "ClassNote.h"
#include "ClassDatabase.h"
#include "aes256.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QStringBuilder>

ClassNote::ClassNote(QString guid)
    : CTextDocument(nullptr)
    , m_uuid8(guid)
    , m_title("Untitled")
    , m_author("Anonymous")
{
}

/*
 * Loads the note content from a file blob.
 * The note can be encrypted and/or compressed.
 */
bool ClassNote::loadBlob(QString blobFileName, bool compress, bool encrypt, QString key)
{
    qInfo() << "loadFromBlob" << blobFileName << "compress:" << compress << "encrypt:" << encrypt << "key:" << key;

    QFile file(blobFileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);

        ByteArray plain;

        size_t size = file.size();
        QByteArray enc(size, 0);
        QByteArray ba;

        stream.readRawData(enc.data(), (int)size);

        if (encrypt)
        {
            Aes256::decrypt(str2ba(key), (const unsigned char*)enc.data(), enc.length(), plain);
            ba = QByteArray((const char*)plain.data(), plain.size());
        }
        else
            ba = QByteArray(enc);

        QString html = compress ? qUncompress(ba) : ba;

        // qUncompress Returns an empty QByteArray if the input data was corrupt.
        if (compress && html.isEmpty())
        {
            qWarning() << "loadFromBlob could not decompress the blob (corrupted file or invalid key)";
            return false;
        }
        setHtml(html);
        m_title = metaInformation(QTextDocument::DocumentTitle);
        return true;
    }
    return false;
}

/*
 * Saves the note content to a file blob.
 * The file can be encrypted and/or compressed.
 */
bool ClassNote::saveBlob(QString blobFileName, bool compress, bool encrypt, QString key)
{
    qInfo() << "saveToBlob" << blobFileName << "compress:" << compress << "encrypt:" << encrypt << "key:" << key;

    setMetaInformation(QTextDocument::DocumentTitle, m_title);
    QString html = toHtml();
    QByteArray ba = html.toUtf8();
    QByteArray cba = compress ? qCompress(ba, 9) : ba;

    QFile file(blobFileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&file);

        if (encrypt)
        {
            ByteArray enc;
            Aes256::encrypt(str2ba(key), (const unsigned char*)cba.data(), cba.length(), enc);
            stream.writeRawData((char *)(enc.data()), int(enc.size()));
        }
        else
            stream.writeRawData(cba.data(), cba.length());
        return true;
    }
    return false;
}

/*
 * Imports a note in ENEX format from the XML stream
 */
bool ClassNote::readNote(QXmlStreamReader &xml)
{
    QSettings settings;

    while (!xml.atEnd())
    {
        xml.readNext();

//        qInfo() << "TYPE:" << ClassEnex::tokenType(xml);
//        qInfo() << "NAME:" << xml.name() << (xml.isStartElement() ? "START" : (xml.isEndElement() ? "END" : ""));
//        qInfo() << "TEXT:" << xml.text();

        if (xml.isStartElement())
        {
            QApplication::processEvents(); // Called from ENEX note import, in the UI thread, give some time slices as we are importing

            if ((xml.name().toString() == "title"))
            {
                if (readSection(xml, "title") == false)
                    return false;
                setMetaInformation(QTextDocument::DocumentTitle, s);
                m_title = s;
                qInfo() << "Title:" << s;
            }

            if (xml.name().toString() == "content")
            {
                if (readSection(xml, "content") == false)
                    return false;

                // Set the app default font and size for all imported elements that do not have it
                QFont font = settings.value("noteFont").value<QFont>();
                font.setPointSize(settings.value("noteFontPointSize").toInt());
                setDefaultFont(font);

                setHtml(s);
                m_summary = toPlainText().left(200).replace('\n', ' ');
            }

            if (xml.name().toString() == "created")
            {
                if (readSection(xml, "created") == false)
                    return false;
                // From "20230722T052819Z" to ISO_8601: "2023-07-22T05:28:19Z"
                QString dts = s.left(4) % '-' % s.mid(4,2) % '-' % s.mid(6,5) % ':' % s.mid(11,2) % ':' %s.mid(13,3);
                m_created = QDateTime::fromString(dts, Qt::ISODate);
            }

            if (xml.name().toString() == "updated")
            {
                if (readSection(xml, "updated") == false)
                    return false;
                // From "20230722T052819Z" to ISO_8601: "2023-07-22T05:28:19Z"
                QString dts = s.left(4) % '-' % s.mid(4,2) % '-' % s.mid(6,5) % ':' % s.mid(11,2) % ':' %s.mid(13,3);
                m_updated = QDateTime::fromString(dts, Qt::ISODate);
            }

            if (xml.name().toString() == "author")
            {
                if (readSection(xml, "author") == false)
                    return false;
                m_author = s;
            }
        }
        if ((xml.name().toString() == "note") && xml.isEndElement())
            return true;
    }
    return false;
}

bool ClassNote::readSection(QXmlStreamReader &xml, QString sectionName)
{
    xml.readNext();
    s = xml.text().toString();

    xml.readNext();
    if (xml.name().toString() != sectionName || !xml.isEndElement())
    {
        qCritical() << "readSection fails for" << sectionName;
        return false;
    }
    return true;
}

/*
 * Update local database with the data from this note record
 */
bool ClassNote::updateDatabase(ClassDatabase *db)
{
    // XXX If the note already exists, we need to fully delete it, including its file blob, attachments etc.
    //     The existing note needs to be fully purged. How about delete history of that note?

    static const QString command = "INSERT INTO note_attr(guid, title, summary, author, notebook, notebook_id, date_created, date_updated)"
                                   "VALUES(?, ?, ?, ?, ?, ?, ?, ?)";
    QStringList binds;
    binds << guid();
    binds << title();
    binds << summary();
    binds << author();
    binds << notebook();
    binds << QString::number(notebookId());
    binds << created().toString(Qt::ISODate);
    binds << updated().toString(Qt::ISODate);
    int lastID = db->queryExec(command, binds);
    qInfo() << "lastID" << lastID;
    if (lastID)
    {
        qInfo() << "New note ID:" << lastID;
        return true;
    }
    else
        qWarning() << "Unable to add a note" << db->getLastSqlError();

    return true;
}

