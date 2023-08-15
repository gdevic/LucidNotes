#pragma once

#include "ClassNote.h"
#include <QMainWindow>

namespace Ui { class EditWindow; }

class EditWindow : public QMainWindow
{
    Q_OBJECT

public:
    EditWindow(QWidget *parent, ClassNote &note, uint positionOffset);
    ~EditWindow();

signals:
    void windowClosed(EditWindow *myself, QString guid);

private slots:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::EditWindow *ui;
    QString m_guid;
};
