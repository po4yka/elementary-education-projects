#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "basicheaders.h"
#include "radixSort.h"

#include <qtmaterialslider.h>
#include <qtmaterialradiobutton.h>
#include <qtmaterialtoggle.h>
#include <qtmaterialtextfield.h>
#include <qtmaterialscrollbar.h>
#include <qtmaterialflatbutton.h>
#include <qtmaterialdialog.h>
#include "lib/qtmaterialstyle.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void changeStepButtonState(bool);
    void onFileChooseClicked();
    void onEnterLine();
    void deactivateNextStepBut();
    void deactivateStepBox(bool);
    void activateStepBox(bool);
    void printStrToWindow(QString);

private:
    void cleanOutLine();
    Q_OBJECT
    Ui::MainWindow *ui;
    QtMaterialFlatButton *fileOpen;
    QtMaterialRadioButton *lsdSortChoose;
    QtMaterialRadioButton *msdSortChoose;
    QtMaterialCheckable *stepImplSwitch;
    QtMaterialFlatButton *nextStep;
    QtMaterialTextField *inputLine;
    QTextEdit *outputLine;

    radixSort *workSort;
};

#endif // MAINWINDOW_H
