#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    TreeModel *model = new TreeModel({"Наименование подразделения","Количество сотрудников","Средняя зарплата","",""});
    if(model->setupFromXML(fileName))
    {
        ui->treeView->setModel(model);
        for (int column = 0; column < model->columnCount()-2; ++column)
            ui->treeView->resizeColumnToContents(column);
    }
    else
    {
        QMessageBox::warning(this,"Error", "Can't setup model");
    }
}

void MainWindow::on_Save_triggered()
{

}

void MainWindow::on_Save_As_triggered()
{

}

void MainWindow::on_Undo_triggered()
{

}

void MainWindow::on_Redo_triggered()
{

}
