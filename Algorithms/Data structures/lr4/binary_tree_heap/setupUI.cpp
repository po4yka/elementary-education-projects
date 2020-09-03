#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "basicheaders.h"

void MainWindow::setUpUI() {
    qDebug() << "UI set up started" << endl;

    // creates actual widget instances
    // from QtDesigner
    ui->setupUi(this);

    // working zone - main widget
    QWidget *canvas = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    canvas->setLayout(layout);

    // sets the stretch factor at position index
    // and aligning of main widget
    layout->setStretch(1, 2);
    layout->setAlignment(Qt::AlignCenter);

    // set main widget of window
    canvas->setStyleSheet("QWidget { background: white; }");
    setCentralWidget(canvas);

    // set up table of values and conditions
    QGridLayout *valuesInfoLayout = new QGridLayout();

    isBstTree->setStyleSheet("color: black;");
    labelIsBsdTree->setStyleSheet("color: black;");
    isBinHeap->setStyleSheet("color: black;");
    labelIsBinHeap->setStyleSheet("color: black;");
    maxNode->setStyleSheet("color: black;");
    labelMaxNode->setStyleSheet("color: black;");
    minNode->setStyleSheet("color: black;");
    labelMinNode->setStyleSheet("color: black;");

    valuesInfoLayout->addWidget(labelIsBsdTree, 0, 0, Qt::AlignLeft);
    valuesInfoLayout->addWidget(isBstTree, 0, 1, Qt::AlignHCenter);
    valuesInfoLayout->addWidget(labelIsBinHeap, 1, 0, Qt::AlignLeft);
    valuesInfoLayout->addWidget(isBinHeap, 1, 1, Qt::AlignHCenter);
    valuesInfoLayout->addWidget(labelMaxNode, 2, 0, Qt::AlignLeft);
    valuesInfoLayout->addWidget(maxNode, 2, 1, Qt::AlignHCenter);
    valuesInfoLayout->addWidget(labelMinNode, 3, 0, Qt::AlignLeft);
    valuesInfoLayout->addWidget(minNode, 3, 1, Qt::AlignHCenter);

    labelIsBsdTree->setText("Is BST tree: ");
    labelIsBinHeap->setText("Is Bin Heap: ");
    labelMaxNode->setText("Max node: ");
    labelMinNode->setText("Min node: ");

    // default parameters
    isBstTree->setText("-");
    isBinHeap->setText("-");
    maxNode->setText("-");
    minNode->setText("-");

    // set up steps buttons and check switchers
    stepImplSwitch->setText("Step mode");
    nextStep->setText("Next step");
    checkBst->setText("Check BST");
    checkBinHeap->setText("Check Bin heap");
    QGridLayout *stepsButtonsLayout = new QGridLayout();
    stepsButtonsLayout->addWidget(stepImplSwitch, 0, 0, Qt::AlignHCenter);
    stepsButtonsLayout->addWidget(nextStep, 1, 0, Qt::AlignCenter);
    stepsButtonsLayout->addWidget(checkBst, 2, 0, Qt::AlignLeft);
    stepsButtonsLayout->addWidget(checkBinHeap, 3, 0, Qt::AlignLeft);

    // setup input line
    inputLine->setLabel("Enter binary tree in bracket representation:");
    inputLine->setTextColor(QColor::fromRgb(0, 0, 0));
    inputLine->setValidator(new QRegExpValidator(QRegExp("[0-9()\\s]*"), inputLine));

    //  set up full buttons layout
    runButton->setText("Run");
    fileOpen->setText("Open file");
    QHBoxLayout *allButtonsLayout = new QHBoxLayout();
    allButtonsLayout->addWidget(runButton);
    allButtonsLayout->addWidget(fileOpen);
    allButtonsLayout->addWidget(inputLine);
    allButtonsLayout->addLayout(stepsButtonsLayout);
    allButtonsLayout->addLayout(valuesInfoLayout);

    // main layout setup
    layout->addLayout(allButtonsLayout);
    layout->addWidget(mainGraphicsView);

    // default conditions
    nextStep->setDisabled(true);
    stepImplSwitch->setChecked(false);
    checkBst->setEnabled(false);
    checkBinHeap->setEnabled(false);
    _root = nullptr;

    /* SIGNALS AND SLOTS CONNECTIONS */
    connect(stepImplSwitch, SIGNAL(toggled(bool)), this, SLOT(changeActivitiStepBut(bool)));
    connect(runButton, SIGNAL(clicked()), this, SLOT(onRunButtonClicked()));
    connect(fileOpen, SIGNAL(clicked()), this, SLOT(onFileOpenButtonClicked()));
}
