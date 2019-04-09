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
#include <QMediaPlayer>
#include <QvideoWidget>

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
    //m_thread=new playthread(this);
    /*
     REMEMBER TO CHANGE THE PATH FIRST
     */
    ui->speedControl->setValue(1.0);
    //QDir myPath("C:/Users/user/CSCI3280-PROJ/P2Psystem/Music");
    QDir myPath("../P2Psystem/Music");
    //QDir myPath("/Users/JoanneCheung/Desktop/3280 PROJ/P2Psystem/Music");
    myPath.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    myList = myPath.entryList();

    //QFile file("C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\music_database.txt");
    QFile file("../P2Psystem/music_database.txt");
    //QFile file("/Users/JoanneCheung/Desktop/3280 PROJ/P2Psystem/music_database.txt");
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"database not found",file.errorString());

    //INPUTING DATABASE INTO ARRAY
    QTextStream in(&file);
    QString whole = in.readAll();
    QStringList linelist = whole.split('\n');
    for(int i = 0; i < myList.size(); i += 1) {

        //for each myList item
        for(int count = 0; count < myList.size(); count++) {

            //finding its according partList item
            QStringList partList = linelist[count].split(", ");
            if (myList[i] == partList[0].mid(1, partList[0].length()-2)) {
                //I have to delete that annoying \r
                QStringList tmpList = partList[3].split('\r');

                QListWidgetItem *pItem = new QListWidgetItem(ui->songL);
                pItem->setData(Qt::UserRole, partList[0].mid(1, partList[0].length()-2));
                pItem->setData(Qt::UserRole + 1, partList[1].mid(1, partList[1].length()-2));
                pItem->setData(Qt::UserRole + 2, partList[2].mid(1, partList[2].length()-2));
                pItem->setData(Qt::UserRole + 3, partList[3].mid(1, tmpList[0].length()-2));
                pItem->setText(partList[1].mid(1, partList[1].length()-2));
                ui->songL->addItem(pItem);
                break;
            }
        }
    }
    /*
    pathName is partList[0], index: Qt::UserRole
    songName is partList[1], index: Qt::UserRole + 1
    bandName is partList[2], index: Qt::UserRole + 2
    albumName is partList[3], index: Qt::UserRole + 3
    */
    file.close();
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
    //std::string newpath="/Users/JoanneCheung/Desktop/3280 PROJ/P2Psystem/Music/"+song;
    QString qnewPath=QString::fromStdString(newpath.c_str());//string to Qstring
    QFile::copy(path, qnewPath);

    QListWidgetItem *pItem = new QListWidgetItem(ui->songL);
    pItem->setData(Qt::UserRole, tmp);
    pItem->setData(Qt::UserRole + 1, tmpList[0]);
    pItem->setData(Qt::UserRole + 2, "N/A");
    pItem->setData(Qt::UserRole + 3, "N/A");
    pItem->setText(tmpList[0]);
    ui->songL->addItem(pItem);

    //adding new line to database when adding new songs
    QFile file("../P2Psystem/music_database.txt");
    //QFile file("/Users/JoanneCheung/Desktop/3280 PROJ/P2Psystem/music_database.txt");
    if(!file.open(QIODevice::ReadWrite))
        QMessageBox::information(0,"database not found",file.errorString());
    QTextStream edit(&file);
    QString tobeAdd = "'"+ tmp +"', '"+ tmpList[0] +"', 'N/A', 'N/A'";

    file.seek(file.size());
    edit << endl << tobeAdd;
    file.close();
}

void MainWindow::on_Del_clicked()
{
    QList<QListWidgetItem *> itemList = ui->songL->selectedItems();
    int row = ui->songL->row(itemList[0]);

    //delete line in database
    QFile file("../P2Psystem/music_database.txt");
    //QFile file("/Users/JoanneCheung/Desktop/3280 PROJ/P2Psystem/music_database.txt");
    if(!file.open(QIODevice::ReadWrite))
        QMessageBox::information(0,"database not found",file.errorString());
    QTextStream edit(&file);

    QString oriPassage = edit.readAll();
    QStringList linelist = oriPassage.split('\n');
    QString newpassage;

    for(int j = 0; j < linelist.size(); j++) {
        //finding its according partList item
        QStringList partList = linelist[j].split(", ");
        if (ui->songL->item(row)->data(Qt::UserRole) != partList[0].mid(1, partList[0].length()-2)) {
            newpassage += linelist[j] + '\n';
        }
    }

    newpassage.chop(1);
    file.resize(0);
    edit << newpassage;

    QString delPath = "/Users/JoanneCheung/Desktop/ver 1/P2Psystem/Music/" + ui->songL->item(row)->data(Qt::UserRole).toString();
    QTextStream debug(stdout);
    debug << delPath;
    QFile delFile(delPath);
    delFile.remove();

    for(int k = 0; k < myList.size(); k++) {
        if(myList[k] == ui->songL->item(row)->data(Qt::UserRole)) {
            myList.removeAt(k);
            break;
        }
    }

    ui->songL->takeItem(row);
    file.close();
}


void MainWindow::onupLyric(const char * line){
    ui->lyrics->setText(QString::fromStdString(line));
}

void MainWindow::ontimeUpdate(unsigned int cur_time)
{
    int _mins = int(cur_time/60);
    int _sec = int(cur_time%60);
    QString cur_length;
    if(_sec <10)
        cur_length = QString::number(_mins) + ":0" + QString::number(_sec);
    else
        cur_length = QString::number(_mins) + ":" + QString::number(_sec);
    qDebug() << "cur_length" << cur_length;
    ui->timer->setText(cur_length);
}

void MainWindow::onsliderUpdate(unsigned int cur_pos)
{
    ui->ProgressBar->setValue(cur_pos);
}

void MainWindow::onGetLength(QString leng)
{
    ui->_length->setText(leng);
}

void MainWindow::on_playButton_clicked()
{
    float speed;
    //QList<QListWidgetItem *> itemList = ui->songL->selectedItems();
    //int row= ui->songL->row(itemList[0]);f
    speed = (float)ui->speedControl->value();
    int exist=0;
    QString song = ui->songName->text();
    //std::string song1=song.toStdString().c_str();
    /*for(int i=0;i<myList.size();i++){//check is the song exit
        if(song==myList[i]){
            break;
        }else if(i==myList.size()-1){
            exist=1;
        }
    }*/
    //std::string path="C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\"+song1+".wav";
    //std::string path="E:\\Alisa\\Yr4_Sem2\\CSCI3280 Multimedia\\project\\CSCI3280-PROJ\\P2Psystem\\Music\\"+song1+".wav";

    if((ui->playButton->text() == "play") && (exist==0)){
        p =new playthread(this);
        connect(p,SIGNAL(upLyric(const char*)),this,SLOT(onupLyric(const char*)));
        connect(p, SIGNAL(timeUpdate(unsigned int)), this, SLOT(ontimeUpdate(unsigned int)));
        connect(p, SIGNAL(sliderUpdate(unsigned int)), this, SLOT(onsliderUpdate(unsigned int)));
        ui->lyrics->setText("Lyrics");
        p->song=song;
        p->Stop=true;
        p->speed = speed;
        p->start();
        qDebug() << "The speed" << QString::number(p->speed, 'f', 2);
        connect(p, SIGNAL(GetLength(QString)), this, SLOT(onGetLength(QString)));
        ui->playButton->setText("stop");
        QIcon ico;
        ico.addPixmap(QPixmap("../P2Psystem/images/stop.png"),QIcon::Normal,QIcon::On);
        //ico.addPixmap(QPixmap("/Users/JoanneCheung/Desktop/3280 PROJ/P2Psystem/images/stop.png"),QIcon::Normal,QIcon::On);
        ui->playButton->setIcon(ico);
        ui->playButton->setCheckable(true);
    } else {
        ui->playButton->setText("play");
        QIcon ico;
        ico.addPixmap(QPixmap("../P2Psystem/images/play.png"),QIcon::Normal,QIcon::On);
        //ico.addPixmap(QPixmap("/Users/JoanneCheung/Desktop/3280 PROJ/P2Psystem/images/play.png"),QIcon::Normal,QIcon::On);
        ui->playButton->setIcon(ico);
        ui->playButton->setCheckable(true);
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

//void MainWindow::on_ProgressBar_sliderMoved(int position)
//{
//
//}

void MainWindow::on_searchBar_textChanged(const QString &arg1)
{
    QRegExp regExp(arg1, Qt::CaseInsensitive, QRegExp::Wildcard);
    ui->songL->clear();
    QFile file("C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\music_database.txt");
    //QFile file("/Users/JoanneCheung/Desktop/3280 PROJ/P2Psystem/music_database.txt");
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"database not found",file.errorString());
    QTextStream in(&file);

    //mylist.filter(regExp) return list of strings that match the regular expression
    for(int i = 0; i < myList.size(); i++) {
        QString line = in.readLine();
        QStringList strlist = line.split('\'');
        QStringList forCompare = strlist;
        forCompare.removeAt(1);

        if (forCompare.filter(regExp).size() != 0) {
            QListWidgetItem *pItem = new QListWidgetItem(ui->songL);
            pItem->setData(Qt::UserRole, strlist[1]);
            pItem->setData(Qt::UserRole + 1, strlist[3]);
            pItem->setData(Qt::UserRole + 2, strlist[5]);
            pItem->setData(Qt::UserRole + 3, strlist[7]);
            pItem->setText(strlist[3]);
            ui->songL->addItem(pItem);
        }

    }
    file.close();
}

void MainWindow::on_songL_itemDoubleClicked(QListWidgetItem *item)
{
    QList<QListWidgetItem *> itemList = ui->songL->selectedItems();
    int row = ui->songL->row(itemList[0]);


    ui->songName->setText(item->data(Qt::UserRole + 1).toString());
    ui->bandName->setText(item->data(Qt::UserRole + 2).toString());
    ui->albumName->setText(item->data(Qt::UserRole + 3).toString());

    QString album_picture = "../P2Psystem/images/" + ui->songName->text() + ".jpg";
    //QString album_picture = "/Users/JoanneCheung/Desktop/3280 PROJ/P2Psystem/images/" + ui->songName->text() + ".jpg";
    QFile pic(album_picture);
    if(pic.exists())
    {
        QPixmap pix(album_picture);
        ui->albumpic->setPixmap(pix);

    }
    else
    {
        ui->albumpic->setText("Album picture is not available");
    }
}

void MainWindow::on_Edit_clicked()
{
    QString album_picture = "../P2Psystem/images/" + ui->songName->text() + ".jpg";
    //QFile file("/Users/JoanneCheung/Desktop/3280 PROJ/P2Psystem/music_database.txt");
    if(!file.open(QIODevice::ReadWrite))
        QMessageBox::information(0,"database not found",file.errorString());

    QList<QListWidgetItem *> itemList = ui->songL->selectedItems();
    int row = ui->songL->row(itemList[0]);
    QTextStream edit(&file);
    QTextStream db(stdout); //for debug

    QString oriPassage = edit.readAll();
    db << oriPassage << endl;
    //lineList is ready
    QStringList linelist = oriPassage.split('\n');
    QString newPassage;

    db << myList.size() << endl;
    for (int i = 0; i < myList.size(); i++) {
        //prepare partlist for you
        QStringList partList = linelist[i].split(", ");

        if(ui->songL->item(row)->data(Qt::UserRole) != partList[0].mid(1, partList[0].length()-2)) {
            newPassage += linelist[i]+ '\n';
        }
        else {
            QString tobeAdd, part2, part3;
            //edit it
            if(ui->singerEdit->text() != NULL){
                part2 = ui->singerEdit->text();

                //start to update songL item
                ui->songL->item(row)->setData(Qt::UserRole + 2, ui->singerEdit->text());
                ui->bandName->setText(ui->singerEdit->text());
            }
            else {
                part2 = ui->songL->item(row)->data(Qt::UserRole + 2).toString();
            }
            if(ui->albumEdit->text() != NULL){
                part3 = ui->albumEdit->text();

                //start to update songL item
                ui->songL->item(row)->setData(Qt::UserRole + 3, ui->albumEdit->text());
                ui->albumName->setText(ui->albumEdit->text());
            }
            else {
                part3 = ui->songL->item(row)->data(Qt::UserRole + 3).toString();
            }
            tobeAdd = partList[0] +", '"+ ui->songName->text() +"', '"+ part2 +"', '"+ part3 + "'";
            db << tobeAdd << endl; //for debug
            newPassage += tobeAdd + '\n';
        }
    }

    //push it into the file
    file.resize(0);
    newPassage.chop(1);
    edit << newPassage;
    file.close();
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




//void MainWindow::on_ProgressBar_actionTriggered(int action)
//{
//
//}


void MainWindow::on_pushButton_clicked()
{

    QString path = QFileDialog::getOpenFileName(this,"Add");
    if(path == NULL){
        return;
    }
    qDebug() << path;
    QMediaPlayer* player = new QMediaPlayer;
    QVideoWidget* vw = new QVideoWidget;

    player->setVideoOutput(vw);
    player->setMedia(QUrl::fromLocalFile((path)));
    vw->setGeometry(100,100,300,400);
    vw->show();

    player->play();


/*
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("Users/adam9/Desktop/Nishino.mp4"));
    player->setVolume(10);
    player->play();
    */
}
