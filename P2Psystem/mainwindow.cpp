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
    /*
    ui->songName->setText(current->data(Qt::UserRole + 1).toString());
    ui->bandName->setText(current->data(Qt::UserRole + 2).toString());
    ui->albumName->setText(current->data(Qt::UserRole + 3).toString());

     * FOR TRANSLATING QSTRING TO CHAR*
    QString temp = current->data(Qt::UserRole).toString();
    QByteArray byteArray = temp.toLocal8Bit();
    char *c = byteArray.data();
    kernel->playMusic(c, 1.0);
    */
}

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
    ui->bandName->setText(item->data(Qt::UserRole + 2).toString());;
    ui->albumName->setText(item->data(Qt::UserRole + 3).toString());;
}
