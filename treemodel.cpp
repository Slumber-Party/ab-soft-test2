#include "treemodel.h"
#include <qdebug.h>

TreeModel::TreeModel(const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    for (const QString &header : headers)
        rootData << header;

    rootItem = new TreeItem(rootData);

    undoStack_ = new QUndoStack();
}


TreeModel::~TreeModel()
{
    delete undoStack_;
    delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::BackgroundColorRole && index.row() == 0 && index.parent().isValid())
    {
        return QColor(Qt::darkGray);
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags item_flags = Qt::ItemIsEnabled;

    if (index.row() != 0 && index.parent().isValid()) //если это вторичный заголовок
        item_flags |= Qt::ItemIsEditable;
    if(!index.parent().isValid() && index.column() == 0) //если это не поле названия отдела
        item_flags |= Qt::ItemIsEditable;

    if(!index.parent().isValid() || (index.parent().isValid() && index.row() !=0 )) //если это запись об отделе или сотруднике
        item_flags |= Qt::ItemIsSelectable;

    return item_flags;
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QString TreeModel::trHeaderInTag(const QString header) const
{
    if(header == "Фамилия")
        return "surname";
    if(header == "Имя")
        return "name";
    if(header == "Отчество")
        return "middleName";
    if(header == "Должность")
        return "function";
    if(header == "Зарплата")
        return "salary";

    return "undefined";
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    if(orientation == Qt::Horizontal && role == Qt::BackgroundColorRole)
    {
        return QColor(Qt::darkGray);
    }

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return QModelIndex();

    if(parent.isValid() && row == 0)
        return createIndex(row,column,new TreeItem({"Фамилия","Имя","Отчество","Должность","Зарплата"},parentItem));

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    beginInsertColumns(parent, position, position + columns - 1);
    const bool success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginInsertRows(parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position,
                                                    rows,
                                                    rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem ? childItem->parent() : nullptr;

    if (parentItem == rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    beginRemoveColumns(parent, position, position + columns - 1);
    const bool success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

bool TreeModel::setupFromXML(const QString &pathToXmlFile)
{
    QFile xmlFile(pathToXmlFile);

    if(!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        xmlFile.close();
        return false;
    }

    currentPathToXmlFile = pathToXmlFile;

    QXmlStreamReader xml(&xmlFile);

    int salarySum = 0, employments = 0;
    TreeItem *currentDepItem = new TreeItem({"","","","",""},rootItem);
    QVector<QVariant> currentEmploymentData;

    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartElement)
        {
            if(xml.name() == "department")
            {
                currentDepItem->setData(0,xml.attributes().value("name").toString());
            }

            if(xml.name() == "employment")
            {
                employments++;
            }

            if(xml.name() == "name" || xml.name() == "surname" || xml.name() == "middleName"
                    || xml.name() == "function")
            {
                currentEmploymentData.push_back(xml.readElementText());
            }

            if(xml.name() == "salary")
            {
                QString str = xml.readElementText();
                salarySum += str.toInt();
                currentEmploymentData.push_back(str);
            }
        }
        else if(token == QXmlStreamReader::EndElement)
        {
            if(xml.name() == "department")
            {
                currentDepItem->setData(1,employments);
                currentDepItem->setData(2,(double)salarySum/employments);

                rootItem->insertChild(currentDepItem);

                salarySum = 0;
                employments = 0;

                currentDepItem = new TreeItem({"","","","",""},rootItem);
            }

            if(xml.name() == "employment")
            {
                currentDepItem->insertChild(employments-1,currentEmploymentData);
                currentEmploymentData.clear();
            }
        }
    }

    xml.clear();
    xmlFile.close();

    if(xml.hasError() || rootItem->childCount() == 0) //если нет ни одного департамента
    {
        currentPathToXmlFile = "";
        return false;
    }
    else
    {
        return true;
    }
}

bool TreeModel::saveToXML(const QString &pathToXmlFile)
{
    if(pathToXmlFile != QString())
    {
        currentPathToXmlFile = pathToXmlFile;
    }

    QFile xmlFile(currentPathToXmlFile);

    if(!xmlFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        xmlFile.close();
        return false;
    }

    QXmlStreamWriter xml(&xmlFile);

    xml.setAutoFormatting(true);
    xml.writeStartDocument("1.0");

    xml.writeStartElement("departments");

    //header item всегда один и тот же для всех сотрудников любого департамента, а значит можно взять самый верхний
    //действие безопасно, т.к гарантируется наличие хотя бы одного департамента
    TreeItem *headerItem = static_cast<TreeItem*>(index(0,0,index(0,0)).internalPointer());

    for(int i =0;i<rootItem->childCount();i++)
    {
        TreeItem *departmentItem = rootItem->child(i);

        xml.writeStartElement("department");
        xml.writeAttribute("name",departmentItem->data(0).toString());
        xml.writeStartElement("employments");

        for(int j = 0;j<departmentItem->childCount();j++)
        {
            TreeItem *employmentItem = departmentItem->child(j);

            xml.writeStartElement("employment");

            for(int k = 0;k<employmentItem->columnCount();k++)
            {
                xml.writeTextElement(trHeaderInTag(headerItem->data(k).toString())
                                     ,employmentItem->data(k).toString());
            }

            xml.writeEndElement();
        }

        xml.writeEndElement();
        xml.writeEndElement();
    }

    xml.writeEndElement();
    xml.writeEndDocument();

    xmlFile.close();

    return !xml.hasError();
}

QUndoStack *TreeModel::undoStack() const
{
    return undoStack_;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    const TreeItem *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}
