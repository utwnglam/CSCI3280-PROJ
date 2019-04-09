#include "p2pthread.h"

p2pThread::p2pThread(int ID, QObject *parent):QThread(parent)
{
    this->socketDescriptor = ID;
    socketTh = new QTcpSocket();
    qDebug()<<"(server)Start thread";
    //socketTh = new QTcpSocket();
    if(!socketTh->setSocketDescriptor(this->socketDescriptor)){
        emit error(socketTh->error());
        return;
    }
    connect(socketTh,SIGNAL(readyRead()),this,SLOT(readyRead()));
    //connect(socketTh,SIGNAL(sendString()),this,SLOT(sendString()));
    connect(socketTh,SIGNAL(disconnected()),this,SLOT(disconnected()));
}

void p2pThread::run(){
    //thread start
    qDebug()<<socketDescriptor<<"(server)Client Connected";
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

    blockSize=0;
    QDataStream in(socketTh);
    in.setVersion(QDataStream::Qt_4_6);
    QByteArray Data;
    Data=socketTh->readAll();
    qDebug()<<socketDescriptor<<"Data in: "<<Data;
   /* if (blockSize == 0) {
            if(socketTh->bytesAvailable() < (int)sizeof(quint16)) return;
            in >> blockSize;
        }
        if(socketTh->bytesAvailable() < blockSize)
            return;*/
    //in >> Data;
    socketTh->write(Data);
    socketTh->waitForBytesWritten(6000);

    QFile file("C:\\Users\\user\\CSCI3280-PROJ\\database2.txt"); // download path
    //QFile file("C:\\Users\\user\\CSCI3280-PROJ\\numb.wav");
    file.open(QIODevice::WriteOnly);
    file.write(Data);
    file.close();
    //Data.resize(0);
}

void p2pThread::disconnected(){

    qDebug()<<socketDescriptor<<"Disconnected~";
    socketTh->deleteLater();
    exit(0);
}
