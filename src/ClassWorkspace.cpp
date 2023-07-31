#include "ClassDatabase.h"
#include "ClassNote.h"
#include "ClassWorkspace.h"
#include <QMessageBox>

ClassWorkspace::ClassWorkspace(QString wksDir)
    : QObject{}
    , m_wksDir(wksDir)
    , m_wksDataDir(wksDir + "/Data")
    , m_lockFile(wksDir + "/.lock")
{
    // Create the workspace Data folder if it does not exist
    QDir dir; dir.mkpath(m_wksDataDir);

    m_lockFile.setStaleLockTime(0);
}

bool ClassWorkspace::init()
{
#ifndef QT_DEBUG // Do not use locks when we are debugging
    if (!m_lockFile.tryLock(100))
    {
        QMessageBox::critical(nullptr, "Notes",
            "This workspace is already in use.\n\n" \
            "If that is incorrect, remove the lock file:\n" + m_lockFile.fileName());
        return false;
    }
#endif

    ClassDatabase db;
    QString ret = db.open("workspace");
    if (!ret.isEmpty())
    {
        QMessageBox::critical(nullptr, "Notes",
            "Unable to initialize local SQLite database:\n" + ret +
            "\nPlease make sure this application is installed correctly.\n");
        return false;
    }

    // Initialize database with the tables used by the workspace class

    static const QStringList commands = {
        {"CREATE TABLE IF NOT EXISTS notebook_attr "
         "(id INTEGER PRIMARY KEY NOT NULL, "
         "name TEXT NOT NULL COLLATE NOCASE, "
         "stack TEXT NOT NULL COLLATE NOCASE, "
         "flags INTEGER, "
         "note_count INTEGER);"},

        {"INSERT OR REPLACE INTO notebook_attr(id, name, stack, flags) "
         "VALUES (0, 'Inbox', '.', 1);"},        // Inbox folder is the default folder with the ID of 0

        {"CREATE TABLE IF NOT EXISTS note_attr "
         "(id INTEGER PRIMARY KEY NOT NULL, "    // Note ID, locally unique to the database
         "guid TEXT NOT NULL, "                  // GUID universally unique note ID
         "title TEXT NOT NULL COLLATE NOCASE, "  // Title string
         "summary TEXT COLLATE NOCASE, "         // Note short summary text, normally taken from the note start
         "author TEXT COLLATE NOCASE, "          // Author name or email
         "notebook_id INTEGER NOT NULL, "        // ID of the notebook folder this note is shown
         "date_created REAL, "                   // Date and time the note was created
         "date_updated REAL, "                   // Date and time the note was last edited/changed
         "flags INTEGER);"}                      // Flags bitmap
    };
    if (!db.queryExec(commands))
    {
        QMessageBox::critical(nullptr, "Notes", "Unable to initialize database defaults.\n\n" + db.getLastSqlError());
        return false;
    }
    return true;
}

/*
 * Add a new note to the workspace (file blob + database)
 */
bool ClassWorkspace::addNote(ClassNote *note)
{
    qInfo() << "Workspace adding note:" << note->guid();

    // Tell the note to save its document as a file data blob (the text of the note, which we do not put in the database)
    if (note->saveBlob(m_wksDataDir))
    {
        ClassDatabase db;
        QString ret = db.open("addNote");
        Q_ASSERT_X(ret.isEmpty(), __FUNCTION__, ret.toStdString().c_str());

        static const QString command = "INSERT INTO note_attr(guid, title, summary, author, notebook_id, date_created, date_updated, flags)"
                                       "VALUES(?, ?, ?, ?, ?, ?, ?, ?)";
        QStringList binds;
        binds << note->guid();
        binds << note->title();
        binds << note->summary();
        binds << note->author();
        binds << "0";
        binds << note->created().toString(Qt::ISODate);
        binds << note->updated().toString(Qt::ISODate);
        binds << "0";
        int lastID = db.queryExec(command, binds);
        qInfo() << "lastID" << lastID;
        if (lastID)
        {
            qInfo() << "New note ID:" << lastID;
            return true;
        }
        else
            qWarning() << "Unable to add a note" << db.getLastSqlError();

        // Since adding to the database did not suceed, remove the file data blob
        note->deleteBlob(m_wksDataDir);
    }
    else
        qWarning() << "Unable to save note blob";
    return false;
}

/*
 * Open a note with the specified guid to be edited. It creates a note class in memory and sends it to either
 * the main editor or the extra windowed editor which is used when the user double-clicks on a note in the table.
 */
void ClassWorkspace::onNoteOpen(QString guid, bool useMainEditor)
{
    qInfo() << "onNoteOpen" << useMainEditor << guid;
    // Find if that note is already instantiated or we need to create and load it
    if (!m_notes.contains(guid))
    {
        ClassNote *note = new ClassNote(guid);
        note->loadBlob(m_wksDataDir);
        m_notes[guid] = note;
    }

    if (useMainEditor)
        emit mainEditorLoadNote(*m_notes[guid]);
    else
        emit auxEditorLoadNote(*m_notes[guid]);

    QSettings settings;
    settings.setValue("lruNote", guid);
}
