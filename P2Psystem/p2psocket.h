#ifndef P2PSOCKET_H
#define P2PSOCKET_H
#include <QTcpSocket>
#include <QObject>
#include <QDebug>
//#include <QTcpServer>
#include <QAbstractSocket>

class p2psocket : public QObject
{
    Q_OBJECT
public:
    explicit p2psocket(QObject *parent =0);

    void p2pconnect();

signals:


public slots:
    //void newConnection();
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *socket;
    //QTcpServer *server;

};

#endif // P2PSOCKET_H
