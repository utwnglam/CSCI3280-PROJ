#ifndef PLAYTHREAD_H
#define PLAYTHREAD_H
//#include <QThread>
#include <QtCore>

class playthread : public QThread
{
    Q_OBJECT
public:
    explicit playthread(QObject *parent =0);
    //~playthread();
    void run();
    bool Stop;
    QString song;

signals:
    void play();

public slots:
    void playwave();

private:
    //std::string song1;
    //QString song;
    //int exist;
};

#endif // PLAYTHREAD_H
