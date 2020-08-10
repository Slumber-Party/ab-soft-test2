#include "additemcommand.h"
#include "treemodel.h"

AddItemCommand::AddItemCommand(int position, int rows, const QModelIndex &parent, TreeModel *model)
{
    row = position;
    rows_ = rows;
    ind = parent;
    model_ = model;

    if(parent.isValid())
    {
        p_row = parent.row();
        p_column = parent.column();
    }
}

void AddItemCommand::undo()
{
    if(ind.isValid())
        model_->removeRows(row,rows_,model_->index(p_row,p_column));
    else
        model_->removeRows(row,rows_);
}

void AddItemCommand::redo()
{
    if(ind.isValid())
        model_->insertRows(row,rows_,model_->index(p_row,p_column));
    else
        model_->insertRows(row,rows_);
}
