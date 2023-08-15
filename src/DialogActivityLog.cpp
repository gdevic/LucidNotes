#include "DialogActivityLog.h"
#include "ui_DialogActivityLog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

DialogActivityLog::DialogActivityLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogActivityLog)
{
    ui->setupUi(this);

    connect(ui->saveAs, SIGNAL(clicked(bool)), this, SLOT(saveAs(bool)));
}

DialogActivityLog::~DialogActivityLog()
{
    delete ui;
}

/*
 * We never close this dialog but instead just hide it
 */
void DialogActivityLog::closeEvent(QCloseEvent *e)
{
    QSettings settings;
    settings.setValue("activityLogGeometry", saveGeometry());

    hide();
    e->ignore();
}

void DialogActivityLog::showEvent(QShowEvent *)
{
    QSettings settings;
    restoreGeometry(settings.value("activityLogGeometry").toByteArray());
}

/*
 * Save the log to a file
 * TODO: We will save complete log although the user might have selected a subset of lines. Maybe that's ok.
 */
void DialogActivityLog::saveAs(bool)
{
    QString logName = QFileDialog::getSaveFileName(this, "Save As", "", "Log (*.log)");
    if (!logName.isEmpty())
    {
        qInfo() << "Save log as" << logName;
        QFile file(logName);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream out(&file);
            out << ui->editLog->toPlainText();
        }
        else
            QMessageBox::critical(this, "Save", "Error saving log to a file");
    }
}

/*
 * This handler is called from a global handler in main.cpp, messageHandler
 */
void DialogActivityLog::messageHandler(QStringList &log)
{
    foreach (QString line, log)
        ui->editLog->appendPlainText(line);
    log.clear();

    ui->labelEntries->setText(QString("%1 log entries").arg(ui->editLog->blockCount()));
}
