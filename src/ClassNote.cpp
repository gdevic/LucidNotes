#include "ClassNote.h"
#include "ClassUtils.h"
#include "ClassEnex.h"
#include "aes256.h"
#include <QFile>
#include <QDir>

ClassNote::ClassNote(QObject *parent)
    : QObject(parent)
    , m_uuid8(CUtils::getUUID8())
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
            Aes256::decrypt(CUtils::str2ba(key), (const unsigned char*)enc.data(), enc.length(), plain);
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
        m_doc.setHtml(html);
        //m_title = (m_doc->metaInformation(QTextDocument::DocumentTitle));
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

    //m_doc->setMetaInformation(QTextDocument::DocumentTitle, m_title);
    QString html = m_doc.toHtml();
    QByteArray ba = html.toUtf8();
    QByteArray cba = compress ? qCompress(ba, 9) : ba;

    QFile file(blobFileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&file);

        if (encrypt)
        {
            ByteArray enc;
            Aes256::encrypt(CUtils::str2ba(key), (const unsigned char*)cba.data(), cba.length(), enc);
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
    while (!xml.atEnd())
    {
        xml.readNext();

        qInfo() << "TYPE:" << ClassEnex::tokenType(xml);
        qInfo() << "NAME:" << xml.name() << (xml.isStartElement() ? "START" : (xml.isEndElement() ? "END" : ""));
        qInfo() << "TEXT:" << xml.text();

        if (xml.isStartElement())
        {
            if ((xml.name().toString() == "title"))
            {
                if (readSection(xml, "title") == false)
                    return false;
                m_doc.setMetaInformation(QTextDocument::DocumentTitle, s);
                qInfo() << "Title:" << s;
            }

            if (xml.name().toString() == "content")
            {
                if (readSection(xml, "content") == false)
                    return false;
                m_doc.setHtml(s);
            }

            if (xml.name().toString() == "created")
            {

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
