#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DialogOptions.h"
#include "DialogImportEnex.h"
#include "EditWindow.h"
#include <QCloseEvent>
#include <QFileDialog>
#include <QScreen>
#include <QSettings>
#include <QSplitter>

MainWindow::MainWindow(ClassWorkspace &wks)
    : QMainWindow(nullptr)
    , ui(new Ui::MainWindow)
    , m_wks(wks)
{
    QSettings settings;
    ui->setupUi(this);

    connect(ui->actionImportENEX, SIGNAL(triggered(bool)), this, SLOT(onImportEnex()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionOptions, SIGNAL(triggered()), this, SLOT(onOptions()));
//    connect(ui->actionOpenInNewWindow, &QAction::triggered, this, [=]() { m_wks.onNoteOpen(guid, true); });
//    connect(ui->actionOpenInNewWindow, SIGNAL(triggered()), this, SLOT(openInExternalWindow()));

    ui->tableView->setupModel();

    // When the user single- and double-clicks on a note in the table view, load that note into the main/aux editor
    connect(ui->tableView, &WidgetTableView::noteSingleClicked, this, [=](QString guid) { m_wks.onNoteOpen(guid, true); });
    connect(ui->tableView, &WidgetTableView::noteDoubleClicked, this, [=](QString guid) { m_wks.onNoteOpen(guid, false); });

    // The workspace signals the main editor or the aux editor to load a specific note
    connect(&m_wks, SIGNAL(mainEditorLoadNote(ClassNote&)), ui->textEdit, SLOT(loadNote(ClassNote&)));
    connect(&m_wks, SIGNAL(auxEditorLoadNote(ClassNote&)), this, SLOT(openInExternalEditor(ClassNote&)));

    // Testing the main view
    connect(ui->actionViewHorizontal, &QAction::triggered, this, [=]() { ui->splitterEdit->setOrientation(Qt::Horizontal); });
    connect(ui->actionViewVertical, &QAction::triggered, this, [=]() { ui->splitterEdit->setOrientation(Qt::Vertical); });

    restoreWindowGeometry();

    // Load last recently used note
    wks.onNoteOpen(settings.value("lruNote", QString()).toString());
}

MainWindow::~MainWindow()
{
    saveWindowGeometry();
    delete ui;
}

/*
 * Import Evernote exported files
 */
void MainWindow::onImportEnex()
{
    DialogImportEnex enex(this, &m_wks);
    enex.exec();

    ui->tableView->setupModel();
}

void MainWindow::onOptions()
{
    DialogOptions dlg(this);
    dlg.exec();
}

void MainWindow::openInExternalEditor(ClassNote &note)
{
    if (!m_editWindows.contains(note.guid()))
    {
        EditWindow *w = new EditWindow(this, note);
        w->show();
        m_editWindows[note.guid()] = w;
    }
    else
        static_cast<QMainWindow *>(m_editWindows[note.guid()])->activateWindow();
}

void MainWindow::saveWindowGeometry()
{
    QSettings settings;
    settings.setValue("splitterTree", ui->splitterTree->saveState());
    settings.setValue("splitterEdit", ui->splitterEdit->saveState());
    settings.setValue("mainWindowGeometry", saveGeometry());
}

void MainWindow::restoreWindowGeometry()
{
    QSettings settings;
    ui->splitterTree->setStretchFactor(0, 1);
    ui->splitterTree->setStretchFactor(1, 4);
    ui->splitterTree->restoreState(settings.value("splitterTree").toByteArray());
    ui->splitterEdit->restoreState(settings.value("splitterEdit").toByteArray());
    if (!settings.contains("mainWindowGeometry"))
    {
        QScreen *screen = QApplication::screenAt(QPoint(0,0));
        QRect w = screen->availableGeometry();
        setGeometry(w.center().x() - 500, w.center().y() - 350, 1000, 700);
    }
    else
        restoreGeometry(settings.value("mainWindowGeometry", QByteArray()).toByteArray());
}
