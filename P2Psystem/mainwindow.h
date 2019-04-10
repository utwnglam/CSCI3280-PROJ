#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QListWidget>
#include <QDir>
#include <QDebug>
#include <QRegExp>
#include <QTcpSocket>
#include "p2psocket.h"
#include "playthread.h"
#include "updatabase.h"

namespace Ui {
class MainWindow;
}

//class QMediaPlayer;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //playthread *p;

public slots:
    void onupLyric(const char *);
    void onGetLength(QString);
    void ontimeUpdate(unsigned int);
    void onsliderUpdate(unsigned int);

private slots:
    void on_Add_clicked();
    void on_Del_clicked();
    void on_playButton_clicked();

    //void on_ProgressBar_sliderMoved(int position);

    void on_searchBar_textChanged(const QString &arg1);

    void on_songL_itemDoubleClicked(QListWidgetItem *item);


    void on_Edit_clicked();

    //void on_ProgressBar_valueChanged(int value);
    void adding_new_song();
    void on_connectButton_clicked();
    void on_disButton_clicked();
    void on_p2pButton_clicked();

    //play();

    void delete_nonlocal_song();
    //void on_ProgressBar_actionTriggered(int action);

    void on_pushButton_clicked();
    void delete_duplicates();

    void on_download_clicked();



private:
    Ui::MainWindow *ui;
    QStringList myList;
    updatabase *database;
    p2psocket *socket;
    playthread *m_thread;
    playthread *p;
};

#endif // MAINWINDOW_H
