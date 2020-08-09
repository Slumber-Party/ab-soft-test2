#include "adddepartmentcommand.h"
#include "treemodel.h"

AddDepartmentCommand::AddDepartmentCommand(int position, const QModelIndex &parent, TreeModel *model, bool &result)
    : position_(position),model_(model),indexParent(parent)
{
    TreeItem *parentItem = model->getItem(parent);

    model->beginInsertRows(parent, position, position);
    const bool success = parentItem->insertChildren(position,
                                                    1,
                                                    model->rootItem->columnCount());
    model->endInsertRows();

    result = success;
}

void AddDepartmentCommand::undo()
{

}

void AddDepartmentCommand::redo()
{

}
