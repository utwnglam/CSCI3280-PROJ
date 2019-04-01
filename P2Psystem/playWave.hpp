#pragma once
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include <string>
#include <fstream>
#include <stdio.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"msacm32.lib")

using namespace std;

class WavePlayer
{
public:
	WavePlayer() {};
	~WavePlayer() {};

	void playMusic(char musicpath[], float speed);
	void stopPlaying();
};