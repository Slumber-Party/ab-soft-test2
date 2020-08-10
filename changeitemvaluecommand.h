#ifndef CHANGEITEMVALUECOMMAND_H
#define CHANGEITEMVALUECOMMAND_H

#include <QUndoCommand>
#include <QModelIndex>

class TreeModel;

class ChangeItemValueCommand : public QUndoCommand
{
public:
    ChangeItemValueCommand(const QModelIndex &index, const QVariant &value, int role, TreeModel *model);

    void undo() override;
    void redo() override;

    bool getResult() const;
private:
    int role_;
    int p_row,p_column;

    QModelIndex ind;
    QVariant oldValue,newValue;

    bool result;

    TreeModel *model_;
};

#endif // CHANGEITEMVALUECOMMAND_H
