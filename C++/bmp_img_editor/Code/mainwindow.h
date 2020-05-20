#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>

#include "bmpimage.h"
#include "imgedit.h"
#include "customgraphicsview.h"
#include "fileinfo.h"

// operation codes
#define REFLECTION 1
#define COPY_ZONE 2
#define COPY_POINT 3
#define RECT 4

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    CustomGraphicsView *picture;
    int flag;
    int flag_fill;
    QColor color_split_line;
    QColor color_to_change;
    QColor color_on_which_change;
    QColor color_of_rect;
    QColor color_of_fill_rect;
    int thick_line_split;

private slots:

    void on_actionFileInf_triggered();
    void on_actionExit_triggered();
    void on_actionQt_triggered();
    void on_actionAuthor_triggered();

    void on_actionBinCopy_triggered();
    void on_butOpen_clicked();

    void on_butSave_clicked();

    void selection();
    void click_coords();

    void on_butRefl_clicked();

    void on_butCollage_clicked();

    void on_butSplit_clicked();

    void on_butChange_clicked();

    void on_butBlackAndWhite_clicked();

    void on_butCopy_clicked();

    void on_butRotate_clicked();

    void on_butRect_clicked();

    void on_butFill_clicked();

private:
    Ui::MainWindow *ui;
    QImage  *imageObject;
    ImgEdit *imgEdit;
    QLabel *_label;
    QLabel *imgDisplayLabel;
    int button_pressed;
    fileinfo *information;

};

#endif // MAINWINDOW_H
