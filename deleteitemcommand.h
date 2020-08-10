#ifndef ITEMCOMMAND_H
#define ITEMCOMMAND_H

#include <QUndoCommand>
#include <QModelIndex>

class TreeModel;
class TreeItem;

class DeleteItemCommand : public QUndoCommand
{
public:
    DeleteItemCommand(int position, int rows, const QModelIndex &parent, TreeModel *model);
    void undo() override;
    void redo() override;
private:
    int row;
    int rows_;

    int p_row,p_column;

    QVector<QVariant> oldParentData;
    QVector<TreeItem> children;

    TreeModel *model_;
    QModelIndex ind;
};

#endif // ITEMCOMMAND_H
