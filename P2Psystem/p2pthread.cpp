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

void p2pThread::readyRead(){

    //blockSize=0;
    //QDataStream in(socketTh);
    //in.setVersion(QDataStream::Qt_5_5);
    QByteArray Data2;
    QByteArray Data;
    QString check=socketTh->readAll();

    if(check=="get database"){
        qDebug()<<"This get database";
        std::string path="../P2Psystem/music_database.txt";
        QFile file(path.c_str());
        file.open(QIODevice::ReadOnly);
        Data=file.readAll();
        file.close();
        socketTh->write(Data);
        //socketTh->flush();
        socketTh->waitForBytesWritten();
        //socketTh->close();
    }else{
        this->song =check;
        qDebug()<<this->song;
        qDebug()<<"This get song";
        //QFile file("C:\\Users\\user\\CSCI3280-PROJ\\database2.txt"); // download path
        std::string song1=this->song.toStdString().c_str();
        std::string path="../P2Psystem/Music/"+song1+".wav";
        QFile file(path.c_str());
        file.open(QIODevice::ReadOnly);
        Data2=file.readAll();
        file.close();
        socketTh->write(Data2);
        //socketTh->flush();
        socketTh->waitForBytesWritten();
        //socketTh->close();
    }
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
    socketTh->close();
    //socketTh->disconnectFromHost();
}

void p2pThread::disconnected(){

    qDebug()<<socketDescriptor<<"Disconnected~";
    socketTh->deleteLater();
    exit(0);
}
