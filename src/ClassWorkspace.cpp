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
//    if (!m_lockFile.tryLock(100))
//    {
//        QMessageBox::critical(nullptr, "Notes",
//            "This workspace is already in use.\n\n" \
//            "If that is incorrect, remove the lock file:\n" + m_lockFile.fileName());
//        return false;
//    }

    if (!m_db.init(m_wksDir + "/notes.db"))
    {
        QMessageBox::critical(nullptr, "Notes",
            "Unable to initialize local SQLite database.\n\n" \
            "Please make sure this application is installed correctly.\n");
        return false;
    }

    return true;
}
