#include "ClassWorkspace.h"
#include <QMessageBox>

ClassWorkspace::ClassWorkspace(QString wksDir)
    : QObject{nullptr}
    , m_wksDir(wksDir)
    , m_lockFile(wksDir + "/.lock")
{
    m_lockFile.setStaleLockTime(0);
}

bool ClassWorkspace::tryInit()
{
    if (!m_lockFile.tryLock(100))
    {
        QMessageBox::critical(nullptr, "Notes",
            "This workspace is already in use.\n\n" \
            "If that is incorrect, remove the lock file:\n" + m_lockFile.fileName());
        return false;
    }
    return true;
}
