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

    qDebug()<<"(Client)Connecting...";
    socket->connectToHost("127.0.0.1",1234);//"127.0.0.1" should be input

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
    //socket->write("this is p2psocket");
    QFile file("C:\\Users\\user\\CSCI3280-PROJ\\music_database2.txt");
    //QFile file("C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\numb.wav");
    file.open(QIODevice::ReadOnly);
    mydata=file.readAll();
    //QDataStream out(&mydata, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_4_6);
    //out << (quint16)0;
    //out.device()->seek(0);
    //out << (quint16)(mydata.size() - sizeof(quint16));



    socket->write(mydata);
    socket->waitForBytesWritten(1000);
    //mydata.resize(0);
    //file.close();

    //send wav:
    //QFile file2("C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\numb.wav");
    //file2.open(QIODevice::ReadOnly);
    //QByteArray mywav=file2.readAll();
    //socket->write(mydata);
    //file2.close();
}

void p2psocket::disconnected(){
    qDebug()<<"(Client)Disconnected";
}

void p2psocket::bytesWritten(qint64 bytes){
    qDebug()<<"(Client)wrote bytes:"<<bytes;
}

void p2psocket::readyRead(){
    qDebug()<<"(Client)Reading: "<<socket->bytesAvailable();
    qDebug()<<socket->readAll();

    mydata=socket->readAll();
    QFile file("C:\\Users\\user\\CSCI3280-PROJ\\database.txt"); // download path
    file.open(QIODevice::WriteOnly);
    file.write(mydata);
    mydata.resize(0);
    file.close();
}
