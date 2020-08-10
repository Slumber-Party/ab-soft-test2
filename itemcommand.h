#ifndef ITEMCOMMAND_H
#define ITEMCOMMAND_H

#include <QUndoCommand>
#include <QModelIndex>

class TreeModel;

class ItemCommand : public QUndoCommand
{
public:
    ItemCommand(int position, int rows, const QModelIndex &parent, TreeModel *model, bool isDelete);

    void undo() override;
    void redo() override;
private:
    int row, p_row;
    int rows_;
    bool isDelete_;

    TreeModel *model_;
    QModelIndex ind;
};

#endif // ITEMCOMMAND_H
