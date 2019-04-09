#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>

class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = 0);
    void SetSocket(int Descriptor);

signals:

public slots:
    void connected();
    void disconnected();
    //void bytesWritten(qint64 bytes);
    void readyRead();
    void TaskResult(int Number);

private:
    QTcpSocket *socket;
};

#endif // CLIENT_H
