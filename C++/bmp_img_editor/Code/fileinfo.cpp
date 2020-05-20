#include "fileinfo.h"
#include "ui_fileinfo.h"

fileinfo::fileinfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fileinfo)
{
    ui->setupUi(this);
}

fileinfo::~fileinfo()
{
    delete ui;
}

void fileinfo::write_information()
{
    ui->name->setText(info.name.toLocal8Bit().constData());
    ui->size->setText(QString::number(info.size));
    ui->width->setText(QString::number(info.width));
    ui->hight->setText(QString::number(info.hight));
    if (info.is_readable == true)
        ui->isreadable->setText("Yes");
    else
        ui->isreadable->setText("No");
    if (info.is_writeable == true)
        ui->iswrittable->setText("Yes");
    else
        ui->iswrittable->setText("No");
}

void fileinfo::on_pushButton_clicked()
{
    this->close();
}
