#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "console.h"
#include "basicheaders.h"
#include "customvector.h"

int main(int argc, char *argv[]) {
    bool console = false;
    Console *workC = new Console();
    if(DEBUG) std:: cout << "Main function started" << std::endl;

    for(int i = 0; i < argc; i++) {
        if(!strcmp("console", argv[i]) || !strcmp("-console", argv[i]) \
                || !strcmp("-c", argv[i])) {
            if(DEBUG) std:: cout << "flag was found" << std::endl;
            console = true;
            break;
        }
    }
    if (console) {
        if(DEBUG) std:: cout << "flag was read" << std::endl;
        workC->consoleExec();
    }
    else {
        Q_INIT_RESOURCE(resources);

        QApplication a(argc, argv);

        MainWindow window;
        window.show();

        return a.exec();
    }

    return 0;
}
