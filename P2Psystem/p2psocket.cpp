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
    socket->connectToHost(ip,1234);//"127.0.0.1" should be input

    if(!(socket->waitForConnected(3000))){
        qDebug()<<"(Client)Not Connected"<<socket->errorString();
     }
    if(socket->waitForConnected(3000)){
    socket->write(song.toStdString().c_str());
    socket->waitForBytesWritten();
    //socket->waitForReadyRead();
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
    //QFile file("C:\\Users\\user\\CSCI3280-PROJ\\music_database2.txt");
    //std::string song1=this->song.toStdString().c_str();
    //std::string path="../P2Psystem/Music/"+song1+".wav";
    //QFile file(path.c_str());
    //if(file.open(QIODevice::ReadOnly)){
        //mydata=file.readAll();
        //int size =file.size();
        //socket->write(std::to_string(size).c_str());
        //QDataStream out(&mydata, QIODevice::WriteOnly);
        //out.setVersion(QDataStream::Qt_5_5);
        //out << (quint16)0;
        //out.device()->seek(0);
        //out << (quint16)(mydata.size() - sizeof(quint16));
        //out <<mydata;
        //QDataStream socketStream(socket);
        //while(mydata=)
        //socketStream << mydata;
        //socket->waitForBytesWritten(-1);

        //mydata.resize(0);
    //}
    //file.close();

    //send wav:
    //QFile file2("C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\numb.wav");
    //file2.open(QIODevice::ReadOnly);
    //QByteArray mywav=file2.readAll();
    //socket->write(mydata);
    //file2.close();
    socket->waitForReadyRead();
    //socket->disconnectFromHost();
}

void p2psocket::bytesWritten(qint64 bytes){
    qDebug()<<"(Client)wrote bytes:"<<bytes;
}

void p2psocket::readyRead(){
    //socket->waitForReadyRead();
    qDebug()<<"(Client)Reading: "<<socket->bytesAvailable();
    qDebug()<<socket->readAll();
    std::string song1=this->song.toStdString().c_str();
    std::string path="../P2Psystem/Video"+song1+".wav";
    QFile file(path.c_str());
    //QFile file("C:\\Users\\user\\CSCI3280-PROJ\\database2.txt");
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
    file.close();
    //Data.resize(0);
    socket->disconnectFromHost();
}

void p2psocket::disconnected(){
    qDebug()<<"(Client)Disconnected";
    socket->close();
}
