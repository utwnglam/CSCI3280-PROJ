#include "updatabase.h"

updatabase::updatabase(QObject *parent) : QObject(parent)
{

}

void updatabase::databaseConnect(){
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

}

void updatabase::connected(){
    qDebug()<<"(Client)Connected";
    socket->write("get database");
    socket->waitForBytesWritten();

}

void updatabase::bytesWritten(qint64 bytes){
    qDebug()<<"(Client)wrote bytes:"<<bytes;
}

void updatabase::readyRead(){
    QByteArray Data;
    //socket->waitForReadyRead();
    //qDebug()<<"(Client)Reading: "<<socket->bytesAvailable();
    std::string path="../P2Psystem/music_database2.txt";
    QFile file(path.c_str());
    //QFile file("C:\\Users\\user\\CSCI3280-PROJ\\music_database2.txt");
    file.open(QIODevice::WriteOnly);
    //int filesize=0;
    if (socket->bytesAvailable() <= 0)
        return;

    file.write(socket->readAll());
    file.waitForBytesWritten(1000);

    this->ok=true;
    file.close();
    //Data.resize(0);
    //socket->write("done");
    //socket->close();
    socket->disconnectFromHost();
}

void updatabase::disconnected(){
    qDebug()<<"(Client)Disconnected";
    socket->close();
}
