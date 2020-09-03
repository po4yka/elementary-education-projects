#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "allheaders.h"
#include "customvector.h"
#include "HashTable.h"

#include <qtmaterialslider.h>
#include <qtmaterialradiobutton.h>
#include <qtmaterialtoggle.h>
#include <qtmaterialtextfield.h>
#include <qtmaterialscrollbar.h>
#include <qtmaterialflatbutton.h>
#include <qtmaterialdialog.h>
#include <qtmaterialcheckbox.h>
#include "lib/qtmaterialstyle.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onRunButtonClicked();
    void onFileOpenButtonClicked();
    void onFindElButtonClicked();
    void changeNextStepButState();
    void outputInfo();

private:
    void setUpUI();

    Ui::MainWindow *ui;

    // WORK BUTTONS
    QtMaterialFlatButton *fileOpen;
    QtMaterialFlatButton *runButton;
    QLabel *fileWay;
    QtMaterialTextField *findElInput;
    QtMaterialFlatButton *findEl;
    QtMaterialCheckBox *stepByStepSwitcher;
    QtMaterialFlatButton *nextStep;

    QTextEdit *mainTextOutput;
    lrstruct::Vector<std::string> inputStr;

    lrstruct::HashTable<std::string> *workTable;
};

#endif // MAINWINDOW_H
