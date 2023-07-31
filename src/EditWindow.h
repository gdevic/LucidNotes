#pragma once

#include "ClassNote.h"
#include <QMainWindow>

namespace Ui { class EditWindow; }

class EditWindow : public QMainWindow
{
    Q_OBJECT

public:
    EditWindow(QWidget *parent, ClassNote &note);
    ~EditWindow();

private slots:
    void closeEvent(QCloseEvent *event) override;

private:
    void writeSettings();
    void readSettings();

private:
    Ui::EditWindow *ui;
};
