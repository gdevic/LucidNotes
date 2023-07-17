#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DialogOptions.h"
#include <QCloseEvent>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionOptions, SIGNAL(triggered()), this, SLOT(onOptions()));

    readSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::onOptions()
{
    DialogOptions dlg(this);
    dlg.exec();
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    const auto geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
        setGeometry(200, 200, 400, 400);
    else
        restoreGeometry(geometry);
    settings.endGroup();
}
