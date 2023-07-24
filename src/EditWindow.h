#pragma once

#include <QMainWindow>

namespace Ui { class EditWindow; }

class EditWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditWindow(QWidget *parent = nullptr);
    ~EditWindow();

private slots:
    void closeEvent(QCloseEvent *event);

private:
    void writeSettings();
    void readSettings();

private:
    Ui::EditWindow *ui;
};
