#include "MainWindow.h"
#include "ClassWorkspace.h"
#include "DialogActivityLog.h"
#include "Utils.h"
#include <QApplication>
#include <QSettings>
#include <QTime>

/*
 * Root handler for our debug/info messages and the activity log class that handles messages once the MainWindow creates it
 */
DialogActivityLog *activityLog = nullptr;
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    static const QString typeString[5] = { "[Debug]", "[Warning]", "[Critical]", "[Fatal]", "[Info]" }; // QtMsgType
    static QStringList log;
    QString line = QString("%1 %2 %3").arg(QTime::currentTime().toString("HH:mm:ss"), typeString[type], msg);
    log.append(line);

    // Echo every log line to stderr for now
    fprintf(stderr, line.toLocal8Bit());
    fprintf(stderr, "\n");
    fflush(stdout);

    // Call the Activity Log window once it has been created
    if (activityLog)
        activityLog->messageHandler(log); // This function will also empty the log
}

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

    if (!settings.contains("userName"))           settings.setValue("userName", getUserName());
#ifdef QT_DEBUG
    if (!settings.contains("workspaceDir"))       settings.setValue("workspaceDir", "R:");
#else
    if (!settings.contains("workspaceDir"))       settings.setValue("workspaceDir", QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
#endif
    if (!settings.contains("titleFont"))          settings.setValue("titleFont", QApplication::font());
    if (!settings.contains("noteFont"))           settings.setValue("noteFont", QApplication::font());
    if (!settings.contains("titleFontPointSize")) settings.setValue("titleFontPointSize", QString::number(QApplication::font().pointSize() + 2));
    if (!settings.contains("noteFontPointSize"))  settings.setValue("noteFontPointSize", QString::number(QApplication::font().pointSize()));
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageHandler);

    QApplication a(argc, argv);
    initAppDefaults();

    QSettings settings;
    int ret = -1;
    do
    {
        settings.remove("restart");

        // Initialize workspace folder and, if there is a new value ("armed value"), switch to it
        if (settings.contains("workspaceDirNext"))
        {
            // XXX Do we want to copy the workspace files over to a new directory?
            settings.setValue("workspaceDir", settings.value("workspaceDirNext"));
            settings.remove("workspaceDirNext");
        }
        settings.sync();
        qInfo() << "Using workspace" << settings.value("workspaceDir");

        ClassWorkspace wks(settings.value("workspaceDir").toString());
        if (wks.init())
        {
            MainWindow w(wks);
            w.show();
            ret = a.exec();
        }
    }
    // If the settings contain the key "restart", restart the application
    while (settings.contains("restart"));

    return ret;
}
