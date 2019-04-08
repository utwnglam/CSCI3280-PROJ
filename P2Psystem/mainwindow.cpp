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
    //m_thread=new playthread(this);
    /*
     REMEMBER TO CHANGE THE PATH FIRST
     */

    ui->speedDrop->addItem("x 0.5");
    ui->speedDrop->addItem("x 1.0");
    ui->speedDrop->addItem("x 1.5");
    ui->speedDrop->addItem("x 1.75");
    ui->speedDrop->addItem("x 2.0");
    //QDir myPath("C:/Users/user/CSCI3280-PROJ/P2Psystem/Music");
    QDir myPath("../P2Psystem/Music");
    myPath.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    myList = myPath.entryList();


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

void MainWindow::onplay(){

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
    qDebug() << "cur position" << cur_pos;
}

void MainWindow::onGetLength(QString leng)
{
    qDebug() << "transfer length is" << leng;
    ui->_length->setText(leng);
}

void MainWindow::on_playButton_clicked()
{
    float speed;
    qDebug() << "the speed selected" << ui->speedDrop->currentIndex() << ui->speedDrop->currentText();
    //QList<QListWidgetItem *> itemList = ui->songL->selectedItems();
    //int row= ui->songL->row(itemList[0]);f
    switch(ui->speedDrop->currentIndex()) {
        case 0 : speed = 0.5; // prints "1"
                 break;       // and exits the switch
        case 1 : speed = 1.0;
                 break;
        case 2 : speed = 1.5;
             break;
        case 3 : speed = 1.75;
             break;
        case 4 : speed = 2.0;
             break;
    }
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
        connect(p,SIGNAL(upLyric(const char*)),this,SLOT(onupLyric(const char*)));
        connect(p, SIGNAL(timeUpdate(unsigned int)), this, SLOT(ontimeUpdate(unsigned int)));
        connect(p, SIGNAL(sliderUpdate(unsigned int)), this, SLOT(onsliderUpdate(unsigned int)));
        ui->lyrics->setText("Lyrics");
        p->song=song;
        p->Stop=true;
        p->start();
        p->speed = speed;
        qDebug() << "The speed" << QString::number(p->speed, 'f', 2);
        connect(p, SIGNAL(GetLength(QString)), this, SLOT(onGetLength(QString)));
        ui->playButton->setText("stop");
    } else {
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

//void MainWindow::on_ProgressBar_sliderMoved(int position)
//{
//
//}

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

