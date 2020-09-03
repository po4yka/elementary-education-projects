#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "allheaders.h"
#include "customvector.h"

#include "test.h"
#include "HashTable.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),

        fileOpen(new QtMaterialFlatButton),
        runButton(new QtMaterialFlatButton),
        fileWay(new QLabel),
        findElInput(new QtMaterialTextField),
        findEl(new QtMaterialFlatButton),
        stepByStepSwitcher(new QtMaterialCheckBox),
        nextStep(new QtMaterialFlatButton),

        mainTextOutput(new QTextEdit),
        workTable(new lrstruct::HashTable<std::string>)
        {
            setUpUI();
        }

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onFileOpenButtonClicked() {
    qDebug() << "onFileOpenlicked();" << endl;

    std::fstream fs;
    fs.open(LOG_FILE_WAY, std::ios::out | std::ios::trunc);
    fs.close();

    fileWay->clear();

    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open TXT File"),  QDir::homePath(),
            tr("TXT text (*.txt);;All Files (*)"));

    if (fileName == nullptr) {
        qDebug() << "No file name" << endl;
        return;
    }

    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        foreach (QString i, QString(stream.readAll()).split(QRegExp("[\n]"), \
                                                            QString::SkipEmptyParts))
        inputStr.push_back(i.toUtf8().constData());
    }

    fileWay->setText(fileName);

    // working function activate
    findElInput->setDisabled(false);
    findEl->setDisabled(false);
    runButton->setDisabled(false);
    stepByStepSwitcher->setDisabled(false);

    connect(stepByStepSwitcher, SIGNAL(toggled(bool)), this, SLOT(changeNextStepButState()));
    connect(nextStep, SIGNAL(clicked()), workTable, SLOT(disableLoopLatency()));
    connect(workTable, SIGNAL(printOutput()), this, SLOT(outputInfo()));

    file.close();

    qDebug() << "End of onFileOpenlicked()" << endl;
}

void MainWindow::onRunButtonClicked() {
    qDebug() << "onRunButtonClicked();" << endl;

    mainTextOutput->clear();
    std::fstream fs;
    fs.open(LOG_FILE_WAY, std::ios::out | std::ios::trunc);
    fs << "LOG OF HASH TABLE WORK\n\n";
    fs.close();

    if(inputStr.empty()) {
        mainTextOutput->setText("No input data!");
        return;
    }

    Test::makeHashTable(inputStr, workTable);

    outputInfo();
}

void MainWindow::onFindElButtonClicked() {
    if(findElInput->text().isEmpty())
        return;

    std::string elFind = findElInput->text().toUtf8().constData();

    Test::findElement(workTable, elFind);

    outputInfo();
}

void MainWindow::outputInfo() {
    QFile file(LOG_FILE_WAY);
    file.open(QFile::ReadOnly);
    QString content = QString::fromUtf8(file.readAll());
    mainTextOutput->setPlainText(content);
    file.close();
}
