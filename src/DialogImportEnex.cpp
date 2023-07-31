#include "DialogImportEnex.h"
#include "ui_DialogImportEnex.h"
#include "ClassDatabase.h"
#include "ClassNote.h"
#include "ClassWorkspace.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

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
    connect(ui->pbMerge, SIGNAL(clicked()), this, SLOT(onMerge()));
    connect(ui->pbStop, SIGNAL(clicked()), this, SLOT(onStop()));

    connect(ui->editEnex, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
    connect(ui->editExb, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));

    QSettings settings;
    ui->editEnex->setText(settings.value("importEnexFile", "").toString());
    ui->editExb->setText(settings.value("importExbFile", "").toString());

    restoreGeometry(settings.value("importEnexWindowGeometry", QByteArray()).toByteArray());
}

DialogImportEnex::~DialogImportEnex()
{
    QSettings settings;
    settings.setValue("importEnexWindowGeometry", saveGeometry());

    delete ui;
}

/*
 * User clicked on the Select ENEX button to select an exported file
 */
void DialogImportEnex::onFileEnex()
{
    QSettings settings;
    QString fileName = QFileDialog::getOpenFileName(this, "Select ENEX File", settings.value("importEnexFile").toString(), "Enex notes (*.enex);;All files (*.*)").trimmed();
    if(!fileName.isNull())
    {
        QString ret = checkEnexFile(fileName);
        if (ret.isEmpty())
        {
            ui->editEnex->setText(fileName);
            settings.setValue("importEnexFile", fileName);
        }
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
    QString fileName = QFileDialog::getOpenFileName(this, "Select Evernote EXB File", settings.value("importExbFile").toString(), "Evernote database (*.exb);;All files (*.*)").trimmed();
    if(!fileName.isNull())
    {
        QString ret = checkExbFile(fileName);
        if (ret.isEmpty())
        {
            ui->editExb->setText(fileName);
            settings.setValue("importExbFile", fileName);
        }
        else
            QMessageBox::critical(this, "Evernote EXB File", "The selected file does not appear to be a valid Evernote Database file!");
    }
}

/*
 * User clicked on the Import button to start the import process
 */
void DialogImportEnex::onImport()
{
    qInfo() << __FUNCTION__ << "Thread:" << QThread::currentThread();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]()
    {
        QString s = QString("running:%1 cancelled:%2 fin:%3").arg(m_future.isRunning()).arg(m_future.isCanceled()).arg(m_future.isFinished());
        ui->labelStatus->setText(s);
        ui->labelStatus->update();
        ui->textProgress->appendPlainText(s);
    });
    timer->start(500);

    QString ret = checkEnexFile(getEnex());
    if (ret.isEmpty())
    {
        if (getExb().isEmpty() || checkExbFile(getExb()).isEmpty())
        {
            m_enex.thID = QThread::currentThread();
            ui->textProgress->clear();

            readNotes(getEnex());

//            m_future = QtConcurrent::run(&DialogImportEnex::readNotes, this, getEnex())
//                           .then(QtFuture::Launch::Inherit, [this](bool res) { qInfo() << "Phase 2"; return res ? readDatabase(getExb()) : res; });
//                           .then(QtFuture::Launch::Inherit, [this](bool res) { qInfo() << "Phase 3"; return res ? importNotes() : res; });
        }
        else
            QMessageBox::critical(this, "Evernote EXB File", "The selected file does not appear to be a valid Evernote Database file!");
    }
    else
        QMessageBox::critical(this, "ENEX File", ret);
}

void DialogImportEnex::onMerge()
{
    qInfo() << __FUNCTION__ << "Thread:" << QThread::currentThread();

    // Phase 3: Store (effectively import) new notes into our own database
    foreach (ClassNote *note, m_enex.m_notes)
        m_wks->addNote(note);
}

/*
 * User stopped the import in progress
 * Stop the threads and stay put where you are...
 */
void DialogImportEnex::onStop()
{
    m_future.cancel(); // Track the canceled status in QFuture<>
    m_enex.cancel();
}

void DialogImportEnex::onTextChanged(QString s)
{
    bool validEnex = checkEnexFile(getEnex()).isEmpty();
    bool validExb = getExb().isEmpty() || checkExbFile(getExb()).isEmpty();
    ui->pbImport->setEnabled(validEnex && validExb);
    ui->editEnex->setStyleSheet(validEnex ? "" : "color: red");
    ui->editExb->setStyleSheet(validExb ? "" : "color: red");
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

/*
 * The main import function; it takes care of all phases of the import
 * This function runs in a Qt Concurrent thread.
 */
bool DialogImportEnex::readNotes(const QString enexFileName)
{
    qInfo() << __FUNCTION__ << "Thread:" << QThread::currentThread();

    // Phase 1: Using the ClassEnex class, prepare it to load a new set of notes; load the notes
    QString ret = m_enex.import(enexFileName);
    return ret.isEmpty();
}

bool DialogImportEnex::readDatabase(const QString exbFileName)
{
    qInfo() << __FUNCTION__ << "Thread:" << QThread::currentThread();

    // Phase 2: if we have access to Evernote database, update list of notes with its data
    if (!exbFileName.isEmpty())
    {
    }
    return true;
}

bool DialogImportEnex::importNotes()
{
    qInfo() << __FUNCTION__ << "Thread:" << QThread::currentThread();

    // Phase 3: Store (effectively import) new notes into our own database
    foreach (ClassNote *note, m_enex.m_notes)
        m_wks->addNote(note);

    return true;
}
