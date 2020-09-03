#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>

#include "universalqtheader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_fileChoose_clicked();

    void on_inputString_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
