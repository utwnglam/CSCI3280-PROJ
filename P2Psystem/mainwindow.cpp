#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QMediaPlayer>
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QListWidgetItem *list = ui->songL->;
    /*
     REMEMBER TO CHANGE THE PATH FIRST
     */
    QFile file("music_database.txt");
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"database not found",file.errorString());
    QTextStream in(&file);
    QString str2;
    while(!(in.atEnd()))
    {
        QString line = in.readLine();
        QStringList strlist  = line.split('\'');
        str2.append(strlist[3]+"\n");
        ui->songL->addItem(strlist[3]);
        ui->bandName->setText(strlist[5]);
        ui->albumName->setText(strlist[7]);
        ui->songName->setText(strlist[3]);
    }
    file.close();
    //int nCount = ui->songL->count();
    //for(int i=0;i<nCount;i++){
       // QListWidgetItem *item = ui->songL->item(i);
   // }

    //QString lyric = this->getLyric(currentSongInfo.lyricLink);//get lyrics from web
     //lyricList = this->parse(lyric);//get lyrics sentence by sentence to lyricList
    //list->setBackgroundColor(Qt::red);
    //mPlayer = new QMediaPlayer(this);
    //connect(mPlayer,&Qplayer::positionchanged,[&](qint64 pos){
       // ui->avance->setValue(pos);
   //});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Add_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,"Add");
    if(filename==NULL){
        return;
    }
    ui->songL->addItem(filename);
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
    if(ui->playButton->text()=="play"){
        //mPlayer -> play();
        ui->playButton->setText("stop");
    }else{
        //mPlayer->stop();
        ui->playButton->setText("play");
    }
}

void MainWindow::on_songL_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    //QList<QListWidgetItem *> itemList = ui->songL->selectedItems();
    //int row= ui->songL->row(itemList[0]);
    ui->bandName->setText(current->text());
    ui->albumName->setText(current->text());
    ui->songName->setText(current->text());
    //if( NULL == current )   //dont handle null ptr
    //{
     //   return;
    //}
    //ui->bandName->setText(current);
    //ui->albumName->setText(current);
    //ui->songName->setText(current);
    //get the next text and show to the strtooltip
    //QString strToolTip = current->toolTip();
    //ui->lineEditToolTip->setText( strToolTip );
    //
}
