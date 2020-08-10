#include "deleteitemcommand.h"
#include "treemodel.h"

DeleteItemCommand::DeleteItemCommand(int position, int rows, const QModelIndex &parent, TreeModel *model)
{
    row = position;
    rows_ = rows;
    ind = parent;
    model_ = model;

    p_row = parent.row();
    p_column = parent.column();

    TreeItem *itemToDelete = model->getItem(parent)->child(position);
    for(int i=0;i<model->columnCount();i++)
    {
        oldParentData << itemToDelete->data(i); //сохраняем данные удаляемого элемента
    }

    if(itemToDelete->childCount() <= 1)
        return;

    children = QVector<TreeItem>(itemToDelete->childCount());

    for(int i = 1; i<itemToDelete->childCount();i++)
    {
        children[i] = *itemToDelete->child(i); // сохраняем все подэлементы
    }
}

void DeleteItemCommand::undo()
{
    if(!ind.isValid())
        model_->insertRows(row,rows_);
    else
        model_->insertRows(row,rows_,model_->index(p_row,p_column));

    if(children.isEmpty()) //если true -> восстанавливаем employment, если нет - department
    {
        for(int i =0; i< oldParentData.size();i++)
        {
            QModelIndex insertedInd = model_->index(row,i,model_->index(p_row,p_column));
            model_->changeData(insertedInd,oldParentData[i]);
        }

        return;
    }

    QModelIndex currentParentIndx = model_->index(row,0,ind);
    model_->changeData(currentParentIndx,oldParentData[0]);

    model_->insertRows(1,children.count()-1,model_->index(row,0,ind));

    for(int i =1;i<children.size();i++)
    {
        for(int j=0;j<model_->columnCount();j++)
        {
            QModelIndex childInd = model_->index(i,j,currentParentIndx);
            model_->changeData(childInd,children[i].data(j));
        }
    }
}

void DeleteItemCommand::redo()
{
    if(ind.isValid())
        model_->removeRows(row,rows_,model_->index(p_row,p_column));
    else
        model_->removeRows(row,rows_);
}
