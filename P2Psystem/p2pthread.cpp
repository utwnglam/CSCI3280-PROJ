#include "p2pthread.h"

p2pThread::p2pThread(int ID, QObject *parent):QThread(parent)
{
    this->socketDescriptor = ID;
    socketTh = new QTcpSocket();
    //socketTh2= new QTcpSocket();
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

    //blockSize=0;
    //QDataStream in(socketTh);
    //in.setVersion(QDataStream::Qt_5_5);
    QByteArray Data2;
    //socketTh->waitForReadyRead();
    this->song =socketTh->readAll();

    //QFile file("C:\\Users\\user\\CSCI3280-PROJ\\database2.txt"); // download path
    std::string song1=this->song.toStdString().c_str();
    std::string path="../P2Psystem/Music/"+song1+".wav";
    QFile file(path.c_str());
    file.open(QIODevice::ReadOnly);
    Data2=file.readAll();
    socketTh->write(Data2);
    //socketTh->flush();
    socketTh->waitForBytesWritten();
    socketTh->close();
    //int filesize=0;

    //if (socketTh->bytesAvailable() <= 0)
        //return;

    //while(socketTh->state() == QAbstractSocket::ConnectedState)  {
        //socketTh->waitForReadyRead();
        //file.write(socketTh->readAll());
        //filesize =file.size();
    //}
    //file.write(Data);
    //file.close();
    //Data.resize(0);
    //socketTh->disconnectFromHost();
}

void p2pThread::disconnected(){

    qDebug()<<socketDescriptor<<"Disconnected~";
    socketTh->deleteLater();
    exit(0);
}
