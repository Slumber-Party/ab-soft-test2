#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QColor>
#include <QUndoStack>
#include "treeitem.h"

#include "adddepartmentcommand.h"
#include "addemploymentcommand.h"
#include "deletedepartmentcommand.h"
#include "deleteemploymentcommand.h"

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

    friend class AddDepartmentCommand;
    friend class AddEmploymentCommand;
    friend class DeleteDepartmentCommand;
    friend class DeleteEmploymentCommand;

public:
    TreeModel(const QStringList &headers, QObject *parent = nullptr);
    ~TreeModel() override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
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

    QUndoStack *undoStack() const;
private:
    TreeItem *getItem(const QModelIndex &index) const;
    QString trHeaderInTag(const QString header) const;

    TreeItem *rootItem;

    QString currentPathToXmlFile;

    QUndoStack *undoStack_;
};

#endif // TREEMODEL_H
