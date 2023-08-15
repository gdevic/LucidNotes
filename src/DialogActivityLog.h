#pragma once

#include <QDialog>

namespace Ui { class DialogActivityLog; }

class DialogActivityLog : public QDialog
{
    Q_OBJECT

public:
    explicit DialogActivityLog(QWidget *parent = nullptr);
    ~DialogActivityLog();
    void messageHandler(QStringList &log);

private slots:
    void closeEvent(QCloseEvent *e) override;
    void showEvent(QShowEvent *) override;
    void saveAs(bool);

private:
    Ui::DialogActivityLog *ui;
};
