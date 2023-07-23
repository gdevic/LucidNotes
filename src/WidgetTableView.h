#ifndef WIDGETTABLEVIEW_H
#define WIDGETTABLEVIEW_H

#include "ClassDatabase.h"
#include <QWidget>
#include <QtSql>

namespace Ui { class WidgetTableView; }

class WidgetTableView : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTableView(QWidget *parent = nullptr);
    ~WidgetTableView();
    bool setupModel();

private:
    Ui::WidgetTableView *ui;
    ClassDatabase m_db; // SQLite database connection supporting this view
    QSqlQueryModel m_model; // Query model to select just a subset of notes for our table
    QSortFilterProxyModel m_proxy; // Filter is needed to facilitate sorting of the query rows
};

#endif // WIDGETTABLEVIEW_H
