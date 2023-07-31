#pragma once

#include <QTreeView>
#include <QAbstractItemModel>
#include <QWidget>
#include <QFileSystemModel>

namespace Ui { class WidgetTreeView; }

class WidgetTreeView : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTreeView(QWidget *parent = nullptr);
    ~WidgetTreeView();

private slots:
    void on_btNewNote_clicked();

    void onShortcutsClicked(bool);
    void onShortcutsExpandClicked(bool);
    void onShortcutsPlusClicked(bool);
    void onShortcutsSearchClicked(bool);

    void onNotebooksExpandClicked(bool);

private:
    Ui::WidgetTreeView *ui;
};
