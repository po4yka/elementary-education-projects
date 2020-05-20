#ifndef VISUALFUNC_CPP
#define VISUALFUNC_CPP
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <iostream>

void connectVisualLib()
{
    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        std::cout << "Unable to set stylesheet, file not found\n";
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
}
#endif // VISUALFUNC_CPP
