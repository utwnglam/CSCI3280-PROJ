#ifndef SERVER_H
#define SERVER_H
#include <QTcpSocket>
#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QThreadPool>
#include "p2pthread.h"

class server : public QTcpServer
{
    Q_OBJECT
public:
    explicit server(QObject *parent =0);
    void StartServer();

signals:

public slots:
    void newConnection();

protected:
    void incomingConnection( qintptr socketDescriptor);

private:
    QTcpSocket *socket;
    //QTcpServer *server;
    p2pThread *mthread;
    QThreadPool *pool;

};

#endif // SERVER_H
