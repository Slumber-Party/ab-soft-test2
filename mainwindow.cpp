#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    undoStack = new QUndoStack();
    undoStack->setUndoLimit(64);
    QAction *redo = undoStack->createRedoAction(ui->edit_menu,"Повторить действие");
    QAction *undo = undoStack->createUndoAction(ui->edit_menu,"Отменить действие");

    for(auto action : ui->edit_menu->actions())
    {
        if(action->text() == "Повторить действие" || action->text() == "Отменить действие")
        {
            ui->edit_menu->removeAction(action);
        }
    }

    ui->edit_menu->addActions({redo,undo});
}

MainWindow::~MainWindow()
{
    delete undoStack;
    delete ui;
}


void MainWindow::on_Open_triggered()
{
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter("Xml Files (*.xml)");

    QString fileName;
    if(dialog.exec())
    {
        fileName = dialog.selectedFiles().first();
    }
    else
    {
        return;
    }

    ui->treeView->reset();

    TreeModel *model = getCurrentTreeModel();
    if(model != nullptr)
    {
        delete model;
    }

    undoStack->setClean();
    model = new TreeModel({"Наименование подразделения","Количество сотрудников","Средняя зарплата","",""});

    if(model->setupFromXML(fileName))
    {
        ui->treeView->setModel(model);
        for (int column = 0; column < model->columnCount()-2; ++column)
            ui->treeView->resizeColumnToContents(column);
        ui->treeView->header()->setSectionsMovable(false);
    }
    else
    {
        setActionsEnabled(false);
        QMessageBox::warning(this,"Error", "Can't setup the model");
        return;
    }
\
    ui->treeView->setCurrentIndex(QModelIndex());
    setActionsEnabled(true);
}

void MainWindow::on_Save_triggered()
{
    TreeModel *model = getCurrentTreeModel();

    if(model!= nullptr && !model->saveToXML())
    {
        QMessageBox::warning(this,"Error", "Can't save the model");
    }

    undoStack->setClean();
}

void MainWindow::on_Save_As_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this);

    TreeModel *model = getCurrentTreeModel();

    if(model!= nullptr && !model->saveToXML(filePath))
    {
        QMessageBox::warning(this,"Error", "Can't save the model");
    }

    undoStack->setClean();
}

void MainWindow::on_addDepartment_triggered()
{
    TreeModel *model = getCurrentTreeModel();

    undoStack->push(new ItemCommand(0,1,model->parent(model->index(0,0)),model,false));

//    if(!model->insertRows(0,1,model->parent(model->index(0,0))))
//    {
//        QMessageBox::warning(this,"Ошибка","Не удалось добавить департамент");
//    }
}

void MainWindow::on_deleteDepartment_triggered()
{
    QModelIndex selectedIndex = ui->treeView->currentIndex();
    if(!selectedIndex.isValid() || selectedIndex.parent().isValid())
    {
        QMessageBox::warning(this,"Ошибка","Выберите департамент для удаления");
        return;
    }

    TreeModel *model = getCurrentTreeModel();
    undoStack->push(new ItemCommand(selectedIndex.row(),1,selectedIndex.parent(),model,true));
//    if(!model->removeRows(selectedIndex.row(),1,selectedIndex.parent()))
//    {
//        QMessageBox::warning(this,"Ошибка","Не удалось удалить департамент");
//    }
}

void MainWindow::on_addEmployment_triggered()
{
    QModelIndex selectedIndex = ui->treeView->currentIndex();

    if(!selectedIndex.isValid() || selectedIndex.parent().isValid())
    {
        QMessageBox::warning(this,"Ошибка","Выберите департамент для добавления сотрудника");
        return;
    }

    TreeModel *model = getCurrentTreeModel();
    undoStack->push(new ItemCommand(1,1,selectedIndex,model,false));
//    if(!model->insertRows(1,1,selectedIndex))
//    {
//        QMessageBox::warning(this,"Ошибка","Не удалось добавить сотрудника");
//    }
}

void MainWindow::on_deleteEmployment_triggered()
{
    QModelIndex selectedIndex = ui->treeView->currentIndex();

    qDebug() << selectedIndex.isValid() << " " << selectedIndex.parent().isValid() << " " << selectedIndex.parent().parent().isValid();
    if(!selectedIndex.isValid() || !(selectedIndex.parent().isValid() && !selectedIndex.parent().parent().isValid()))
    {
        QMessageBox::warning(this,"Ошибка","Выберите сотрудника для удаления");
        return;
    }

    TreeModel *model = getCurrentTreeModel();
    undoStack->push(new ItemCommand(selectedIndex.row(),1,selectedIndex.parent(),model,true));
//    if(!model->removeRows(selectedIndex.row(),1,selectedIndex.parent()))
//    {
//        QMessageBox::warning(this,"Ошибка","Не удалось удалить сотрудника");
//    }
}

void MainWindow::setActionsEnabled(bool isEnabled)
{
    ui->Save->setEnabled(isEnabled);
    ui->Save_As->setEnabled(isEnabled);
    ui->addDepartment->setEnabled(isEnabled);
    ui->deleteDepartment->setEnabled(isEnabled);
    ui->addEmployment->setEnabled(isEnabled);
    ui->deleteEmployment->setEnabled(isEnabled);
}

TreeModel *MainWindow::getCurrentTreeModel() const
{
    return dynamic_cast<TreeModel*>(ui->treeView->model());
}
