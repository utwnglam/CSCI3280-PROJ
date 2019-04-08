#include "p2pthread.h"

p2pThread::p2pThread(int ID, QObject *parent):QThread(parent)
{
    this->socketDescriptor = ID;
    socketTh = new QTcpSocket();
    qDebug()<<"Start thread";
    socketTh = new QTcpSocket();
    if(!socketTh->setSocketDescriptor(this->socketDescriptor)){
        emit error(socketTh->error());
        return;
    }
    connect(socketTh,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(socketTh,SIGNAL(disconnected()),this,SLOT(disconnected()));
}

void p2pThread::run(){
    //thread start
    qDebug()<<socketDescriptor<<"Client Connected";
    exec();
}

void p2pThread::readyRead(){

    QByteArray Data=socketTh->readAll();

    qDebug()<<socketDescriptor<<"Data in: "<<Data;
    socketTh->write(Data);
}

void p2pThread::disconnected(){

    qDebug()<<socketDescriptor<<"Disconnected~";
    socketTh->deleteLater();
    exit(0);
}
