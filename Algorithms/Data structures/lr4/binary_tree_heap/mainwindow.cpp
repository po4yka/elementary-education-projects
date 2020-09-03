#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "basicheaders.h"
#include "customvector.h"
#include "tree.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),

        fileOpen(new QtMaterialFlatButton),
        inputLine(new QtMaterialTextField),
        runButton(new QtMaterialFlatButton),
        stepImplSwitch(new QtMaterialCheckable),
        nextStep(new QtMaterialFlatButton),
        checkBst(new QtMaterialRadioButton),
        checkBinHeap(new QtMaterialRadioButton),

        isBstTree(new QLabel),
        labelIsBsdTree(new QLabel),
        isBinHeap(new QLabel),
        labelIsBinHeap(new QLabel),
        maxNode(new QLabel),
        labelMaxNode(new QLabel),
        minNode(new QLabel),
        labelMinNode(new QLabel),

        mainGraphicsView(new QGraphicsView),
        mainGraphicsScene(new QGraphicsScene)
{
    setUpUI();

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::changeActivitiStepBut(bool state) {
    if(state == false) {
        nextStep->setDisabled(true);
        checkBst->setEnabled(false);
        checkBinHeap->setEnabled(false);
    } else {
        nextStep->setDisabled(false);
        checkBst->setEnabled(true);
        checkBinHeap->setEnabled(true);
    }
}

void MainWindow::onFileOpenButtonClicked() {
    qDebug() << "onFileOpenlicked();" << endl;

    std::string inputStr;
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open TXT File"),  QDir::homePath(),
            tr("TXT text (*.txt);;All Files (*)"));
    if (fileName == nullptr)
    {
        qDebug() << "No file name" << endl;
        return;
    }
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        foreach (QString i ,QString(stream.readAll()).split(QRegExp("[ \t]"), \
                                                            QString::SkipEmptyParts))
        inputStr.append(i.toUtf8().constData());
    }

    if(inputStr.empty())
        return;

    file.close();

    inputLine->setText(QString::fromUtf8(inputStr.c_str()));

    qDebug() << "End of onFileOpenlicked()" << endl;
}
