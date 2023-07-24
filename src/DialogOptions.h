#pragma once

#include <QDialog>

namespace Ui { class DialogOptions; }
class QTreeWidgetItem;

/*
 * This class implements the UI to edit application options
 */
class DialogOptions : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOptions(QWidget *parent);
    ~DialogOptions();

private slots:
    void onApply();
    void accept() override;
    void reject() override;
    void onOptionTreeClicked(QTreeWidgetItem*, int);
    void onChangeWksDirClicked();
    void onResetWksDirClicked();

private:
    Ui::DialogOptions *ui;
};
