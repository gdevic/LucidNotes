#include "EditWindow.h"
#include "ui_EditWindow.h"
#include <QCloseEvent>
#include <QSettings>

EditWindow::EditWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);

    readSettings();
}

EditWindow::~EditWindow()
{
    delete ui;
}

void EditWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void EditWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("editWindowGeometry", saveGeometry());
}

void EditWindow::readSettings()
{
    QSettings settings;
    const auto geometry = settings.value("editWindowGeometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
        setGeometry(100, 100, 600, 800);
    else
        restoreGeometry(geometry);
}
