#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "allheaders.h"
#include "customvector.h"
#include "test.h"

int main(int argc, char *argv[]) {

    // force initialization of library resources
    Q_INIT_RESOURCE(resources);

    QApplication a(argc, argv);

    MainWindow window;

    // set up full screen view
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    window.resize(static_cast<int>(0.5 * width), static_cast<int>(0.5 * height));

    window.show();

    return a.exec();
}
