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

#include "playthread.h"

#include <QtCore>
#include <QDebug>
#include <QStringList>

playthread::playthread(QObject *parent):QThread(parent)
{
    qDebug()<<"Start playthread";
}

void playthread::run(){


    //std::string path="C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\"+song1+".wav";
    //qDebug()<< song;
    if(this->Stop){
        playwave();
        if(!this->Stop){
            exit(0);
        }
    }

    qDebug()<< "Stop";
    exec();
}

void playthread::playwave(){
    std::string song1=this->song.toStdString().c_str();
    //std::string path="C:\\Users\\user\\CSCI3280-PROJ\\P2Psystem\\Music\\"+song1+".wav";
    std::string path="../P2Psystem/Music/"+song1+".wav";

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
    float speed = 1.0;
    HMMIO m_hmmio;
    printf("%s\n", szPathName);
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
    int min=0, sec=0, total_sec;
    min = (int)(m_dwDataSize/lpFormat->nAvgBytesPerSec/60);
    sec = (int)(m_dwDataSize/lpFormat->nAvgBytesPerSec%60);
    total_sec = (int)(m_dwDataSize/lpFormat->nAvgBytesPerSec);
    qDebug() << "the length of a song is" << m_dwDataSize <<"/" << lpFormat->nAvgBytesPerSec <<"is" << min << ":" << sec;
    int slider_pos = 0;
    //ui->ProgressBar->setValue(slider_pos);
    //slider_pos = int(current_sec/total_sec) * 100; // update the slider position by current playing time
    //update timer label
    //ui->_length->setText(min + ":" + sec);
    //ui->timer->setText(current_sec/60 + ":" + current_sec%60);
    int count=0;
    do {
        if(!this->Stop){
            qDebug()<<this->Stop;
           break;
        }
    } while (waveOutUnprepareHeader(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
    waveOutReset(hWaveOut);
    waveOutClose(hWaveOut);
}
