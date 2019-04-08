#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QListWidget>
#include <QDir>
#include <QDebug>
#include <QRegExp>

//#include "./playWave.hpp"

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

private slots:
    void on_Add_clicked();
    void on_Del_clicked();
    void on_playButton_clicked();

    void on_ProgressBar_sliderMoved(int position);

    void on_searchBar_textChanged(const QString &arg1);

    void on_songL_itemDoubleClicked(QListWidgetItem *item);

    void on_Edit_clicked();

private:
    Ui::MainWindow *ui;
    QStringList myList;
};

#endif // MAINWINDOW_H
