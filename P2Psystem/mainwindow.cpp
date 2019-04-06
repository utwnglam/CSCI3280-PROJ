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
    QString fileName;
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
    QFile file("/Users/JoanneCheung/Desktop/3280 PROJ/P2Psystem/music_database.txt");
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"database not found",file.errorString());

    //INPUTING DATABASE INTO ARRAY
    QTextStream in(&file);
    while(!(in.atEnd()))
    {
        QString line = in.readLine();
        QStringList strlist = line.split('\'');

        QListWidgetItem *pItem = new QListWidgetItem(ui->songL);
        pItem->setData(Qt::UserRole, strlist[1]);
        pItem->setData(Qt::UserRole + 1, strlist[3]);
        pItem->setData(Qt::UserRole + 2, strlist[5]);
        pItem->setData(Qt::UserRole + 3, strlist[7]);
        pItem->setText(strlist[3]);
        ui->songL->addItem(pItem);

        /*
        fileName is strlist[1], index: Qt::UserRole
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
    ui->songName->setText(current->data(Qt::UserRole + 1).toString());
    ui->bandName->setText(current->data(Qt::UserRole + 2).toString());;
    ui->albumName->setText(current->data(Qt::UserRole + 3).toString());;
}

void MainWindow::on_ProgressBar_sliderMoved(int position)
{

}
