#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DialogOptions.h"
#include "ClassEnex.h"
#include <QCloseEvent>
#include <QFileDialog>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionImport, SIGNAL(triggered()), this, SLOT(onImport()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionOptions, SIGNAL(triggered()), this, SLOT(onOptions()));

    readSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onImport()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Import ENEX Notes", "", "Enex notes (*.enex);;All files (*.*)");
    if(!fileName.isNull())
    {
        ClassEnex enex;
        enex.import(fileName);
    }
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
    settings.setValue("mainWindowGeometry", saveGeometry());
}

void MainWindow::readSettings()
{
    QSettings settings;
    const auto geometry = settings.value("mainWindowGeometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
        setGeometry(200, 200, 400, 400);
    else
        restoreGeometry(geometry);
}
