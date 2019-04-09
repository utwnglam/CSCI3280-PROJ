#include "send.h"

send::send()
{

}

void send::run()
{
    if(!SocketDescriptor) return;
    QByteArray mydata;
    QTcpSocket socket;
    socket.setSocketDescriptor(SocketDescriptor);
    std::string song1=this->song.toStdString().c_str();
    std::string path="../P2Psystem/Music/"+song1+".wav";
    QFile file(path.c_str());
    if(file.open(QIODevice::ReadOnly)){
        mydata=file.readAll();
        socket->write(mydata);
        //socket->waitForBytesWritten(1000);
    }
    file.close();
    socket.flush();
    socket.waitForBytesWritten();
    socket.close();
}
