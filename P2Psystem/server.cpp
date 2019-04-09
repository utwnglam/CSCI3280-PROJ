#include "server.h"

server::server(QObject *parent):QTcpServer(parent)
{
    /*connect(server,SIGNAL(newConnection()),this,SLOT(newConnection());
    if(!server->listen(QHostAddress::Any,1234)){
        qDebug()<<"Server build fail";
    }else{
        qDebug()<<"Server builded";
    }*/
    pool= new QThreadPool(this);
    pool->setMaxThreadCount(6);
}

void server::newConnection(){
    //QTcpSocket *socket2=server->nextPendingConnection();
    //socket2->write("Wellcome client!\n");

    /*QFile sendFile = new QFile(sendFilePath);
    if (!sendFile->open(QFile::ReadOnly )){
        return;
    }
    QByteArray outBlock;
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_0);
    sendOut << "message";
    outBlock = sendFile->read(sendFile.size());
    socket2->writeData(outBlock,outBlock.size());
    outBlock.resize(0);*/

    //socket2->flush();

    //socket2->waitForBytesWritten(3000);
    //socket2->close();
}

void server::StartServer(){

    if(!this->listen(QHostAddress::Any,1234)){//this=QtcpServer server
        qDebug()<<"Server build fail";
    }else{
        qDebug()<<"Server builded";
    }
}

void server::incomingConnection(qintptr socketDescriptor){

    qDebug()<<socketDescriptor<<"(server)a client connecting..";
    p2pThread *thread =new p2pThread(socketDescriptor,this);
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    thread->start();
}
