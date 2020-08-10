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
    int row,column;

    QVariant oldValue,newValue;
    int oldAvrValue;

    bool result;

    TreeModel *model_;
};

#endif // CHANGEITEMVALUECOMMAND_H
