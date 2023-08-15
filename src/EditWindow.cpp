#include "EditWindow.h"
#include "ui_EditWindow.h"
#include <QCloseEvent>
#include <QSettings>

EditWindow::EditWindow(QWidget *parent, ClassNote &note, uint positionOffset) :
    QMainWindow(parent),
    ui(new Ui::EditWindow),
    m_guid(note.guid())
{
    ui->setupUi(this);

    ui->editNote->loadNote(note);

    // Restore the editor position on the screen but offset it by the given number of pixels
    // We send that from the main window that keeps track of how many external windows we have opened so they are staggered
    QSettings settings;
    if (settings.contains("editWindowGeometry"))
    {
        restoreGeometry(settings.value("editWindowGeometry").toByteArray());
        move(pos() + QPoint(positionOffset, positionOffset));
    }
}

EditWindow::~EditWindow()
{
    QSettings settings;
    settings.setValue("editWindowGeometry", saveGeometry());

    delete ui;
}

void EditWindow::closeEvent(QCloseEvent *event)
{
    emit windowClosed(this, m_guid);

    event->accept();
}
