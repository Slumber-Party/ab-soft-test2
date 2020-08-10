#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QColor>

#include <QUndoStack>

#include "treeitem.h"
#include "changeitemvaluecommand.h"

#include "deleteitemcommand.h"

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

    friend class DeleteItemCommand;
    friend class ChangeItemValueCommand;

public:
    TreeModel(const QStringList &headers, QObject *parent = nullptr);
    ~TreeModel();
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, //этот метод представляет собой интерфейс для удаления
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

    bool setupFromXML(const QString &pathToXmlFile);
    bool saveToXML(QString pathToXmlFile = "");

    void setUndoStack(QUndoStack *stack);
private:
    bool changeData(const QModelIndex &index, const QVariant &value, //этот метод занимается непосредственно удалением и вызывается только объектом Command
                    int role = Qt::EditRole);

    TreeItem *getItem(const QModelIndex &index) const;
    QString trHeaderInTag(const QString header) const;

    TreeItem *rootItem;

    QUndoStack *undoStack;

    QString currentPathToXmlFile;
};

#endif // TREEMODEL_H
