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
	status = gamemode * 5;//gamemode 0 赋值status为0 SendCardPeriod_doudizhu，gamemode 1 赋值status为5 SendCardPeriod_gandengyan

	//然后我想在这里传一个是一副牌还是两副牌的参数给card类

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
	status++;
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
	ifstream in;
	in.open("data.txt");//随便写的名字
	if (in.is_open())
	{
		for (int i = 0; i < 4; i++)
		{
			in >> player[i]->score; //等着player类写出来改
		}
	}
}

void Putscore()
{
	ofstream out;
	out.open("data.txt");//随便写的名字
	if (out.is_open())
	{
		for (int i = 0; i < 4; i++)
		{
			out << player[i]->score<<"\n"; //等着player类写出来改
		}
	}
}

void SendCard()
{
	if (Paidui.remain() >= 4) //等着card类写出来改
	{
		for (int i = 0; i < 4; i++)
		{
			player[i]->addcard(Paidui.getcard()); //等着palyer类和card类写出来改
		}
	}
	else
	{
		while (Paidui.remain())//等着card类写出来改
		{
			currentplayer->addcard(Paidui.getcard());//等着card类写出来改
			currentplayer = player[GetnextPlayernum()];
		}
	}
}

void SendCard_doudizhu()
{
	while (Paidui.remain() > 8)//等着card类写出来改
	{
		SendCard();
	}
	for (int i = 0; i < 8; i++)
	{
		DiZhuCard[i] = Paidui.getcard();//等着card类写出来改
	}
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
	for (int i = 0; i < 8; i++)
	{
		DiZhu->addcard(DiZhuCard[i]);
	}
	//下面是抄的
	scene->DrawBackground();//产生了地主，将地主牌正面显示
	scene->ShowScene(hMainWnd);
	status = DiscardPeriod_doudizhu;//当前状态为出牌阶段
	SetTimer(hMainWnd, 1, 500, NULL);
}

void AskforDiZhu()
{
	if (!calltimes) //初始化
	{
		default_random_engine   e;
		e.seed(time(NULL));
		uniform_int_distribution<unsigned>  u(0, 3);
		callbegin = u(e);
		currentplayer = player[u(e)];
		status = AskDiZhuPeriod_doudizhu;
	}

	if (!DiZhu&&4==calltimes) //判断是否需要重新发牌，如果没人叫地主的话
	{
		status = SendCardPeriod_doudizhu;
	}

	if (!currentplayer->ishuman()) //乱写的函数，意思就是判断这个是不是人，这里意思是如果不是人的话就调用自动叫地主的函数
	{
		calltimes++;
		if (某某函数)
		{ 
			DiZhu = currentplayer;
			status = SendDiZhuCardPeriod_doudizhu;
			SetTimer(hMainWnd, 1, 500, NULL);
		}
		else
		{
			currentplayer= player[GetnextPlayernum()];
		}
	}
	else //是人的话
	{
		scene->ShowQuestionBtn();
		return;
	}
}

void GetideasofDizhu(bool idea)
{
	calltimes++;
	if (idea)
	{
		DiZhu = currentplayer;
		status = SendDiZhuCardPeriod_doudizhu;
		SetTimer(hMainWnd, 1, 500, NULL);
	}
	else
	{
		currentplayer = player[GetnextPlayernum()];
	}
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