#include "itemcommand.h"
#include "treemodel.h"

ItemCommand::ItemCommand(int position, int rows, const QModelIndex &parent, TreeModel *model, bool isDelete)
{
    row = position;
    rows_ = rows;
    ind = parent;
    model_ = model;
    isDelete_ = isDelete;
}

void ItemCommand::undo()
{
    if(isDelete_)
        model_->insertRows(row,rows_,ind);
    else
        model_->removeRows(row,rows_,ind);
}

void ItemCommand::redo()
{
    if(isDelete_)
        model_->removeRows(row,rows_,ind);
    else
        model_->insertRows(row,rows_,ind);
}
