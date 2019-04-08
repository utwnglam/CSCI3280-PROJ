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
    connect(socketTh,SIGNAL(sendString()),this,SLOT(sendString()));
    connect(socketTh,SIGNAL(disconnected()),this,SLOT(disconnected()));
}

void p2pThread::run(){
    //thread start
    qDebug()<<socketDescriptor<<"Client Connected";
    exec();
}

void p2pThread::sendString()  {
    // send via QByteArray
    /*QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);
    out << (quint16)0;
    // create data for the client
    QString data;
    data.append(tr("Just some bla "));
    out << data;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    // get the next client-connection
    QTcpSocket *clientConnection =
        mServer->nextPendingConnection();
    // signal
    connect(clientConnection, SIGNAL(disconnected()),
        clientConnection, SLOT(deleteLater()));
    // write the string into the socket
    c->write(block);
    // disconnect
    clientConnection->disconnectFromHost();*/
}

void p2pThread::readyRead(){

    QFile file("C:\\Users\\user\\CSCI3280-PROJ\\music_database2.txt");
    file.open(QIODevice::ReadOnly);
    QByteArray mydata=file.readAll();
    QByteArray Data=socketTh->readAll();

    qDebug()<<socketDescriptor<<"Data in: "<<Data;
    qDebug()<<socketDescriptor<<"Data in: "<<mydata;
    socketTh->write(mydata);
    socketTh->write(Data);
    Data.resize(0);
    mydata.resize(0);
}

void p2pThread::disconnected(){

    qDebug()<<socketDescriptor<<"Disconnected~";
    socketTh->deleteLater();
    exit(0);
}
