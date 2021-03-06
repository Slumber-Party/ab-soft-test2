#include "changeitemvaluecommand.h"
#include "treemodel.h"

ChangeItemValueCommand::ChangeItemValueCommand(const QModelIndex &index, const QVariant &value, int role, TreeModel *model)
{
    role_ = role;
    model_ = model;

    row = index.row();
    column = index.column();

    if(index.parent().isValid())
    {
        p_row = index.parent().row();
        p_column = index.parent().column();

        TreeItem *parent = static_cast<TreeItem*>(index.parent().internalPointer());
        oldAvrValue = parent->data(2).toInt();
    }
    else
    {
        p_row = -1;
        p_column = -1;
        oldAvrValue = -1;
    }

    newValue = value;

    oldValue = model->data(index,role);
}

void ChangeItemValueCommand::undo()
{
    if(!(p_row>=0 && p_column >=0))
    {
        TreeItem *item = model_->rootItem->child(row);
        result = model_->changeData(model_->createIndex(row,column,item),oldValue);
    }
    else
    {
        TreeItem *item = model_->rootItem->child(p_row)->child(row);
        result = model_->changeData(model_->createIndex(row,column,item),oldValue);

        if(column == 4 && oldAvrValue != -1 && result)
        {
            TreeItem *parent = model_->rootItem->child(p_row);
            result = model_->changeData(model_->createIndex(p_row,2,parent),oldAvrValue);
        }
    }
}

void ChangeItemValueCommand::redo()
{
    if(!(p_row>=0 && p_column >=0))
    {
        TreeItem *item = model_->rootItem->child(row);
        result = model_->changeData(model_->createIndex(row,column,item),newValue);
    }
    else
    {
        TreeItem *item = model_->rootItem->child(p_row)->child(row);
        result = model_->changeData(model_->createIndex(row,column,item),newValue);

        if(column == 4 && oldAvrValue != -1 && result)
        {
            TreeItem *parent = model_->rootItem->child(p_row);

            int newAvrValue = (double)(oldAvrValue*(parent->childCount()-1)-oldValue.toInt()+newValue.toInt())/(parent->childCount()-1);
            result = model_->changeData(model_->createIndex(p_row,2,parent),newAvrValue);
        }
    }
}

bool ChangeItemValueCommand::getResult() const
{
    return result;
}
