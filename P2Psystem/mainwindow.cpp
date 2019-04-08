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
    QDir myPath("/Users/JoanneCheung/Desktop/3280 PROJ/P2Psystem/Music");
    myPath.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    myList = myPath.entryList();

    QFile file("/Users/JoanneCheung/Desktop/3280 PROJ/P2Psystem/music_database.txt");
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

    std::string song=tmp.toStdString().c_str();//Qstring to string
    std::string newpath="C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\"+song;
    QString qnewPath=QString::fromStdString(newpath.c_str());//string to Qstring
    QFile::copy(path, qnewPath);//copy the new song to music folder

    QListWidgetItem *pItem = new QListWidgetItem(ui->songL);
    pItem->setData(Qt::UserRole, path);
    pItem->setData(Qt::UserRole + 1, tmpList[0]);
    pItem->setData(Qt::UserRole + 2, "N/A");
    pItem->setData(Qt::UserRole + 3, "N/A");
    pItem->setText(tmpList[0]);
    ui->songL->addItem(pItem);

    //adding new line to database when adding new songs
    QFile file("../P2Psystem/music_database.txt");
    if(!file.open(QIODevice::ReadWrite))
        QMessageBox::information(0,"database not found",file.errorString());
    QTextStream edit(&file);
    QString tobeAdd = "'"+ path +"', '"+ tmpList[0] +"', 'N/A', 'N/A'";

    file.seek(file.size());
    edit << tobeAdd << endl;
}

void MainWindow::on_Del_clicked()
{
    QList<QListWidgetItem *> itemList = ui->songL->selectedItems();
    int row = ui->songL->row(itemList[0]);

    //delete line in database
    QFile file("../P2Psystem/music_database.txt");
    if(!file.open(QIODevice::ReadWrite))
        QMessageBox::information(0,"database not found",file.errorString());
    QTextStream edit(&file);
    QString newPassage;

    int i = 0;
    while(ui->songL->item(row)->data(Qt::UserRole + 1) != myList[i]) {
        i += 1;
    }
    for (int k = 0; k <= i; k++) {
        file.readLine();
    }
    QString remainText = edit.readAll();
    file.seek(0);
    for (int k = 0; k < i; k++) {
        newPassage += file.readLine();
    }
    newPassage += remainText;
    file.resize(0);
    edit << newPassage;

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

void MainWindow::on_Edit_clicked()
{
    QFile file("../P2Psystem/music_database.txt");
    if(!file.open(QIODevice::ReadWrite))
        QMessageBox::information(0,"database not found",file.errorString());
    QTextStream edit(&file);
    QTextStream db(stdout);
    QString desiredLine;

    QList<QListWidgetItem *> itemList = ui->songL->selectedItems();
    int row = ui->songL->row(itemList[0]);

    //change singer name
    if(ui->singerEdit->text() != NULL){
        int i = 0;
        //find the desired line
        while(ui->songName->text() != myList[i]) {
            i += 1;
        }
        for (int k = 0; k <= i; k++) {
            desiredLine = file.readLine();
        }
        //edit it
        QStringList strlist = desiredLine.split('\'');
        QString tobeAdd = "'"+ strlist[1] +"', '"+ ui->songName->text() +"', '"+ ui->singerEdit->text() +"', '"+ strlist[7] +"'";

        db << tobeAdd << endl; //for debug
        //push it into the file
        file.seek(0);
        QString editText = edit.readAll();
        QRegularExpression re(desiredLine);
        QString replacementText(tobeAdd);
        editText.replace(re, replacementText);
        file.resize(0);
        edit << editText;

        //start to update songL item
        ui->songL->item(row)->setData(Qt::UserRole + 2, ui->singerEdit->text());
        ui->bandName->setText(ui->singerEdit->text());
    }

    //change album name
    if(ui->albumEdit->text() != NULL){
        int i = 0;
        while(ui->songName->text() != myList[i]) {
            i += 1;
        }
        for (int k = 0; k <= i; k++) {
            desiredLine = file.readLine();
        }
        QStringList strlist = desiredLine.split('\'');
        QString tobeAdd = "'"+ strlist[1] +"', '"+ ui->songName->text() +"', '"+ strlist[5] +"', '"+ ui->albumEdit->text() +"'";

        file.seek(0);
        QString editText = edit.readAll();
        QRegularExpression re(desiredLine);
        QString replacementText(tobeAdd);
        editText.replace(re, replacementText);

        file.resize(0);
        edit << editText;
        ui->songL->item(row)->setData(Qt::UserRole + 3, ui->albumEdit->text());
        ui->albumName->setText(ui->albumEdit->text());
    }
}
