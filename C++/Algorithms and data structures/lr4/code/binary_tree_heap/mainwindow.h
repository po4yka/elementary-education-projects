#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "basicheaders.h"
#include "customvector.h"
#include "tree.h"

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // STATE FLAGS
    bool checkType = true;  // default is BST check

public slots:
    void changeActivitiStepBut(bool);
    void onRunButtonClicked();
    void onFileOpenButtonClicked();
    void highlightCurrNode(lrstruct::Node*, int);

private:
    void setUpUI();

    lrstruct::Node* _root;

    Ui::MainWindow *ui;
    bool DrawTree{false};

    // WORK BUTTONS
    QtMaterialFlatButton *fileOpen;
    QtMaterialTextField *inputLine;
    QtMaterialFlatButton *runButton;
    QtMaterialCheckable *stepImplSwitch;
    QtMaterialFlatButton *nextStep;
    QtMaterialRadioButton *checkBst;
    QtMaterialRadioButton *checkBinHeap;

    // INFORMATION LABELS
    QLabel *isBstTree;
    QLabel *labelIsBsdTree;
    QLabel *isBinHeap;
    QLabel *labelIsBinHeap;
    QLabel *maxNode;
    QLabel *labelMaxNode;
    QLabel *minNode;
    QLabel *labelMinNode;

    // DRAWING
    QGraphicsView *mainGraphicsView;
    // correct, cuz there's the only one scene + view
    QGraphicsScene *mainGraphicsScene;
    QPen pen;
    QColor color;
    QBrush brush;
    QFont font;
    void setUpDrawingTools(QColor&);
    void mainGraph();
};

#endif // MAINWINDOW_H
