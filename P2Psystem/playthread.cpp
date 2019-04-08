#define _CRT_SECURE_NO_WARNINGS
#undef UNICODE
#pragma comment(lib, "ws2_32.lib")
#define MY_PORT 3434
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"msacm32.lib")
#include <Windows.h>
#include <iostream>
#include <mmsystem.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "playthread.h"
#include <cstdio>
#include <ctime>
#include <QtCore>
#include <QDebug>
#include <QStringList>

playthread::playthread(QObject *parent):QThread(parent)
{
    qDebug()<<"Start playthread";
}

void playthread::run(){

    if(this->Stop){
        playwave();
        if(!this->Stop){
            exit(0);
        }
    }

    qDebug()<< "Stop";
    exec();
}

const char * playthread::readLine(FILE *file) {

    if (file == NULL) {
        printf("Error: file pointer is null.");
        exit(1);
    }

    int maximumLineLength = 128;
    char *lineBuffer = (char *)malloc(sizeof(char) * maximumLineLength);

    if (lineBuffer == NULL) {
        printf("Error allocating memory for line buffer.");
        exit(1);
    }

    char ch = getc(file);
    int count = 0;

    while ((ch != '\n') && (ch != EOF)) {
        if (count == maximumLineLength) {
            maximumLineLength += 128;
            lineBuffer = (char*)realloc(lineBuffer, maximumLineLength);
            if (lineBuffer == NULL) {
                printf("Error reallocating space for line buffer.");
                exit(1);
            }
        }
        lineBuffer[count] = ch;
        count++;

        ch = getc(file);
    }

    lineBuffer[count] = '\0';
    char* line = (char*)malloc(sizeof(char) * (count + 1));
    strncpy(line, lineBuffer, (count + 1));
    free(lineBuffer);
    const char *constLine = line;
    return constLine;
}

int playthread::calculatingTime(char min[], char sec[], double speed)
{
    int minutes = atoi(min);
    int seconds = atoi(sec);

    int timeCalculated = (int)((minutes * 60 + seconds)/ speed);
    return timeCalculated;
}

void playthread::playwave(){
    std::string song1=this->song.toStdString().c_str();
    //std::string path="C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\"+song1+".wav";
    std::string path="../P2Psystem/Music/"+song1+".wav";
    std::string pathLyric="../P2Psystem/Lyric/"+song1+".txt";
    LPSTR szFileName;
    LPTSTR szPathName;
    MMCKINFO mmckinfoParent;
    MMCKINFO mmckinfoSubChunk;
    DWORD dwFmtSize;
    DWORD m_WaveLong;
    WAVEFORMATEX* lpFormat;
    DWORD m_dwDataOffset;
    DWORD m_dwDataSize;
    WAVEOUTCAPS pwoc;
    LONG lSoundOffset;

    LONG lSoundLong;

    //szFileName = (LPTSTR)filename;
    szPathName = (LPTSTR)path.c_str();

    float speed = this->speed;

    HMMIO m_hmmio;
    //printf("%s\n", szPathName);
    // lyrics start
    const char* lyricPath = pathLyric.c_str();
    char mins[3];
    char secs[3];
    char timeBuff[12];
    int nextLyricTime;
    const char *line;
        FILE * fp = fopen(lyricPath, "r");
        if (fp == NULL)
        {
            qDebug() <<"Error while opening the file.\n";
        }
        else
        {

            fgets(timeBuff, 11, (FILE*)fp);
            line = readLine(fp);

            mins[0] = timeBuff[1];
            mins[1] = timeBuff[2];
            mins[2] = 0;
            secs[0] = timeBuff[4];
            secs[1] = timeBuff[5];
            secs[2] = 0;
            qDebug() << mins[0];
            qDebug() << mins[1];
            qDebug() << secs[0];
            qDebug() << secs[1];

            nextLyricTime = calculatingTime(mins, secs, speed);
            qDebug() << nextLyricTime;
        }

    // lyrics end
    if (!(m_hmmio = mmioOpen(szPathName, NULL, MMIO_READ)))
    {
        printf("fail in mmioOpen\n");
    }

    mmckinfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    if (mmioDescend(m_hmmio, (LPMMCKINFO)& mmckinfoParent, NULL, MMIO_FINDRIFF))
    {
        printf("fail in mmioDescend:WAVE\n");
    }
    mmckinfoSubChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if (mmioDescend(m_hmmio, &mmckinfoSubChunk, &mmckinfoParent,
        MMIO_FINDCHUNK))
    {
        printf("fail in mmioDescend:fmt\n");
    }

    dwFmtSize = mmckinfoSubChunk.cksize;
    lpFormat = (WAVEFORMATEX *)LocalLock(LocalAlloc(LMEM_MOVEABLE, LOWORD(dwFmtSize)));
    if ((unsigned long)mmioRead(m_hmmio, (HPSTR)lpFormat, dwFmtSize) != dwFmtSize)
    {
        printf("fail in mmioRead\n");
    }
    mmioAscend(m_hmmio, &mmckinfoSubChunk, 0);
    mmckinfoSubChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if (mmioDescend(m_hmmio, &mmckinfoSubChunk, &mmckinfoParent, MMIO_FINDCHUNK))
    {
        printf("fail in mmioDescend:data\n");
    }
    m_dwDataSize = mmckinfoSubChunk.cksize;
    m_dwDataOffset = mmckinfoSubChunk.dwDataOffset;
    if (m_dwDataSize == 0L)
    {
        printf("the file has no data!");
    }

    char* lpData = (char *)malloc(sizeof(char) * m_dwDataSize);
    if (!lpData)
    {
        printf("Alloc memory for wave data failed!");
    }


    lSoundOffset = m_dwDataOffset;
    LONG lSize = mmioSeek(m_hmmio, lSoundOffset, SEEK_SET);
    int value;
    if (mmioRead(m_hmmio, lpData, m_dwDataSize) != m_dwDataSize)
    {
        printf("something wrong in mmioRead\n");
    }

    HWAVEOUT hWaveOut;
    lpFormat->nSamplesPerSec *= speed; // revise this variable for tuning the speed
    waveOutOpen(&hWaveOut, WAVE_MAPPER, lpFormat, 0L, 0L, WAVE_FORMAT_DIRECT);

    WAVEHDR WaveOutHdr;
    WaveOutHdr.lpData = (LPSTR)lpData;
    WaveOutHdr.dwBufferLength = m_dwDataSize;
    WaveOutHdr.dwBytesRecorded = 0;
    WaveOutHdr.dwUser = 0L;
    WaveOutHdr.dwFlags = 0L;
    WaveOutHdr.dwLoops = 0L;
    waveOutPrepareHeader(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR));
    int minutes=0, sec=0, total_sec;
    minutes = (int)(m_dwDataSize/lpFormat->nAvgBytesPerSec/60);
    sec = (int)(m_dwDataSize/lpFormat->nAvgBytesPerSec%60);
    total_sec = (int)(m_dwDataSize/lpFormat->nAvgBytesPerSec);
    qDebug() << "the length of a song is" << m_dwDataSize <<"/" << lpFormat->nAvgBytesPerSec <<"is" << minutes << ":" << sec;
    QString length;
    if(sec < 10)    // add the zero
        length = QString::number(minutes)+":0"+QString::number(sec);
    else
        length = QString::number(minutes)+":"+QString::number(sec);
    emit GetLength(length);
    std::clock_t start;
    double duration;
    int real_time;
    int time_temp = 0;
    double test;
    start = std::clock();
    do {
        duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
        //update time and slider
        real_time = int(duration)*speed;
        if(real_time > time_temp)   // when it updates
        {
            emit timeUpdate(real_time);
            test = ((double)real_time/(double)total_sec)*100;
            //qDebug() << real_time << "/" << total_sec << "*100= the slider pos" << QString::number(test, 'f', 2);
            emit sliderUpdate((int)test);
        }
        time_temp = real_time;
        if(fp != NULL)
        {
            if (nextLyricTime == (int)duration)
            {
                nextLyricTime--;
                qDebug() << line;
                //update lyric;
                emit upLyric(line);
                fgets(timeBuff, 11, (FILE*)fp);
                line = readLine(fp);
                mins[0] = timeBuff[1];
                mins[1] = timeBuff[2];
                secs[0] = timeBuff[4];
                secs[1] = timeBuff[5];
                nextLyricTime = calculatingTime(mins, secs, speed);
            }
        }

        if(!this->Stop){
           break;
        }
    } while (waveOutUnprepareHeader(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
    waveOutReset(hWaveOut);
    waveOutClose(hWaveOut);
}

