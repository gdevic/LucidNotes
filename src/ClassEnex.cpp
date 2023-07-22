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

bool ClassEnex::import(QString fileName)
{
    qInfo() << "Importing from:" << fileName;

    QFile inFile(fileName);

    if (inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QXmlStreamReader xml(&inFile);

        while (!xml.atEnd())
        {
            xml.readNext();

            qInfo() << "TYPE:" << tokenType(xml);
//            qInfo() << "NAME:" << xml.name() << (xml.isStartElement() ? "START" : (xml.isEndElement() ? "END" : ""));
//            qInfo() << "TEXT:" << xml.text();

            if ((xml.name().toString() == "en-export") && xml.isStartElement())
            {
                if (readExport(xml) == false)
                    return false;
            }
        }
        qInfo() << "Imported" << m_notes.count() << "notes.";
        return true;
    }
    else
        qCritical() << "Unable to open XML file" << fileName << inFile.error() << inFile.errorString();
    return false;
}

// Reads the export section and every note that is defined within it
bool ClassEnex::readExport(QXmlStreamReader &xml)
{
    while (!xml.atEnd())
    {
        xml.readNext();

        if ((xml.name().toString() == "note") && xml.isStartElement())
        {
            ClassNote *note = new ClassNote();

            if (note->readNote(xml) == false) // Ignore a note that fails to load XXX can we contine reading XML?
                delete note;
            else
                m_notes.append(note);
        }
        if ((xml.name().toString() == "en-export") && xml.isEndElement())
            return true;
    }
    return false;
}

QString ClassEnex::tokenType(QXmlStreamReader &xml)
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
