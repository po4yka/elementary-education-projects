#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "basicheaders.h"
#include "radixSort.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        fileOpen(new QtMaterialFlatButton),
        lsdSortChoose(new QtMaterialRadioButton),
        msdSortChoose(new QtMaterialRadioButton),
        stepImplSwitch(new QtMaterialCheckable),
        nextStep(new QtMaterialFlatButton),
        inputLine(new QtMaterialTextField),
        outputLine(new QTextEdit)
{
    // creates actual widget instances
    // from QtDesigner
    ui->setupUi(this);

    // working zone - main widget
    QWidget *canvas = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    canvas->setLayout(layout);

    // sets the stretch factor at position index
    // and aligning of main widget
    layout->setStretch(1, 2);
    layout->setAlignment(Qt::AlignLeft);

    // set main widget of window
    canvas->setStyleSheet("QWidget { background: white; }");
    setCentralWidget(canvas);

    // sort type buttons
    lsdSortChoose->setText("LSD sort");
    msdSortChoose->setText("MSD sort");
    fileOpen->setText("File");

    // next step button setup
    nextStep->setText("Next step");
    nextStep->applyPreset(Material::FlatPreset);

    // widget of sort button choose
    QWidget *buttonWidget = new QWidget();
    QGridLayout *buttonLayout = new QGridLayout();
    buttonWidget->setLayout(buttonLayout);

    layout->addWidget(buttonWidget);
    buttonLayout->addWidget(lsdSortChoose, 0, 0);
    buttonLayout->addWidget(msdSortChoose, 1, 0);

    // policy of sort choose buttons
    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::Maximum);
    policy.setVerticalPolicy(QSizePolicy::Minimum);
    buttonWidget->setSizePolicy(policy);

    stepImplSwitch->setText("Step by step implementation");

    QVBoxLayout *mainButtonGrid = new QVBoxLayout();
    mainButtonGrid->addWidget(buttonWidget);
    mainButtonGrid->addWidget(stepImplSwitch);
    mainButtonGrid->addWidget(nextStep);

    QSizePolicy allButtonsPolicy;
    allButtonsPolicy.setVerticalPolicy(QSizePolicy::Minimum);

    QWidget *allButtonWidget = new QWidget();
    allButtonWidget->setSizePolicy(allButtonsPolicy);
    allButtonWidget->setLayout(mainButtonGrid);

    // line for input
    inputLine->setLabel("Enter numbers for sorting (separate by spaces)");
    outputLine->setReadOnly(true);  // area for output
    QWidget *inOutAreas = new QWidget();
    QGridLayout *inOutAreasGrid = new QGridLayout();
    //inOutAreasGrid->addWidget(inputLine, 1, 0);
    QWidget *inArea = new QWidget();
    QHBoxLayout *inAreasGrid = new QHBoxLayout();
    inAreasGrid->addWidget(fileOpen);
    inAreasGrid->addWidget(inputLine);
    inArea->setLayout(inAreasGrid);
    inOutAreasGrid->addWidget(outputLine, 0, 0);
    inOutAreasGrid->addWidget(inArea, 1, 0);
    inOutAreas->setLayout(inOutAreasGrid);

    layout->addWidget(allButtonWidget);
    layout->addWidget(inOutAreas);

    outputLine->setTextColor(QColor::fromRgb(0, 0, 0));
    inputLine->setTextColor(QColor::fromRgb(0, 0, 0));

    // only numbers in input
    inputLine->setValidator(new QRegExpValidator(QRegExp("[0-9\\s]*"), inputLine));

    // disable step button then this option is off
    connect(stepImplSwitch, SIGNAL(toggled(bool)), this, SLOT(changeStepButtonState(bool)));
    stepImplSwitch->setChecked(false);
    nextStep->setDisabled(true);

    // disable step-by-step for MSD & enable for LSD
    connect(msdSortChoose, SIGNAL(toggled(bool)), this, SLOT(deactivateStepBox(bool)));
    connect(lsdSortChoose, SIGNAL(toggled(bool)), this, SLOT(activateStepBox(bool)));

    // line input connect
    connect(inputLine, SIGNAL(returnPressed()), this, SLOT(onEnterLine()));

    // open file button connect
    connect(fileOpen, SIGNAL(clicked()), this, SLOT(onFileChooseClicked()));
}

void MainWindow::changeStepButtonState(bool checked)
{
    Q_UNUSED(checked)
    if (stepImplSwitch->isChecked())
    {
        nextStep->setEnabled(true);
        nextStep->setDisabledBackgroundColor(false);
    } else
    {
        nextStep->setDisabledBackgroundColor(true);
        nextStep->setDisabled(true);
    }
}

void MainWindow::onFileChooseClicked()
{
    cleanOutLine();
    // true == lsd, false == msd
    bool sortType;
    if (lsdSortChoose->isChecked())
        sortType = true;
    else if (msdSortChoose->isChecked())
        sortType = false;
    else
        return;

    nextStep->setDisabled(false);

    // file open check
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open TXT File"),  QDir::homePath(),
            tr("TXT text (*.txt);;All Files (*)"));
    if (fileName == nullptr)
    {
        QMessageBox::warning(this,
                "Warning!",
                "Please choose text file for work.");
        return;
    }

    // check step implementation switcher
    bool stepFlag = stepImplSwitch->isChecked();

    workSort = new radixSort(sortType, stepFlag, false);

    /* PREPARE OBJECT OF RADIX SORT TO WORK */

    if(sortType == true)
    {
        // step-by-step of LSD sort :: deactivate NextButton
        connect(workSort, SIGNAL(sendDeactivateNextStepBut()), this, SLOT(deactivateNextStepBut()));

        // connect next button & next call of lsd sort
        connect(nextStep, SIGNAL(clicked()), workSort, SLOT(callWorkSortFunc()));
    }

    // printing res to view zone in window
    connect(workSort, SIGNAL(strToPrintInWindow(QString)), this, SLOT(printStrToWindow(QString)));

    workSort->implementForFile(fileName);
}

void MainWindow::onEnterLine()
{
    cleanOutLine();
    // true == lsd, false == msd
    bool sortType;
    if (lsdSortChoose->isChecked())
        sortType = true;
    else if (msdSortChoose->isChecked())
        sortType = false;
    else
        return;

    std::vector<int32_t> inpNumbers;
    QString input = inputLine->text();
    QTextStream stream(&input);
    foreach (QString i,QString(stream.readAll()).split(QRegExp("[ \n]"),QString::SkipEmptyParts))
    {
        inpNumbers.push_back(static_cast<int32_t>(i.toInt()));
    }

    // check step implementation switcher
    bool stepFlag = stepImplSwitch->isChecked();

    workSort = new radixSort(sortType, stepFlag, false);

    nextStep->setDisabled(false);

    if(sortType == true)
    {
        // step-by-step of LSD sort :: deactivate NextButton
        connect(workSort, SIGNAL(sendDeactivateNextStepBut()), this, SLOT(deactivateNextStepBut()));

        // connect next button & next call of lsd sort
        connect(nextStep, SIGNAL(clicked()), workSort, SLOT(callWorkSortFunc()));
    }

    // printing res to view zone in window
    connect(workSort, SIGNAL(strToPrintInWindow(QString)), this, SLOT(printStrToWindow(QString)));

    workSort->implementForLine(inpNumbers);
}

void MainWindow::deactivateNextStepBut()
{
    nextStep->setDisabled(true);
}

void MainWindow::deactivateStepBox(bool)
{
    stepImplSwitch->setEnabled(false);
    stepImplSwitch->setDisabled(true);
}

void MainWindow::activateStepBox(bool)
{
    stepImplSwitch->setEnabled(true);
    stepImplSwitch->setDisabled(false);
}

void MainWindow::printStrToWindow(QString str)
{
    outputLine->insertPlainText(str);
}

void MainWindow::cleanOutLine()
{
    outputLine->clear();
}

MainWindow::~MainWindow()
{
}
