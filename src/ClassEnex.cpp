#include "ClassDatabase.h"
#include "ClassEnex.h"
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

    QFile inFile(fileName);

    if (inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QXmlStreamReader xml(&inFile);

        while (!xml.atEnd())
        {
            xml.readNext();

//            qInfo() << "TYPE:" << tokenType(xml);
//            qInfo() << "NAME:" << xml.name() << (xml.isStartElement() ? "START" : (xml.isEndElement() ? "END" : ""));
//            qInfo() << "TEXT:" << xml.text();

            if ((xml.name().toString() == "en-export") && xml.isStartElement())
            {
                if (readExport(xml) == false)
                    return "Error reading a note";
            }
        }
        qInfo() << "Imported" << m_notes.count() << "notes.";
    }
    else
        return QString("Unable to open XML file %1 %2 %3").arg(fileName).arg(inFile.error()).arg(inFile.errorString());
    return QString();
}

QList<ClassNote *> ClassEnex::getDuplicateNotes()
{
    QList<ClassNote *> dups;

    ClassDatabase db;
    QString ret = db.open("getDuplicateNotes");
    Q_ASSERT_X(ret.isEmpty(), __FUNCTION__, ret.toStdString().c_str());
    if (!ret.isEmpty()) return dups;

    foreach(auto note, m_notes)
    {
        if (db.getNoteId(note->title(), note->created()) > 0)
            dups.append(note);
    }
    return dups;
}

/*
 * Save all notes to their file blobs
 */
bool ClassEnex::saveAll(const QString dataDir)
{
    foreach(auto note, m_notes)
    {
        if (note->saveBlob(dataDir) == false)
            return false;
    }
    return true;
}

/*
 * Update local database with the information from each note
 * Use extra information from EN SQLite database if available
 */
bool ClassEnex::updateDatabaseAll()
{
    ClassDatabase db;
    QString ret = db.open("addNotes");
    Q_ASSERT_X(ret.isEmpty(), __FUNCTION__, ret.toStdString().c_str());
    if (!ret.isEmpty()) return false;

    QString defaultNotebook = QString("Imported on %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    uint defaultNotebookId; // ID of the default notebook to import to
    bool defaultImportNotebookCreated = false;

    foreach(auto note, m_notes)
    {
        bool createDefaultImportNotebook = true;

        // If we have extended notebook information from the EN SQLite database, match a note with it
        int index = m_enDb.title.indexOf(note->title());
        if (index > 0)
        {
            uint uid = m_enDb.notebook_uid[index];
            if (m_enDb.notebookAttr.contains(uid))
            {
                // Create a local notebook/stack database entry (only once)
                NotebookAttrStruct &nbk = m_enDb.notebookAttr[uid];
                if (nbk.local_uid == 0) // Did we not create it already? No...
                {
                    // If not, does it already exist from before this import operation?
                    nbk.local_uid = db.getNotebookId(nbk.name);
                    if (nbk.local_uid == 0)
                    {
                        if (db.createNotebookAndStack(nbk.name, nbk.stack, nbk.local_uid) == false)
                        {
                            qCritical() << "Unable to create local notebook/stack database entry";
                            return false;
                        }
                    }
                }

                note->setNotebook(nbk.name);
                note->setNotebookId(nbk.local_uid);

                createDefaultImportNotebook = false;
            }
            else
                qWarning() << "Incorrect EN notebook uid" << index;
        }

        // If we did not match (and have created) a note's notebook, create a default one (only once)
        if (!defaultImportNotebookCreated && createDefaultImportNotebook)
        {
            // Create a new notebook to store this import note(s)
            if (db.createUniqueNotebook(defaultNotebook, defaultNotebookId) == false)
            {
                qCritical() << "Unable to create default import notebook";
                return false;
            }
            defaultImportNotebookCreated = true;
        }

        // If this was not set to true (above), this note does not have its Evernote notebook/stack
        if (createDefaultImportNotebook)
        {
            note->setNotebook(defaultNotebook);
            note->setNotebookId(defaultNotebookId);
        }

        if (note->updateDatabase(&db) == false)
            return false;
    }
    return true;
}

// Reads the export section and every note that is defined within it
bool ClassEnex::readExport(QXmlStreamReader &xml)
{
    while (!xml.atEnd()) // Loop until at the (unexpected!) end of the XML document
    {
        xml.readNext();

        if ((xml.name().toString() == "note") && xml.isStartElement())
        {
            ClassNote *note = new ClassNote();

            if (note->readNote(xml) == false) // Ignore a note that fails to load XXX can we contine reading XML?
                delete note;
            else
            {
                m_notes.append(note);
                emit xmlReadTitle(note->title()); // Used by the ENEX import to show some progress
            }
        }
        if ((xml.name().toString() == "en-export") && xml.isEndElement())
            return true;
    }
    return false;
}

/*
 * Read Evernote's SQLite database and populate internal (temporary) notebook and notes lists
 */
bool ClassEnex::readEnDatabase(const QString exbFileName)
{
    if (!QFile::exists(exbFileName))
        return false;
    ClassDatabase dbEn;
    QString ret = dbEn.open("evernote", exbFileName);
    Q_ASSERT_X(ret.isEmpty(), __FUNCTION__, ret.toStdString().c_str());
    if (!ret.isEmpty()) return false;

    QSqlQuery sq(dbEn.getDB());
    sq.setForwardOnly(true);
    sq.prepare("SELECT uid,name,stack FROM notebook_attr");
    sq.exec();
    while (sq.next())
    {
        int uid = sq.value(0).toUInt();
        NotebookAttrStruct nbk { .name = sq.value(1).toString(), .stack = sq.value(2).toString(), .local_uid = 0 };
        qInfo() << uid << nbk.name << nbk.stack;
        m_enDb.notebookAttr[uid] = nbk;
    }
    sq.finish();

    sq.prepare("SELECT title,notebook_uid,date_created FROM note_attr");
    sq.exec();
    while (sq.next())
    {
        qInfo() << sq.value(0).toString() << sq.value(1).toUInt();
        m_enDb.title.append(sq.value(0).toString());
        m_enDb.notebook_uid.append(sq.value(1).toUInt());
    }
    return true;
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
