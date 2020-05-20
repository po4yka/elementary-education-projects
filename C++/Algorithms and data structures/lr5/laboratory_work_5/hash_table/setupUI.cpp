#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "allheaders.h"
#include "test.h"

void MainWindow::setUpUI() {
    qDebug() << "UI set up started" << endl;

    // working zone - main widget
    QWidget *canvas = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    // set main vertical layout
    canvas->setLayout(layout);

    // sets the stretch factor at position index
    // and aligning of main widget
    layout->setStretch(1, 2);
    layout->setAlignment(Qt::AlignCenter);

    // set main widget of window
    canvas->setStyleSheet("QWidget { background: white; }");
    setCentralWidget(canvas);

    //  set up full buttons layout
    runButton->setText("Run");
    fileOpen->setText("Open file");
    findEl->setText("Find");
    findElInput->setLabel("Element to find");
    nextStep->setText("Next");
    fileWay->setStyleSheet("QLabel {color : black; }");
    mainTextOutput->setStyleSheet("QWidget {color : black; }");
    findElInput->setStyleSheet("QWidget {color : black; }");
    findElInput->setMinimumWidth(150);

    fileWay->setAlignment(Qt::AlignCenter);
    QHBoxLayout *allButtonsLayout = new QHBoxLayout();
    QGridLayout *workingBut = new QGridLayout();
    workingBut->addWidget(runButton, 0, 0, Qt::AlignLeft);
    workingBut->addWidget(fileOpen, 0, 1, Qt::AlignCenter);
    workingBut->addWidget(stepByStepSwitcher, 0, 2, Qt::AlignCenter);
    workingBut->addWidget(nextStep, 0, 3, Qt::AlignRight);
    workingBut->addWidget(fileWay, 1, 0, 1, 3, Qt::AlignLeft);
    QHBoxLayout *findElLayout = new QHBoxLayout();
    findElLayout->addWidget(findEl);
    findElLayout->addWidget(findElInput);
    allButtonsLayout->addLayout(workingBut);
    allButtonsLayout->addLayout(findElLayout);

    // main layout setup
    layout->addLayout(allButtonsLayout);
    layout->addWidget(mainTextOutput);
    mainTextOutput->setReadOnly(true);

    // default conditions
    findElInput->setDisabled(true);
    findEl->setDisabled(true);
    runButton->setDisabled(true);
    nextStep->setDisabled(true);
    stepByStepSwitcher->setDisabled(true);

    fileWay->setText("Please, open the file");

    /* SIGNALS AND SLOTS CONNECTIONS */
    connect(runButton, SIGNAL(clicked()), this, SLOT(onRunButtonClicked()));
    connect(fileOpen, SIGNAL(clicked()), this, SLOT(onFileOpenButtonClicked()));
    connect(findEl, SIGNAL(clicked()), this, SLOT(onFindElButtonClicked()));
}

void MainWindow::changeNextStepButState() {
    if(stepByStepSwitcher->isChecked() == true) {
        nextStep->setDisabled(false);
        workTable->enableLoopLatency();
        workTable->setStepByStepMode(true);
    }
    else {
        nextStep->setDisabled(true);
        workTable->disableLoopLatency();
        workTable->setStepByStepMode(false);
    }
    QFile file(LOG_FILE_WAY);
    file.open(QFile::ReadOnly);
    QString content = QString::fromUtf8(file.readAll());
    mainTextOutput->clear();
    mainTextOutput->setPlainText(content);
    file.close();
}
