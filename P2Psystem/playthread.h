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
    const char *readLine(FILE *file);
    int calculatingTime(char min[], char sec[], double speed);
signals:
    void upLyric(const char *);
    void GetLength(QString);
    void timeUpdate(unsigned int);
    void sliderUpdate(unsigned int);

public slots:
    void playwave();

private:
    //std::string song1;
    //QString song;
    //int exist;

};

#endif // PLAYTHREAD_H
