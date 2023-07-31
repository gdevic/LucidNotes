#pragma once

#include "ClassEnex.h"
#include <QDialog>
#include <QFutureWatcher>
#include <QtConcurrent>

namespace Ui { class DialogImportEnex; }
class ClassWorkspace;

class DialogImportEnex : public QDialog
{
    Q_OBJECT

public:
    DialogImportEnex(QWidget *parent, ClassWorkspace *wks);
    ~DialogImportEnex();

private slots:
    void onFileEnex();
    void onFileExb();
    void onImport();
    void onMerge();
    void onStop();
    void onTextChanged(QString);

private:
    QString checkEnexFile(const QString fileName);
    QString checkExbFile(const QString fileName);
    bool readNotes(const QString enexFileName);
    bool readDatabase(const QString exbFileName);
    bool importNotes();
    const QString getEnex();
    const QString getExb();

private:
    Ui::DialogImportEnex *ui;
    ClassWorkspace *m_wks;
    ClassEnex m_enex;
    QFuture<bool> m_future;
};
