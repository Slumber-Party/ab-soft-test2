#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    makeNewModel();
}

MainWindow::~MainWindow()
{
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

    model = makeNewModel();

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

    setActionsEnabled(true);
}

void MainWindow::on_Save_triggered()
{
    TreeModel *model = getCurrentTreeModel();

    if(model!= nullptr && !model->saveToXML())
    {
        QMessageBox::warning(this,"Error", "Can't save the model");
    }
}

void MainWindow::on_Save_As_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this);

    TreeModel *model = getCurrentTreeModel();

    if(model!= nullptr && !model->saveToXML(filePath))
    {
        QMessageBox::warning(this,"Error", "Can't save the model");
    }

}

void MainWindow::on_addDepartment_triggered()
{
    TreeModel *model = getCurrentTreeModel();
    model->insertRows(0,1,model->parent(model->index(0,0)));
}

void MainWindow::on_deleteDepartment_triggered()
{

}

void MainWindow::on_addEmployment_triggered()
{

}

void MainWindow::on_deleteEmployment_triggered()
{

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

TreeModel *MainWindow::makeNewModel()
{
    TreeModel *model = new TreeModel({"Наименование подразделения","Количество сотрудников","Средняя зарплата","",""});

    QAction *redo = model->undoStack()->createRedoAction(ui->edit_menu);
    redo->setText("Повторить действие");
    QAction *undo = model->undoStack()->createUndoAction(ui->edit_menu);
    undo->setText("Отменить действие");

    for(auto action : ui->edit_menu->actions())
    {
        if(action->text() == "Повторить действие" || action->text() == "Отменить действие")
        {
            ui->edit_menu->removeAction(action);
        }
    }

    ui->edit_menu->addActions({redo,undo});

    return model;
}
