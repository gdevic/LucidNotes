#include "MainWindow.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    int ret = -1;
    QApplication a(argc, argv);

    // Initialize core application attributes
    // Settings are stored in "HKEY_CURRENT_USER\Software\Baltazar Studios, LLC\Notes"
    QCoreApplication::setOrganizationDomain("baltazarstudios.com");
    QCoreApplication::setOrganizationName("Baltazar Studios, LLC");
    QCoreApplication::setApplicationName("Notes");

    MainWindow w;
    w.show();
    ret = a.exec();

    return ret;
}
