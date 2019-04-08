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
#include "playthread.h"
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
int count=0;//count terminal

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("P2P Karaoke System"));
    //connect(p,SIGNAL(play()),this,SLOT(onplay()));
    m_thread=new playthread(this);
    /*
     REMEMBER TO CHANGE THE PATH FIRST
     */
    //QDir myPath("C:/Users/user/CSCI3280-PROJ/P2Psystem/Music");
    QDir myPath("../P2Psystem/Music");
    myPath.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    myList = myPath.entryList();
    //ui->songL->addItems(myList);

    //QFile file("C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\music_database.txt");
    QFile file("../P2Psystem/music_database.txt");
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
}

void MainWindow::DoSetup(QThread &mainThread){
    //connect(&mainThread,SIGNAL(started()),this,SLOT(on_playButton_clicked()));
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
    myList.append(tmpList[0]);

    //copy the new song to music folder
    std::string song=tmp.toStdString().c_str();//Qstring to string
    //std::string newpath="C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\"+song;
    std::string newpath="../P2Psystem/Music/"+song;
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

void MainWindow::onplay(){

}


void MainWindow::on_playButton_clicked()
{
    //QList<QListWidgetItem *> itemList = ui->songL->selectedItems();
    //int row= ui->songL->row(itemList[0]);
    int exist=0;
    QString song = ui->songName->text();
    //std::string song1=song.toStdString().c_str();
    for(int i=0;i<myList.size();i++){//check is the song exit
        if(song==myList[i]){
            break;
        }else if(i==myList.size()-1){
            exist=1;
        }
    }
    //std::string path="C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\"+song1+".wav";
    //std::string path="E:\\Alisa\\Yr4_Sem2\\CSCI3280 Multimedia\\project\\CSCI3280-PROJ\\P2Psystem\\Music\\"+song1+".wav";


    if((ui->playButton->text() == "play") && (exist==0)){
        p =new playthread(this);
        p->song=song;
        p->Stop=true;
        p->start();
        ui->playButton->setText("stop");
    } else {
        //mPlayer->stop();
        ui->playButton->setText("play");
        p->Stop=false;
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
    //QFile file("C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\music_database.txt");
    QFile file("../P2Psystem/music_database.txt");
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
    socket->p2pconnect();//ui->IPaddr->text().toStdString().c_str();
}

void MainWindow::on_disButton_clicked()
{
    count=0;
}

void MainWindow::on_p2pButton_clicked()
{
    if(ui->p2pButton->text()=="P2P open"){
        ui->p2pButton->setText("P2P close");
    }else if(ui->p2pButton->text()=="P2P close"){
        ui->p2pButton->setText("P2P open");
    }
}

