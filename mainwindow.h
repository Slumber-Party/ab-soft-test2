#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

#include <QUndoStack>

#include "itemcommand.h"

#include "treemodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Open_triggered();
    
    void on_Save_triggered();
    
    void on_Save_As_triggered();

    void on_addDepartment_triggered();

    void on_deleteDepartment_triggered();

    void on_addEmployment_triggered();

    void on_deleteEmployment_triggered();

private:
    void setActionsEnabled(bool isEnabled);

    TreeModel *getCurrentTreeModel() const;

    QUndoStack *undoStack;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
