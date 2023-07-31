#include "ClassEnex.h"
#include "ClassNote.h"
#include <QDebug>
#include <QFile>

ClassEnex::ClassEnex(QObject *parent)
    : QObject{parent}
{
}

ClassEnex::~ClassEnex()
{
    qDeleteAll(m_notes);
}

/*
 * Reads a list of Evernote notes in ENEX format into the internal list structure
 * Returns an empty string on success or a text describing the failure
 */
const QString ClassEnex::import(const QString fileName)
{
    qInfo() << "Importing from:" << fileName;

    qDeleteAll(m_notes);
    m_notes.clear();
    is_cancelled = false;

    QFile inFile(fileName);

    if (inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QXmlStreamReader xml(&inFile);

        while (!xml.atEnd() && !is_cancelled)
        {
            xml.readNext();

            qInfo() << "TYPE:" << tokenType(xml);
//            qInfo() << "NAME:" << xml.name() << (xml.isStartElement() ? "START" : (xml.isEndElement() ? "END" : ""));
//            qInfo() << "TEXT:" << xml.text();

            if ((xml.name().toString() == "en-export") && xml.isStartElement())
            {
                if (readExport(xml) == false)
                    return is_cancelled ? "Note import canceled by user" : "Error reading a note";
            }
        }
        qInfo() << "Imported" << m_notes.count() << "notes.";
    }
    else
        return QString("Unable to open XML file %1 %2 %3").arg(fileName).arg(inFile.error()).arg(inFile.errorString());

    return is_cancelled ? "Note import canceled by user" : QString();
}

// Reads the export section and every note that is defined within it
bool ClassEnex::readExport(QXmlStreamReader &xml)
{
    while (!xml.atEnd() || is_cancelled) // Loop until cancelled or at the (unexpected!) end of the XML document
    {
        xml.readNext();

        if ((xml.name().toString() == "note") && xml.isStartElement())
        {
            ClassNote *note = new ClassNote();

            if (note->readNote(xml) == false) // Ignore a note that fails to load XXX can we contine reading XML?
                delete note;
            else
            {
                if (thID)
                    note->moveToThread(thID);
                m_notes.append(note);
            }
        }
        if ((xml.name().toString() == "en-export") && xml.isEndElement())
            return true;
    }
    return false;
}

const QString ClassEnex::tokenType(QXmlStreamReader &xml)
{
    switch (xml.tokenType())
    {
        case QXmlStreamReader::NoToken: return "No token";
        case QXmlStreamReader::Invalid: return "Invalid";
        case QXmlStreamReader::StartDocument: return "Start Document";
        case QXmlStreamReader::EndDocument: return "End Document";
        case QXmlStreamReader::StartElement: return "Start Element";
        case QXmlStreamReader::EndElement: return "End Element";
        case QXmlStreamReader::Characters: return "Characters";
        case QXmlStreamReader::DTD: return "DTD";
        case QXmlStreamReader::EntityReference: return "Entity Reference";
        case QXmlStreamReader::ProcessingInstruction: return "Processing Instruction";
        case QXmlStreamReader::Comment: return "Comment";
    }
    return "UNKNOWN";
}
