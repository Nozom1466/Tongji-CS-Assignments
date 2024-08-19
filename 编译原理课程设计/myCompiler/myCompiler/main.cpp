#include "CCompiler.h"
#include <QtWidgets/QApplication>
#include<thread>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::processEvents();
    myCompiler w;
    w.show();
    return a.exec();
}
