#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "basicqtheader.h"
#include "workingfunc.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fileChoose_clicked()
{
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

    QString logFilePath = "/";

    QFile file(fileName);
    QString fileString;

    // создание и оглавление log файла
    QFile log("log.txt");
    log.open(QIODevice::ReadWrite);
    QTextStream logStream(&log);
    logStream << "************\n";
    logStream << "* LOG FILE *\n";
    logStream << "************\n\n";
    log.close();

    // чтение и интерпретация в виде текста
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        fileString = stream.readLine();
    }

    file.close();

    std::string eqInputLine = fileString.toUtf8().constData();

    // TODO: CALLFUNC
}

void MainWindow::on_inputString_clicked()
{
    // convert QString to std::string, using UTF-8
    std::string eqInputLine = \
            ui->eqInputLine->text().toUtf8().constData();
    if (eqInputLine.empty()) {
        QMessageBox::warning(this,
        "Warning!",
        "Please input the expression.");
        return;
    }

    std::string varInputLine = \
            ui->variableInputLine->text().toUtf8().constData();

    QFile log("../log.txt");
    log.open(QIODevice::ReadWrite | \
             QIODevice::Truncate | QIODevice::Text);
    QTextStream logStream(&log);
    logStream << "************\n";
    logStream << "* LOG FILE *\n";
    logStream << "************\n\n";
    log.close();

    mainWorkFunc(eqInputLine, varInputLine);
}
