#ifndef CHANGEITEMVALUECOMMAND_H
#define CHANGEITEMVALUECOMMAND_H

#include <QUndoCommand>
#include <QModelIndex>

class TreeModel;

class ChangeItemValueCommand : public QUndoCommand
{
public:
    ChangeItemValueCommand();

    void undo() override;
    void redo() override;
private:

};

#endif // CHANGEITEMVALUECOMMAND_H
