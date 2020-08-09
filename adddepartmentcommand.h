#ifndef ADDDEPARTMENTCOMMAND_H
#define ADDDEPARTMENTCOMMAND_H

#include <QUndoCommand>
#include <QModelIndex>

class TreeModel;

class AddDepartmentCommand : public QUndoCommand
{
public:
    AddDepartmentCommand(int position, const QModelIndex &parent, TreeModel *model, bool &result);

    void undo() override;
    void redo() override;
private:
    int position_;
    TreeModel *model_;
    const QModelIndex &indexParent;
};

#endif // ADDDEPARTMENTCOMMAND_H
