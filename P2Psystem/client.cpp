#include "client.h"


client::client(QObject *parent) : QObject(parent)
{

}

void client::SetSocket(int Descriptor){
    socket = new QTcpSocket(this);

    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    //connect(socket,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));

    socket->setSocketDescriptor(Descriptor);

    qDebug()<<"client connected ~";

}

void client::connected(){
    qDebug()<<"client connected event";
}

void client::disconnected(){
    qDebug()<<"client disconnected";
}

void client::readyRead(){
    qDebug()<< socket->readAll();

}

void client::TaskResult(int Number){

}
