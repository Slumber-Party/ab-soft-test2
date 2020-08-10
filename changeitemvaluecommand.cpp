#include "changeitemvaluecommand.h"
#include "treemodel.h"

ChangeItemValueCommand::ChangeItemValueCommand(const QModelIndex &index, const QVariant &value, int role, TreeModel *model)
{
    ind = index;
    role_ = role;
    model_ = model;

    if(index.parent().isValid())
    {
        p_row = index.parent().row();
        p_column = index.parent().column();
    }

    newValue = value;

    oldValue = model->data(index,role);
}

void ChangeItemValueCommand::undo()
{
    //ind не гарантирует иметь валидный указатель на parent => используем только row, column, а parent index получаем через model->index
    //такое поведение реализовано во всех командах
    //если же ind изначально не валидный, то передаем его же, т.к он будет равен QModelIndex()
    if(!ind.parent().isValid())
        result = model_->changeData(ind,oldValue,role_);
    else
        result = model_->changeData(model_->index(ind.row(),ind.column(),model_->index(p_row,p_column)),oldValue,role_);
}

void ChangeItemValueCommand::redo()
{
    if(!ind.parent().isValid())
        result = model_->changeData(ind,newValue,role_);
    else
        result = model_->changeData(model_->index(ind.row(),ind.column(),model_->index(p_row,p_column)),newValue,role_);
}

bool ChangeItemValueCommand::getResult() const
{
    return result;
}
