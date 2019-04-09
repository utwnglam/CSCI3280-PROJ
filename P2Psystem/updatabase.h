#ifndef UPDATABASE_H
#define UPDATABASE_H

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

class updatabase : public QObject
{
    Q_OBJECT
public:
    explicit updatabase(QObject *parent = 0);
    void databaseConnect();
    QString ip;
    bool ok;

public slots:
    //void newConnection();
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *socket;
    QByteArray mydata;
    //QTcpServer *server;

};

#endif // UPDATABASE_H
