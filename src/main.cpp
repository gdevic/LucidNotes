#include "MainWindow.h"

#include <QApplication>
#include <QSettings>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    int ret = -1;
    QApplication a(argc, argv);

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
