#pragma once

#include "Utils.h"
#include <QAbstractItemModel>
#include <QTreeView>

namespace Ui { class WidgetTreeView; }
class CTreeModel;

class WidgetTreeView : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTreeView(QWidget *parent = nullptr);
    ~WidgetTreeView();

    void setupModel();

signals:
    void updateNotelist(const QString s, LIST_BY key);

private slots:
    void on_btNewNote_clicked();

    void onShortcutsClicked(bool);
    void onShortcutsExpandClicked(bool);
    void onShortcutsPlusClicked(bool);
    void onShortcutsSearchClicked(bool);

    void onNotebooksExpandClicked(bool);
    void onNotebooksLeafClicked(const QModelIndex &index);

private:
    Ui::WidgetTreeView *ui;
    CTreeModel *m_tmodel;
};
