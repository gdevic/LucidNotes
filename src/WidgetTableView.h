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
    QSqlTableModel *m_model;
    ClassDatabase m_db; // SQLite database connection supporting this view
};

#endif // WIDGETTABLEVIEW_H
