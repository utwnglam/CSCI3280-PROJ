#include "mainwindow.h"
#include <QApplication>
#include <QThread>
#include "server.h"
#include "playthread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle("fusion");
    server s;
    s.StartServer();

    //QThread mainThread;
    //QThread playing;
    //playthread p;
    MainWindow w;
    //p.moveToThread(&playing);
    //w.DoSetup(mainThread);
    //w.moveToThread(&mainThread);
    w.show();
    //mainThread.start();
    //playing.start();

    //p.start();

    return a.exec();
}
