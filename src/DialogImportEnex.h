#pragma once

#include "ClassEnex.h"
#include <QDialog>

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

private:
    QString checkEnexFile(const QString fileName);
    QString checkExbFile(const QString fileName);
    const QString getEnex();
    const QString getExb();

private:
    Ui::DialogImportEnex *ui;
    ClassWorkspace *m_wks;
    ClassEnex m_enex;
};
