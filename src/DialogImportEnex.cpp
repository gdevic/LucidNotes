#include "DialogImportEnex.h"
#include "ui_DialogImportEnex.h"
#include "ClassDatabase.h"
#include "ClassNote.h"
#include "ClassWorkspace.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

/*
 * Imports the Evernote exported file
 * Creates a new notebook, "Imported on <date>.n" with increasing "n" for each new import (if on the same day)
 * Imports all notes into that notebook
 */

inline const QString DialogImportEnex::getEnex() { return ui->editEnex->text().trimmed(); }
inline const QString DialogImportEnex::getExb() { return ui->editExb->text().trimmed(); }

DialogImportEnex::DialogImportEnex(QWidget *parent, ClassWorkspace *wks) :
    QDialog(parent),
    ui(new Ui::DialogImportEnex),
    m_wks(wks)
{
    ui->setupUi(this);

    connect(ui->pbFileEnex, SIGNAL(clicked()), this, SLOT(onFileEnex()));
    connect(ui->pbFileExb, SIGNAL(clicked()), this, SLOT(onFileExb()));
    connect(ui->pbImport, SIGNAL(clicked()), this, SLOT(onImport()));

    QSettings settings;
    ui->editEnex->setText(settings.value("importEnexFile", "").toString());
    ui->editExb->setText(settings.value("importExbFile", "").toString());

    restoreGeometry(settings.value("importEnexWindowGeometry", QByteArray()).toByteArray());
}

DialogImportEnex::~DialogImportEnex()
{
    QSettings settings;
    settings.setValue("importEnexFile", getEnex());
    settings.setValue("importExbFile", getExb());
    settings.setValue("importEnexWindowGeometry", saveGeometry());

    delete ui;
}

/*
 * User clicked on the Select ENEX button to select an exported file
 */
void DialogImportEnex::onFileEnex()
{
    QSettings settings;
    QString fileName = QFileDialog::getOpenFileName(this, "Select ENEX File", getEnex(), "Enex notes (*.enex);;All files (*.*)").trimmed();
    if(!fileName.isNull())
    {
        QString ret = checkEnexFile(fileName);
        if (ret.isEmpty())
            ui->editEnex->setText(fileName);
        else
            QMessageBox::critical(this, "ENEX File", ret);
    }
}

/*
 * User clicked on the Select EXB button to select an Evernote database file
 */
void DialogImportEnex::onFileExb()
{
    QSettings settings;
    QString fileName = QFileDialog::getOpenFileName(this, "Select Evernote EXB File", getExb(), "Evernote database (*.exb);;All files (*.*)").trimmed();
    if(!fileName.isNull())
    {
        QString ret = checkExbFile(fileName);
        if (ret.isEmpty())
            ui->editExb->setText(fileName);
        else
            QMessageBox::critical(this, "Evernote EXB File", "The selected file does not appear to be a valid Evernote Database file!");
    }
}

/*
 * User clicked on the Import button to start the import process
 */
void DialogImportEnex::onImport()
{
    setCursor(Qt::WaitCursor);
    QString ret = checkEnexFile(getEnex());
    if (ret.isEmpty())
    {
        // If we are provided with the EN database, read some extra information
        if (!getExb().isEmpty() && ui->checkRecreateTree->isChecked())
        {
            if (m_enex.readEnDatabase(getExb()) == false)
            {
                QMessageBox::critical(this, "Import", "Error reading Evernote database");
                return;
            }
        }

        ret = m_enex.import(getEnex());
        if (ret.isEmpty())
        {
            // Check for duplicate notes and ask the user to decide what to do with them, if any was found
            QList<ClassNote *> dups = m_enex.getDuplicateNotes(); // Pointers to all duplicate notes held in m_enex structure
            bool remove_all_dups = false;

            foreach (auto note, dups)
            {
                if (remove_all_dups == true)
                    m_enex.removeNote(note);
                else
                {
                    QMessageBox msg(this);
                    msg.setText("Imported note '" + note->title() + "' is a duplicate from an already existing note. Should I overwrite the existing note?");
                    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll | QMessageBox::Abort);
                    msg.setDefaultButton(QMessageBox::Yes);
                    int ret = msg.exec();
                    if (ret == QMessageBox::Abort)
                        goto end;
                    if (ret == QMessageBox::YesToAll)
                        break;
                    if (ret == QMessageBox::NoToAll)
                        remove_all_dups = true;
                    if ((ret == QMessageBox::No) || (ret == QMessageBox::NoToAll))
                        m_enex.removeNote(note);
                }
            }

            // Save all imported notes to file blobs
            if (m_enex.saveAll(m_wks->getDataDir()))
            {
                // Update local database with each note's meta data
                if (m_enex.updateDatabaseAll())
                {
                    qInfo() << "Import completed";
                    // Delete imported notes records in m_enex destructor
                }
                else
                    QMessageBox::critical(this, "Import", "Error updating local database");
            }
            else
                QMessageBox::critical(this, "Import", "Error saving notes");
        }
        else
            QMessageBox::critical(this, "Import", "Error reading Evernote notes: " + ret);
    }
    else
        QMessageBox::critical(this, "ENEX File", ret);
end:
    setCursor(Qt::ArrowCursor);

    close(); // Close the dialog
}

/*
 * Do a reasonable check that the selected file has expected XML format
 */
QString DialogImportEnex::checkEnexFile(const QString fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        // The XML document should be formatted this way:
        // Line 1: <?xml version="1.0" encoding="UTF-8"?>
        // Line 2: <!DOCTYPE en-export SYSTEM "http://xml.evernote.com/pub/evernote-export2.dtd">
        QString line = in.readLine();
        line = in.readLine();
        if (line.contains("http://xml.evernote.com/pub/evernote-export2.dtd"))
            return QString();
        return "The selected file does not appear to be a valid Evernote Export file!";
    }
    return "Unable to open selected file!";
}

/*
 * Do a reasonable check that the selected file has expected SQLite3 format
 */
QString DialogImportEnex::checkExbFile(const QString fileName)
{
    if (!QFileInfo::exists(fileName))
        return "File does not exist";
    ClassDatabase db;
    return db.open("enex", fileName);
}
