#include "mainwindow.h"
#include <QApplication>

// main function
int main(int arga, char *argv[])
{
    QApplication app(arga, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
