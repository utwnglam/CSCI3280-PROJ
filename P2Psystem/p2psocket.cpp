#include "p2psocket.h"
#define MY_PORT 3434


struct openFileStruct
{
    QString filePath;
    QString fileName;
};

p2psocket::p2psocket(QObject *parent):QObject(parent)
{
}

void p2psocket::p2pconnect(){
    socket =new QTcpSocket(this);

    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(socket,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));

    qDebug()<<"Connecting...";
    socket->connectToHost("127.0.0.1",1234);

    if(!(socket->waitForConnected(3000))){
        qDebug()<<"Not Connected"<<socket->errorString();
     }
    /*socket->connectToHost("127.0.0.1",1234);
    if(socket->waitForConnected(3000)){
        qDebug()<<"Connected";
        //send data
        socket->write("hi\n");
        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);
        qDebug()<<"Reading: "<<socket->bytesAvailable();
        qDebug()<<socket->readAll();

        socket->close();
    }else{
        qDebug()<<"Not Connected"<<socket->errorString();
    }*/
}

void p2psocket::connected(){
    qDebug()<<"Connected";
    socket->write("hi");
}

void p2psocket::disconnected(){
    qDebug()<<"Disconnected";
}

void p2psocket::bytesWritten(qint64 bytes){
    qDebug()<<"wrote bytes:"<<bytes;
}

void p2psocket::readyRead(){
    qDebug()<<"Reading: "<<socket->bytesAvailable();
    qDebug()<<socket->readAll();
}
