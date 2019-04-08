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
    //socket->write("this is p2psocket");
    //QFile file("C:\\Users\\user\\CSCI3280-PROJ\\music_database2.txt");
    QFile file("C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\numb.wav");
    file.open(QIODevice::ReadOnly);
    mydata=file.readAll();

    socket->write(mydata);
    socket->waitForBytesWritten(3000);
    //mydata.resize(0);
    file.close();

    //send wav:
    //QFile file2("C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\numb.wav");
    //file2.open(QIODevice::ReadOnly);
    //QByteArray mywav=file2.readAll();
    //socket->write(mydata);
    //file2.close();
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

    mydata=socket->readAll();
    QFile file("C:\\Users\\user\\CSCI3280-PROJ\\database.txt"); // download path
    file.open(QIODevice::WriteOnly);
    file.write(mydata);
    mydata.resize(0);
    file.close();
}
