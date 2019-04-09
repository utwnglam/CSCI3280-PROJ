#ifndef P2PTHREAD_H
#define P2PTHREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>
#include <QTcpServer>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>

class p2pThread : public QThread
{
    Q_OBJECT
public:
    explicit p2pThread(int ID, QObject *parent =0);
    void run();
    QTcpServer *mServer;
    QString song;
    bool ok;

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void disconnected();
    void readyRead();
    void sendString();

private:
    QTcpSocket *socketTh;
    int socketDescriptor;
    quint16 blockSize;
};

#endif // P2PTHREAD_H
