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
    void onplay();
    void onupLyric(const char *);

private slots:
    void on_Add_clicked();
    void on_Del_clicked();
    void on_playButton_clicked();

    void on_ProgressBar_sliderMoved(int position);

    void on_searchBar_textChanged(const QString &arg1);

    void on_songL_itemDoubleClicked(QListWidgetItem *item);

    //void on_ProgressBar_valueChanged(int value);
    void on_connectButton_clicked();
    void on_disButton_clicked();
    void on_p2pButton_clicked();

    //play();

private:
    Ui::MainWindow *ui;
    QStringList myList;
    p2psocket *socket;
    playthread *m_thread;
    playthread *p;
};

#endif // MAINWINDOW_H
