#include "ClassWorkspace.h"
#include <QMessageBox>

ClassWorkspace::ClassWorkspace(QString wksDir)
    : QObject{}
    , m_wksDir(wksDir)
    , m_lockFile(wksDir + "/.lock")
    , m_db(this)
{
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

    if (!m_db.open("workspace"))
        return false;

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
         "flags INTEGER);"}                      // Flags bitmap
    };
    if (!m_db.queryExec(commands))
    {
        QMessageBox::critical(nullptr, "Notes", "Unable to initialize database defaults.\n\n" + m_db.getLastSqlError());
        return false;
    }

    return true;
}
