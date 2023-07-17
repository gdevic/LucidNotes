#include "MainWindow.h"
#include <QApplication>
#include <QDir>
#include <QLockFile>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    int ret = -1;
    QApplication a(argc, argv);

    // Ensure that only one instance of this application is running
    QLockFile lockFile(QDir::temp().absoluteFilePath("notes.lock"));
    if (!lockFile.tryLock(100))
        return QMessageBox::warning(nullptr, "Notes",
        "The second instance of this application is already running!\n\n" \
        "If that is incorrect, remove the lock file:\n" + lockFile.fileName());

    // Initialize core application attributes
    // Settings are stored in "HKEY_CURRENT_USER\Software\Baltazar Studios, LLC\Notes"
    QCoreApplication::setOrganizationDomain("baltazarstudios.com");
    QCoreApplication::setOrganizationName("Baltazar Studios, LLC");
    QCoreApplication::setApplicationName("Notes");

    QSettings settings;

    // Initialize workspace folder:
    // - If it is empty, use the default OS AppData path
    // - If there is a new value ("armed value"), switch to it
    QString armedDir = settings.value("workspaceDirNext").toString();
    QString wks = settings.value("workspaceDir").toString();
    if (wks.isEmpty())
        settings.setValue("workspaceDir", QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    else if (!armedDir.isEmpty())
    {
        settings.setValue("workspaceDir", armedDir);
        settings.remove("workspaceDirNext");
    }
    qInfo() << "Using workspace" << settings.value("workspaceDir");

    MainWindow w;
    w.show();
    ret = a.exec();

    return ret;
}
