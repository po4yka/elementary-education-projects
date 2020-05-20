#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "universalqtheader.h"
#include "stringchecker.h"

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

    // making log file in path of pr
    QFile log("log.txt");
    log.open(QIODevice::ReadWrite);
    QTextStream logStream(&log);

    // only reading and interpretation as a text
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        fileString = stream.readLine();
    }

    file.close();

    std::string inputString = fileString.toUtf8().constData();

    checkString(inputString, logStream);

    log.close();
}

void MainWindow::on_inputString_clicked()
{
    // convert QString to srd::string, using UTF-8
    std::string inputString = \
            ui->inputLine->text().toUtf8().constData();
    if (inputString.empty()) {
        QMessageBox::warning(this,
        "Warning!",
        "Please input the string.");
        return;
    }

    // making log file in path of pr
    QString logFilePath = QDir::currentPath();
    logFilePath.chop(logFilePath.length() - \
                     logFilePath.lastIndexOf(QChar('/')));
    QFile log(logFilePath + "/log.txt");
    log.open(QIODevice::ReadWrite | \
             QIODevice::Truncate | QIODevice::Text);
    QTextStream logStream(&log);

    checkString(inputString, logStream);

    log.close();
}
