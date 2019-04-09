#ifndef P2PSOCKET_H
#define P2PSOCKET_H
#include <QTcpSocket>
#include <QObject>
#include <QDebug>
//#include <QTcpServer>
#include <QAbstractSocket>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QDataStream>
#include <QAudioOutput>
#include <QAudioInput>

class p2psocket : public QObject
{
    Q_OBJECT
public:
    explicit p2psocket(QObject *parent =0);

    void p2pconnect();
    QString ip;
    QString song;

signals:


public slots:
    //void newConnection();
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *socket;
    QTcpSocket *socket2;
    QByteArray mydata;
    //QTcpServer *server;

};

#endif // P2PSOCKET_H
