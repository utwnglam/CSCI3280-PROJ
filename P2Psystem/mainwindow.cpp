#define _CRT_SECURE_NO_WARNINGS
#undef UNICODE
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"msacm32.lib")
#include <iostream>
#include <Windows.h>
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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("P2P Karaoke System"));
    /*
     REMEMBER TO CHANGE THE PATH FIRST
     */
    QDir myPath("C:/Users/user/CSCI3280-PROJ/P2Psystem/Music");
    myPath.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    myList = myPath.entryList();
    //ui->songL->addItems(myList);

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
    QList<QListWidgetItem *> itemList = ui->songL->selectedItems();
    int row= ui->songL->row(itemList[0]);
    QString song = myList[row];
    std::string song1=song.toStdString().c_str();
    std::string path="C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\"+song1+".wav";

    if(ui->playButton->text() == "play"){
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
        float speed = 0.8;
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
