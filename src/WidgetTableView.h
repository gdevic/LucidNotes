#pragma once

#include "ClassDatabase.h"
#include "Utils.h"
#include <QWidget>
#include <QtSql>

namespace Ui { class WidgetTableView; }

/*
 * This proxy model forces all data from the query to be populated.
 * We need to do that so the random updates do not lock the database.
 */
class PopulateAllProxy : public QIdentityProxyModel
{
    Q_OBJECT

public:
    explicit PopulateAllProxy(QWidget *parent = nullptr) : QIdentityProxyModel{parent}
    {
        connect(this, &QAbstractProxyModel::sourceModelChanged,
                this, &PopulateAllProxy::onSourceModelChanged);
    }

    void onSourceModelChanged()
    {
        if (sourceModel()->canFetchMore(QModelIndex{}))
        {
            QTimer::singleShot(0, this, [this]{
                if (!sourceModel()->canFetchMore(QModelIndex{})) return;
                sourceModel()->fetchMore(QModelIndex{});
                onSourceModelChanged();
            });
        }
    }
};

class WidgetTableView : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTableView(QWidget *parent = nullptr);
    ~WidgetTableView();
    bool setupModel();

public slots:
    void onUpdateQuery(const QString name, LIST_BY key);

signals:
    void noteSingleClicked(QString guid);
    void noteDoubleClicked(QString guid);

private:
    Ui::WidgetTableView *ui;
    ClassDatabase m_db;
    QSqlQueryModel m_sqlModel; // Query model to select just a subset of notes for our table
    PopulateAllProxy m_pxProxy; // Populate all data from the query
    QSortFilterProxyModel m_sortProxy; // Filter is needed to facilitate sorting of the query rows
};
