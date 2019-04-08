#ifndef P2PTHREAD_H
#define P2PTHREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QDebug>

class p2pThread : public QThread
{
    Q_OBJECT
public:
    explicit p2pThread(int ID, QObject *parent =0);
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void disconnected();
    void readyRead();

private:
    QTcpSocket *socketTh;
    int socketDescriptor;
};

#endif // P2PTHREAD_H
