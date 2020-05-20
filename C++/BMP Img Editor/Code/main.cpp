#include "universalqtheader.h"
#include "visualfunc.h"

int main(int argc, char *argv[])
{
    Q_UNUSED (argc)
    Q_UNUSED (argv)

    QApplication app(argc, argv);

    MainWindow w;
    connectVisualLib();
    w.show();

    return app.exec();
}
