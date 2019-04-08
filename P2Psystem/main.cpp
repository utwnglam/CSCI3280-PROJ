#include "mainwindow.h"
#include <QApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle("fusion");
    server s;
    s.StartServer();

    MainWindow w;
    w.show();

    return a.exec();
}
