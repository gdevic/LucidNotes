#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ClassWorkspace.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ClassWorkspace &wks);
    ~MainWindow();

private slots:
    void onImport();
    void closeEvent(QCloseEvent *event);
    void onOptions();

private:
    void writeSettings();
    void readSettings();

private:
    Ui::MainWindow *ui;
    ClassWorkspace &m_wks; // Workspace that this application window works on
};
#endif // MAINWINDOW_H
