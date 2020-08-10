#ifndef ADDITEMCOMMAND_H
#define ADDITEMCOMMAND_H

#include <QUndoCommand>
#include <QModelIndex>

class TreeModel;

class AddItemCommand : public QUndoCommand
{
public:
    AddItemCommand(int position, int rows, const QModelIndex &parent, TreeModel *model);

    void undo() override;
    void redo() override;
private:
    int row;
    int rows_;

    int p_row,p_column;

    TreeModel *model_;
    QModelIndex ind;
};

#endif // ADDITEMCOMMAND_H
