#ifndef SEND_H
#define SEND_H
#include <QDebug>
#include <QObject>
#include <QRunnable>
#include <QTcpSocket>

class send : public QRunnable
{
    Q_OBJECT
public:
    send();
    int SocketDescriptor;
    QString song;

signals:
    //void Result(int Number);

protected:
    void run();





};

#endif // SEND_H
