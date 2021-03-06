#include "treeitem.h"

TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent)
    : itemData(data),
      parentItem(parent)
{}

TreeItem::~TreeItem()
{
    for(auto child : childItems)
    {
        child->~TreeItem();
    }
}

TreeItem *TreeItem::child(int number)
{
    if (number < 0 || number >= childItems.size())
        return nullptr;
    return childItems.at(number);
}

int TreeItem::childCount() const
{
    return childItems.count();
}

int TreeItem::columnCount() const
{
    return itemData.count();
}

QVariant TreeItem::data(int column) const
{
    if (column < 0 || column >= itemData.size())
        return QVariant();
    return itemData.at(column);
}

bool TreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;


    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        data[0] = "Введите...";
        TreeItem *item = new TreeItem(data, this);
        childItems.insert(position, item);
    }

    if(this->parent() != nullptr)
    {
        this->setData(2,this->data(1).toDouble()*this->data(2).toDouble()/(this->data(1).toDouble()+count));
        this->setData(1,this->data(1).toInt() + count); //0 - name, 1 - employment amount, 2 - avr salary
    }

    return true;
}

bool TreeItem::insertChild(TreeItem *item)
{
    childItems.push_back(item);

    return true;
}

bool TreeItem::insertChild(int position,TreeItem *item)
{
    if (position < 0 || position > childItems.size())
        return false;

    childItems.insert(position,item);

    return true;
}

bool TreeItem::insertChild(int position, QVector<QVariant> &data)
{
    return insertChild(position,new TreeItem(data,this));
}

bool TreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    for (TreeItem *child : childItems)
        child->insertColumns(position, columns);

    return true;
}

TreeItem *TreeItem::parent()
{
    return parentItem;
}

bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    int salarySum = 0;
    for (int row = 0; row < count; ++row)
    {
        TreeItem *childItem = childItems.takeAt(position);
        salarySum += childItem->data(4).toInt();
        delete childItem;
    }

    if(this->parent() != nullptr)
    {
        this->setData(2,(this->data(1).toDouble() * this->data(2).toDouble()-salarySum) / (this->data(1).toDouble()-count));
        this->setData(1,this->data(1).toInt() - count); //0 - name, 1 - employment amount, 2 - avr salary
    }

    return true;
}

bool TreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    for (TreeItem *child : qAsConst(childItems))
        child->removeColumns(position, columns);

    return true;
}

int TreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
    return 0;
}

bool TreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}
