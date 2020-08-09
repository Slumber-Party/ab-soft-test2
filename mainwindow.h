#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

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
    
    void on_Undo_triggered();
    
    void on_Redo_triggered();
    
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
