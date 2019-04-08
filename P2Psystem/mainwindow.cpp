#define _CRT_SECURE_NO_WARNINGS
#undef UNICODE
#pragma comment(lib, "ws2_32.lib")
#define MY_PORT 3434
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"msacm32.lib")
#include <Winsock2.h>
#include <Windows.h>
#include <iostream>
#include <mmsystem.h>
#include <string>
#include <fstream>
#include <stdio.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>
#include <QDataStream>
#include <QListWidgetItem>
#include <QListWidget>
#include <QtGui>
#include <QtCore>
#include <QStringList>

struct User : QObjectUserData {
    QString pathName;
    QString songName;
    QString bandName;
    QString albumName;
};
Q_DECLARE_METATYPE(User)
typedef std::map<std::string,std::string> settings_t;
int count=0;//count terminal

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("P2P Karaoke System"));
    /*
     REMEMBER TO CHANGE THE PATH FIRST
     */
    QDir myPath("C:/Users/user/CSCI3280-PROJ/P2Psystem/Music");
    //QDir myPath("C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music");
    myPath.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    myList = myPath.entryList();
    //ui->songL->addItems(myList);

    //QFile file("C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\music_database.txt");
    QFile file("C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\music_database.txt");
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"database not found",file.errorString());

    //INPUTING DATABASE INTO ARRAY
    QTextStream in(&file);
    for(int i = 0; i< myList.size();++i)
    {
        QString line = in.readLine();
        QStringList strlist = line.split('\'');
        QString tmp = myList.at(i);
        QStringList tmpList=tmp.split('.');
        tmpList.removeLast();
        myList[i]=tmpList[0];

        QListWidgetItem *pItem = new QListWidgetItem(ui->songL);
        pItem->setData(Qt::UserRole, strlist[1]);
        pItem->setData(Qt::UserRole + 1, tmpList[0]);
        pItem->setData(Qt::UserRole + 2, strlist[5]);
        pItem->setData(Qt::UserRole + 3, strlist[7]);
        pItem->setText(tmpList[0]);
        ui->songL->addItem(pItem);

        /*
        pathName is strlist[1], index: Qt::UserRole
        songName is strlist[3], index: Qt::UserRole + 1
        bandName is strlist[5], index: Qt::UserRole + 2
        albumName is strlist[7], index: Qt::UserRole + 3
        */
    }
    file.close();

    /*
    int nCount = ui->songL->count();
    for(int i=0;i<nCount;i++){
        QListWidgetItem *item = ui->songL->item(i);
    }
    QString lyric = this->getLyric(currentSongInfo.lyricLink);//get lyrics from web
    lyricList = this->parse(lyric);//get lyrics sentence by sentence to lyricList
    list->setBackgroundColor(Qt::red);

    mPlayer = new QMediaPlayer(this);
    connect(mPlayer, &QMediaPlayer::positionchanged, this, &MainWindow::on_positionChanged);
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Add_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"Add");
    if(path == NULL){
        return;
    }
    QStringList filepath = path.split('/');
    QString tmp =filepath[filepath.size()-1];
    QStringList tmpList=tmp.split('.');
    tmpList.removeLast();

    //copy the new song to music folder
    std::string song=tmp.toStdString().c_str();//Qstring to string
    //std::string newpath="C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\"+song;
    std::string newpath="C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\"+song;
    QString qnewPath=QString::fromStdString(newpath.c_str());//string to Qstring
    QFile::copy(path, qnewPath);

    QListWidgetItem *pItem = new QListWidgetItem(ui->songL);
    pItem->setData(Qt::UserRole, path);
    pItem->setData(Qt::UserRole + 1, tmpList[0]);
    pItem->setData(Qt::UserRole + 2, "N/A");
    pItem->setData(Qt::UserRole + 3, "N/A");
    pItem->setText(tmpList[0]);
    ui->songL->addItem(pItem);

    //mPlayer->setMedia(QUrl::fromLocalFile(filename));
    //on_playButton_clicked();
}

void MainWindow::on_Del_clicked()
{
    QList<QListWidgetItem *> itemList = ui->songL->selectedItems();
    int row= ui->songL->row(itemList[0]);
    ui->songL->takeItem(row);
}

void MainWindow::on_playButton_clicked()
{
    //QList<QListWidgetItem *> itemList = ui->songL->selectedItems();
    //int row= ui->songL->row(itemList[0]);
    int exist=0;
    QString song = ui->songName->text();
    std::string song1=song.toStdString().c_str();
    for(int i=0;i<myList.size();i++){//check is the song exit
        if(song==myList[i]){
            break;
        }else if(i==myList.size()-1){
            exist=1;
        }
    }
    std::string path="C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\"+song1+".wav";
    //std::string path="E:\\Alisa\\Yr4_Sem2\\CSCI3280 Multimedia\\project\\CSCI3280-PROJ\\P2Psystem\\Music\\"+song1+".wav";


    if((ui->playButton->text() == "play") && (exist==0)){
        //mPlayer -> play();
        LPSTR szFileName;
        LPTSTR szPathName;
        MMCKINFO mmckinfoParent;
        MMCKINFO mmckinfoSubChunk;
        DWORD dwFmtSize;
        DWORD m_WaveLong;
        WAVEFORMATEX* lpFormat;
        DWORD m_dwDataOffset;
        DWORD m_dwDataSize;
        WAVEOUTCAPS pwoc;
        LONG lSoundOffset;

        LONG lSoundLong;

        //szFileName = (LPTSTR)filename;
        szPathName = (LPTSTR)path.c_str();
        float speed = 1.0;
        HMMIO m_hmmio;
        printf("%s\n", szPathName);
        if (!(m_hmmio = mmioOpen(szPathName, NULL, MMIO_READ)))
        {
            printf("fail in mmioOpen\n");
        }

        mmckinfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
        if (mmioDescend(m_hmmio, (LPMMCKINFO)& mmckinfoParent, NULL, MMIO_FINDRIFF))
        {
            printf("fail in mmioDescend:WAVE\n");
        }
        mmckinfoSubChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
        if (mmioDescend(m_hmmio, &mmckinfoSubChunk, &mmckinfoParent,
            MMIO_FINDCHUNK))
        {
            printf("fail in mmioDescend:fmt\n");
        }

        dwFmtSize = mmckinfoSubChunk.cksize;
        lpFormat = (WAVEFORMATEX *)LocalLock(LocalAlloc(LMEM_MOVEABLE, LOWORD(dwFmtSize)));
        if ((unsigned long)mmioRead(m_hmmio, (HPSTR)lpFormat, dwFmtSize) != dwFmtSize)
        {
            printf("fail in mmioRead\n");
        }
        mmioAscend(m_hmmio, &mmckinfoSubChunk, 0);
        mmckinfoSubChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
        if (mmioDescend(m_hmmio, &mmckinfoSubChunk, &mmckinfoParent, MMIO_FINDCHUNK))
        {
            printf("fail in mmioDescend:data\n");
        }
        m_dwDataSize = mmckinfoSubChunk.cksize;
        m_dwDataOffset = mmckinfoSubChunk.dwDataOffset;
        if (m_dwDataSize == 0L)
        {
            printf("the file has no data!");
        }

        char* lpData = (char *)malloc(sizeof(char) * m_dwDataSize);
        if (!lpData)
        {
            printf("Alloc memory for wave data failed!");
        }


        lSoundOffset = m_dwDataOffset;
        LONG lSize = mmioSeek(m_hmmio, lSoundOffset, SEEK_SET);
        int value;
        if (mmioRead(m_hmmio, lpData, m_dwDataSize) != m_dwDataSize)
        {
            printf("something wrong in mmioRead\n");
        }

        HWAVEOUT hWaveOut;
        lpFormat->nSamplesPerSec *= speed; // revise this variable for tuning the speed
        waveOutOpen(&hWaveOut, WAVE_MAPPER, lpFormat, 0L, 0L, WAVE_FORMAT_DIRECT);

        WAVEHDR WaveOutHdr;
        WaveOutHdr.lpData = (LPSTR)lpData;
        WaveOutHdr.dwBufferLength = m_dwDataSize;
        WaveOutHdr.dwBytesRecorded = 0;
        WaveOutHdr.dwUser = 0L;
        WaveOutHdr.dwFlags = 0L;
        WaveOutHdr.dwLoops = 0L;
        waveOutPrepareHeader(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR));
        int min=0, sec=0, total_sec;
        min = (int)(m_dwDataSize/lpFormat->nAvgBytesPerSec/60);
        sec = (int)(m_dwDataSize/lpFormat->nAvgBytesPerSec%60);
        total_sec = (int)(m_dwDataSize/lpFormat->nAvgBytesPerSec);
        qDebug() << "the length of a song is" << m_dwDataSize <<"/" << lpFormat->nAvgBytesPerSec <<"is" << min << ":" << sec;
        int slider_pos = 0;
        ui->ProgressBar->setValue(slider_pos);
        //slider_pos = int(current_sec/total_sec) * 100; // update the slider position by current playing time
        //update timer label
        //ui->_length->setText(min + ":" + sec);
        //ui->timer->setText(current_sec/60 + ":" + current_sec%60);
        do {} while (waveOutUnprepareHeader(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
        waveOutClose(hWaveOut);
        ui->playButton->setText("stop");
    } else {
        //mPlayer->stop();
        ui->playButton->setText("play");
        std::cout << "the music is stop!" << endl;
    }
}
/*
 * FOR TRANSLATING QSTRING TO CHAR*
QString temp = item->data(Qt::UserRole).toString();
QByteArray byteArray = temp.toLocal8Bit();
char *c = byteArray.data();
kernel->playMusic(c, 1.0);
*/

void MainWindow::on_ProgressBar_sliderMoved(int position)
{

}

void MainWindow::on_searchBar_textChanged(const QString &arg1)
{
    QRegExp regExp(arg1, Qt::CaseInsensitive, QRegExp::Wildcard);
    ui->songL->clear();
    QFile file("C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\music_database.txt");
    //QFile file("E:\\Alisa\\Yr4_Sem2\\CSCI3280 Multimedia\\project\\CSCI3280-PROJ\\P2Psystem\\music_database.txt");
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"database not found",file.errorString());
    QTextStream in(&file);
    for(int i = 0; i< myList.filter(regExp).size();++i)
    {
        while(!(in.atEnd())){
            QString line = in.readLine();
            QStringList strlist = line.split('\'');
            if(QString::compare(myList.filter(regExp)[i], strlist[3], Qt::CaseInsensitive)==0 ||
                    QString::compare(myList.filter(regExp)[i], strlist[5], Qt::CaseInsensitive)==0||
                    QString::compare(myList.filter(regExp)[i], strlist[7], Qt::CaseInsensitive)==0){
                QString tmp = myList.filter(regExp).at(i);
                QListWidgetItem *pItem = new QListWidgetItem(ui->songL);
                pItem->setData(Qt::UserRole, strlist[1]);
                pItem->setData(Qt::UserRole + 1, tmp);
                pItem->setData(Qt::UserRole + 2, strlist[5]);
                pItem->setData(Qt::UserRole + 3, strlist[7]);
                pItem->setText(tmp);
                ui->songL->addItem(pItem);
                break;
            }
        }
    }
    file.close();
}

void MainWindow::on_songL_itemDoubleClicked(QListWidgetItem *item)
{
    ui->songName->setText(item->data(Qt::UserRole + 1).toString());
    ui->bandName->setText(item->data(Qt::UserRole + 2).toString());
    ui->albumName->setText(item->data(Qt::UserRole + 3).toString());
}

void MainWindow::on_connectButton_clicked()
{
    socket = new p2psocket(this);
    socket->p2pconnect();//ui->IPaddr->text().toStdString().c_str()

    /*WSADATA wsaData;
    if ( WSAStartup (MAKEWORD(2,2), &wsaData)!=0)
       QMessageBox::information(NULL,"network function call fails!","WSAGetLastError");
            //WSAGetLastError()

    SOCKADDR_IN remote_addr;
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(MY_PORT);
    remote_addr.sin_addr.S_un.S_addr = inet_addr(ui->IPaddr->text().toStdString().c_str());

    //Connect server:
    SOCKET conn_sock =socket(AF_INET,SOCK_STREAM,0);
    if(!(conn_sock)){
        QMessageBox::information(NULL,"socket creation call fails!","INVALID_SOCKET" );
        //return INVALID_SOCKET;
    }
    ::connect(conn_sock, (struct sockaddr *)&remote_addr, sizeof(SOCKADDR_IN));

    int bytes_recvd;
    char recvBuf[100];
    bytes_recvd = recv(conn_sock, recvBuf, sizeof(recvBuf),0);
    char *buffer =(char*)"Wellcome to PSPsystem!\0";
    send(conn_sock, buffer, sizeof(buffer), 0);

    //Wait connection request:
    //int listen(SOCKET socket, int backlog);//backlog:The maximum length of the queue of pending connections to accept
    //Accepting connection:
    //SOCKET accept (SOCKET s, struct sockaddr *addr,  int *len);//addr:set NULL if dun wait it
    //Receive /Send data(stream)
    //int recv(SOCKET s, const char *but, int len, int flags);
    //int send(SOCKET s, const char *but, int len, int flags);
    //int recvfrom(SOCKET s, char *buf, int len, int flags, sockaddr *from, int *fromlen) ;
    //int sendto(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen) ;

    //Close socket
    count++;
    ui->terCount->setText(QString::number(count));
    closesocket(conn_sock);
    WSACleanup();*/

}

void MainWindow::on_disButton_clicked()
{
    count=0;
}

void MainWindow::on_p2pButton_clicked()
{
    if(ui->p2pButton->text()=="P2P open"){

       /* WSADATA wsaData;
        if ( WSAStartup (MAKEWORD(2,2), &wsaData)!=0)
           QMessageBox::information(NULL,"network function call fails!","WSAGetLastError");

       SOCKADDR_IN my_addr;
       my_addr.sin_family = AF_INET;
       my_addr.sin_port = htons(MY_PORT);
       int ip;
       my_addr.sin_addr.S_un.S_addr = htonl((u_long)get_local_ip(ip));

       SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
       bind(listen_sock, (struct sockaddr *) &my_addr, sizeof(SOCKADDR_IN));
       listen(listen_sock, SOMAXCONN);

       int dummy;
       char *buffer =(char*)"Wellcome to P2Psystem\0";
       SOCKET new_sock = accept(listen_sock, NULL, &dummy);
       send(new_sock, buffer, sizeof(buffer), 0);
       char recvBuf[100];
       memset(recvBuf, 0, sizeof(recvBuf));
       recv(new_sock, recvBuf, sizeof(recvBuf), 0);

        closesocket(new_sock);
        closesocket(listen_sock);
        WSACleanup();*/
        ui->p2pButton->setText("P2P close");
    }else if(ui->p2pButton->text()=="P2P close"){
        ui->p2pButton->setText("P2P open");
    }
}

