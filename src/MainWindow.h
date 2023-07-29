#pragma once

#include "ClassWorkspace.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class EditWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ClassWorkspace &wks);
    ~MainWindow();

private slots:
    void onImport();
    void onOptions();
    void openInExternalEditor(ClassNote &note); // XXX

private:
    void saveWindowGeometry();
    void restoreWindowGeometry();

private:
    Ui::MainWindow *ui;
    QMap<QString, EditWindow *> m_editWindows; // A map of extra edit windows spawned by the user keyed by the note GUID
    ClassWorkspace &m_wks; // Workspace that this application window works on
};
