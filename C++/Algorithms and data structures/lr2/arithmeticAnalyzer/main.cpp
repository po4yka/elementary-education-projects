#include "mainwindow.h"
#include "visualfunc.h"
#include "basicqtheader.h"
#include "basicheaders.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    connectVisualLib();
    w.show();

    return a.exec();
}
