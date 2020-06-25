#include <Windows.h>
#include <fstream>
#include <random>
#include <time.h>
#include <stdexcept>
#include <vector>
#include <map>
#include <set>
#include "winmain.h"
#include "cardgroup.h"
#include "player.h"
#include "cards.h"
#include "scene.h"
#include "game.h"

using namespace std;

Game::Game(HWND hwnd, int gamemode)
{
	times = 0;
	basispoint = 0;
	currentplayer = nullptr;
	lastplayer = nullptr;
	DiZhu = nullptr;
	calltimes = 0;
	callbegin = 0;
	hMainWnd = hwnd;
	status = gamemode * 5;
	for (int i = 0; i < 4; i++) {
		callscore[i] = 0;
		player[i] = new Player(*this);
	}
}

Game::~Game()
{
	for (int i = 0; i < 4; i++)
	{
		delete Player[i];
	}
}

void Gamestart()
{
	Initialization();
	if (!status) //判断到底是哪一个游戏
	{
		SendCard_doudizhu();
	}
	else
	{
		SendCard_gandengyan();
	}
	status++; //进入下一个阶段
	//下面几行是直接抄的样例的函数
	scene->HideDiscardBtn();
	scene->HideQuestionBtn();
	scene->DrawBackground();
	scene->ShowScene(hMainWnd);;
	SetTimer(hMainWnd, 1, 500, NULL);
}

void Initialization()
{
	DiZhu = currentplayer = lastplayer = nullptr;
	basispoint = calltimes = callbegin = 0;
	times = 1;
	if (status<5)
	{
		status = 0; //斗地主开始
	}
	else
	{
		status = 5; //干瞪眼开始
	}
	//下面几行是直接抄的样例的函数
	for (int i = 0; i < 3; ++i) {
		player[i]->NewGame();
	}
	cardheap.RandCards();
}

void Getscore()
{

}

void Putscore()
{

}

void SendCard()
{

}

void SendCard_doudizhu()
{

}

void SendCard_gandengyan()
{

}

void discard()
{

}

void pass()
{

}

void givinghint()
{

}

void Gameover()
{

}

void SendDiZhuCard()
{

}

void AskforDiZhu()
{

}

int GetnextPlayernum()
{
	for (int i = 0; i < 4; i++)
	{
		if (Player[i] == currentplayer)
			break;
	}
	return (i + 1) % 4;
}

int GetperviousPlayernum()
{
	for (int i = 0; i < 4; i++)
	{
		if (Player[i] == currentplayer)
			break;
	}
	return (i + 3) % 4;
}