#include "MainWindow.h"
#include <QApplication>
#include <QDir>
#include <QFontDatabase>
#include <QLockFile>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>

/*
 * For all important settings that do not exist, initializes default values.
 * We call this at the very start of the application run to guarantee the most important settings are valid.
 */
void initAppDefaults()
{
    // Initialize core application attributes
    // Settings are stored in "HKEY_CURRENT_USER\Software\Baltazar Studios, LLC\Notes"
    QCoreApplication::setOrganizationDomain("baltazarstudios.com");
    QCoreApplication::setOrganizationName("Baltazar Studios, LLC");
    QCoreApplication::setApplicationName("Notes");

    QSettings settings;

    if (!settings.contains("workspaceDir"))       settings.setValue("workspaceDir", QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

    if (!settings.contains("titleFont"))          settings.setValue("titleFont", QApplication::font());
    if (!settings.contains("noteFont"))           settings.setValue("noteFont", QApplication::font());
    if (!settings.contains("titleFontPointSize")) settings.setValue("titleFontPointSize", QString::number(QApplication::font().pointSize() + 2));
    if (!settings.contains("noteFontPointSize"))  settings.setValue("noteFontPointSize", QString::number(QApplication::font().pointSize()));
}

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

    initAppDefaults();

    QSettings settings;
    // Initialize workspace folder and, if there is a new value ("armed value"), switch to it
    if (settings.contains("workspaceDirNext"))
    {
        // XXX Do we want to copy the workspace files over to the new directory?
        settings.setValue("workspaceDir", settings.value("workspaceDirNext"));
        settings.remove("workspaceDirNext");
    }
    settings.sync();
    qInfo() << "Using workspace" << settings.value("workspaceDir");

    MainWindow w;
    w.show();
    ret = a.exec();

    return ret;
}
