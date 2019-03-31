#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <iostream> 
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"msacm32.lib")
int main()
{
	LPSTR szFileName;
	LPSTR szPathName;
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

	char filename[100] = "ZenZenZense.wav";
	//szFileName = (LPTSTR)filename;
	szPathName = (LPTSTR)filename;
	HMMIO m_hmmio;
	if (!(m_hmmio = mmioOpen(szPathName, NULL, MMIO_READ)))
	{
		printf("fail in mmioOpen\n");
	}

	mmckinfoParent.fccType = mmioFOURCC('W', 'A','V', 'E');
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
	lpFormat->nSamplesPerSec *= 1; // revise this variable for tunning the speed
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

	do {} while (waveOutUnprepareHeader(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
	waveOutClose(hWaveOut);

	
}
