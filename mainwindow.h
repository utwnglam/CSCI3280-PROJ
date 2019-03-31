#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QListWidget>

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
    void on_songL_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::MainWindow *ui;
   // QMediaPlayer *mPlayer;
};

#endif // MAINWINDOW_H
