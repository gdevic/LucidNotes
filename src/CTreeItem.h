#pragma once

#include <QVariant>
#include <QList>

class CTreeItem
{
public:
    explicit CTreeItem(const QList<QVariant> &data, CTreeItem *parent = nullptr);
    ~CTreeItem();

    CTreeItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    CTreeItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

private:
    QList<CTreeItem *> childItems;
    QList<QVariant> itemData;
    CTreeItem *parentItem;
};
