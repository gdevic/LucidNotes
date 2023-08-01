#include "CTreeItem.h"

CTreeItem::CTreeItem(const QList<QVariant> &data, CTreeItem *parent)
    : itemData(data), parentItem(parent)
{}

CTreeItem::~CTreeItem()
{
    qDeleteAll(childItems);
}

CTreeItem *CTreeItem::child(int number)
{
    if (number < 0 || number >= childItems.size())
        return nullptr;
    return childItems.at(number);
}

int CTreeItem::childCount() const
{
    return childItems.count();
}

int CTreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<CTreeItem*>(this));
    return 0;
}

int CTreeItem::columnCount() const
{
    return itemData.count();
}

QVariant CTreeItem::data(int column) const
{
    if (column < 0 || column >= itemData.size())
        return QVariant();
    return itemData.at(column);
}

bool CTreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QList<QVariant> data(columns);
        CTreeItem *item = new CTreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool CTreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    for (CTreeItem *child : std::as_const(childItems))
        child->insertColumns(position, columns);

    return true;
}

CTreeItem *CTreeItem::parent()
{
    return parentItem;
}

bool CTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool CTreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    for (CTreeItem *child : std::as_const(childItems))
        child->removeColumns(position, columns);

    return true;
}

bool CTreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}
