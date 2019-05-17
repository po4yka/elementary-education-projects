#ifndef FILEINFO_H
#define FILEINFO_H

#include <QDialog>

typedef struct Information{
  QString name;
  //int version;
  int hight;
  int width;
  int size;
  int version;
  bool is_readable;
  bool is_writeable;
}Information;

namespace Ui {
class fileinfo;
}

class fileinfo : public QDialog
{
    Q_OBJECT

public:
    explicit fileinfo(QWidget *parent = nullptr);
    ~fileinfo();
    Information info;
    void write_information();

private slots:
    void on_pushButton_clicked();

private:
    Ui::fileinfo *ui;
};

#endif // FILEINFO_H
