#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DialogOptions.h"
#include "ClassEnex.h"
#include "EditWindow.h"
#include <QCloseEvent>
#include <QFileDialog>
#include <QSettings>
#include <QSplitter>

MainWindow::MainWindow(ClassWorkspace &wks)
    : QMainWindow(nullptr)
    , ui(new Ui::MainWindow)
    , m_wks(wks)
{
    ui->setupUi(this);

    connect(ui->actionImport, SIGNAL(triggered()), this, SLOT(onImport()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionOptions, SIGNAL(triggered()), this, SLOT(onOptions()));
    connect(ui->actionOpenInNewWindow, SIGNAL(triggered()), this, SLOT(openInExternalWindow()));

    ui->tableView->setupModel();

    // Testing the main view
    connect(ui->actionViewHorizontal, &QAction::triggered, this, [=]() { ui->splitter->setOrientation(Qt::Horizontal); });
    connect(ui->actionViewVertical, &QAction::triggered, this, [=]() { ui->splitter->setOrientation(Qt::Vertical); });

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
        foreach (auto note, enex.getNotes())
            m_wks.addNote(note);
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

void MainWindow::openInExternalWindow(QString guid)
{
    if (!m_editWindows.contains(guid))
    {
        EditWindow *w = new EditWindow(this);
        w->show();
        m_editWindows[guid] = w;
    }
    else
        static_cast<QMainWindow *>(m_editWindows[guid])->activateWindow();
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
