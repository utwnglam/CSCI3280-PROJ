#include "p2psocket.h"
#define MY_PORT 3434

p2psocket::p2psocket(QObject *parent):QObject(parent)
{
}

void p2psocket::p2pconnect(){
    socket =new QTcpSocket(this);

    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(socket,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));

    qDebug()<<"(Client)Connecting...";
    socket->connectToHost(this->ip,1234);//"127.0.0.1" should be input

    if(!(socket->waitForConnected(3000))){
        qDebug()<<"(Client)Not Connected"<<socket->errorString();
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
    qDebug()<<"(Client)Connected";
    qDebug()<<this->song;
    socket->write(this->song.toStdString().c_str());
    socket->waitForBytesWritten();

}

void p2psocket::bytesWritten(qint64 bytes){
    qDebug()<<"(Client)wrote bytes:"<<bytes;
}

void p2psocket::readyRead(){

    std::string song1=this->song.toStdString().c_str();
    std::string path="../P2Psystem/Music/"+song1+".wav";
    QFile file(path.c_str());
    file.open(QIODevice::WriteOnly);
    //int filesize=0;
    if (socket->bytesAvailable() <= 0)
        return;

    while(socket->state() == QAbstractSocket::ConnectedState)  {
        socket->waitForReadyRead();
        file.write(socket->readAll());
        //filesize =file.size();
    }
    //file.write(Data);
    this->ok=true;
    file.close();
    //Data.resize(0);
    socket->disconnectFromHost();
}

void p2psocket::disconnected(){
    qDebug()<<"(Client)Disconnected";
    socket->close();
}
