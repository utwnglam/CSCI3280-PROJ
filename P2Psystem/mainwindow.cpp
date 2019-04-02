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

class Songs{
public:
    QString fileName, songName, albumName, bandName;
    Songs* next;
};

Songs* DataBase;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    /*
     REMEMBER TO CHANGE THE PATH FIRST
     */
    QFile file("/Users/JoanneCheung/Desktop/3280 PROJ/P2Psystem/music_database.txt");
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"database not found",file.errorString());

    //INPUTING DATABASE INTO ARRAY
    int databaseFlag = 1;
    Songs* current;
    QTextStream in(&file);
    while(!(in.atEnd()))
    {
        QString line = in.readLine();
        QStringList strlist  = line.split('\'');
        ui->songL->addItem(strlist[3]);
        Songs newSong;
        newSong.fileName = strlist[1];
        newSong.songName = strlist[3];
        newSong.bandName = strlist[5];
        newSong.albumName = strlist[7];

        if(databaseFlag == 1) {
            DataBase = &newSong;
            current = &newSong;
            databaseFlag = 0;
        } else {
            current->next = &newSong;
            current = current->next;
        }

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
    connect(mPlayer,&Qplayer::positionchanged,[&](qint64 pos){
        ui->avance->setValue(pos);
    });
    */
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
    if(ui->playButton->text() == "play"){
        //mPlayer -> play();
        ui->playButton->setText("stop");
    } else {
        //mPlayer->stop();
        ui->playButton->setText("play");
    }
}

void MainWindow::on_songL_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Songs* songlistCurrent = DataBase;
    /*
    while (songlistCurrent->songName.compare(current->text()) != 0 && (songlistCurrent->next != NULL)) {
        songlistCurrent = songlistCurrent->next;
    }
    ui->bandName->setText(songlistCurrent->bandName);
    ui->albumName->setText(songlistCurrent->albumName);
    ui->songName->setText(songlistCurrent->songName);
    */
    ui->bandName->setText(current->text());
    ui->albumName->setText(current->text());
    ui->songName->setText(current->text());
}
