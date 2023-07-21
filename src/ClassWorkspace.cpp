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
         "VALUES (0, 'Inbox', '.', 1);"},

        {"CREATE TABLE IF NOT EXISTS note_attr "
         "(id INTEGER PRIMARY KEY NOT NULL, "
         "guid TEXT NOT NULL, "
         "title TEXT NOT NULL COLLATE NOCASE, "
         "author TEXT COLLATE NOCASE, "
         "notebook_id INTEGER NOT NULL, "
         "flags INTEGER);"}
    };
    if (!m_db.queryExec(commands))
    {
        QMessageBox::critical(nullptr, "Notes", "Unable to initialize database defaults.\n\n" + m_db.getLastSqlError());
        return false;
    }

    return true;
}
