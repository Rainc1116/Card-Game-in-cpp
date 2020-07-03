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

Game::Game(HWND hwnd, int gamemode，int robotnum)
{
	times = 0;
	basispoint = 0;
	currentplayer = nullptr;
	lastplayer = nullptr;
	DiZhu = nullptr;
	calltimes = 0;
	callbegin = 0;
	hMainWnd = hwnd;
	robotnumber = robotnum;
	Getscore();
	if (!gamemode)
	{
		status = SendCardPeriod_doudizhu;

	}
	else
	{
		status = SendCardPeriod_gandengyan;

	}
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

void Game::Gamestart()
{
	Initialization();
	if (status<5) //判断到底是哪一个游戏
	{
		SendCard_doudizhu();
	}
	else
	{
		SendCard_gandengyan();
	}
	status++;

	scene->HideDiscardBtn();
	scene->HideQuestionBtn();
	scene->DrawBackground();
	scene->ShowScene(hMainWnd);;
	SetTimer(hMainWnd, 1, 500, NULL);
}

void Game::Initialization()
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
	
	for (int i = 0; i < 3; ++i) {
		player[i]->NewGame();
	}
	cardheap.RandCards();
}

void Game::Getscore()
{
	ifstream in;
	in.open("data.txt");
	if (in.is_open())
	{
		for (int i = 0; i < 4; i++)
		{
			in >> score[i]; 
		}
	}
}

void Game::Putscore()
{
	ofstream out;
	out.open("data.txt");
	if (out.is_open())
	{
		for (int i = 0; i < 4; i++)
		{
			out << score[i] <<"\n"; 
		}
	}
}

void Game::SendCard()
{
	if (Paidui.remain() >= 4) 
	{
		for (int i = 0; i < 4; i++)
		{
			player[i]->AddCard(Paidui.getcard()); 
		}
	}
	else
	{
		while (Paidui.remain())
		{
			currentplayer->AddCard(Paidui.getcard());
			currentplayer = player[GetnextPlayernum()];
		}
	}
}

void Game::SendCard_doudizhu()
{
	while (Paidui.remain() > 8)
	{
		SendCard();
	}
	for (int i = 0; i < 8; i++)
	{
		DiZhuCard[i] = Paidui.getcard();
	}
}

void Game::SendCard_gandengyan()
{
	default_random_engine   e;
	e.seed(time(NULL));
	uniform_int_distribution<unsigned>  u(0, 3);
	for (i = 0; i < 5; i++)
	{
		SendCard();
	}
	player[u(e)]->AddCard(Paidui.getcard());
	currentplayer = player[u(e)];
}

void Game::discard()
{
	currentplayer->discard.Clear();
	currentplayer->nodiscard = false;
	if (lastplayer == currentplayer && status >= 5) //干瞪眼中无人压死则各摸一张牌
	{
		sendcard();
	}
	if (GetcurrentPlayernum() < (4 - robotnumber)) {//当前玩家为人
		if (currentplayer->selection.count && currentplayer->IsRight()) {//玩家已选牌并且符合规定
			scene->HideDiscardBtn();
			lastplayer = currentplayer;
			//倍率增加环节
			{
				if (curplayer->discard.type == BombFour)//炸弹4
					times *= 2;
				else if (curplayer->discard.type == BombFive)//炸弹5
					times *= 6;
				else if (curplayer->discard.type == BombSix)//炸弹6
					times *= 24;
				else if (curplayer->discard.type == BombSeven)//炸弹7
					times *= 120;
				else if (curplayer->discard.type == BombEight)//炸弹8
					times *= 720;
				else if (curplayer->discard.type == BombFour)//炸弹king
					times *= 100;
			}
		}
		else {//否则继续等待玩家选牌
			scene->ShowScene(hMainWnd);
			scene->ShowDiscardBtn();
			return;
		}
	}
	else {//当前出牌方为电脑
		if (!currentplayer->AIPass())
			lastplayer = currentplayer;
		currentplayer->AISelect(); /////////不知道参数是什么
		//倍率增加环节
		{
			if (curplayer->discard.type == BombFour)//炸弹4
				times *= 2;
			else if (curplayer->discard.type == BombFive)//炸弹5
				times *= 6;
			else if (curplayer->discard.type == BombSix)//炸弹6
				times *= 24;
			else if (curplayer->discard.type == BombSeven)//炸弹7
				times *= 120;
			else if (curplayer->discard.type == BombEight)//炸弹8
				times *= 720;
			else if (curplayer->discard.type == BombFour)//炸弹king
				times *= 100;
		}
	}
	scene->ShowScene(hMainWnd);
	if (currentplayer->cards.empty())//当前出牌方已无手牌
	{
        if (status<5)
	        status = EndingPeriod_doudizhu;//doudizhu游戏结束
	    if(status>=5)
	        status = EndingPeriod_gandengyan;//gandengyan游戏结束
    }
	else
		currentplayer = player[GetnextPlayernum()];//下家继续出牌
	SetTimer(hMainWnd, 1, 500, NULL);
}

void Game::pass()
{
	currentplayer->pass();
	currentplayer = player[GetnextPlayernum()];
	scene->ShowScene(hMainWnd);
	SetTimer(hMainWnd, 1, 500, NULL);
}

void Game::givinghint()
{
	currentplayer->selection.Clear();
	currentplayer->AISelect(); ////////不知道参数是什么
	if (currentplayer->selection.count != 0)
		PostMessage(scene->discand, WM_MYBUTTON, TRUE, 0);
	InvalidateRect(hMainWnd, NULL, FALSE);
}

void Game::Gameover()
{
	if (status = 4)
	{   int flag = -1; //判断是不是地主获胜
		if (DiZhu == currentplayer)
		{   flag = 1;
			for (int i = 0; i < 3; i++)
			{   if (player[i] == DiZhu)
					score[i] += 3 * times;
				else
					score[i] -= times;			}	}
		else
		{   flag = 0;
			for (int i = 0; i < 3; i++)
			{   if (player[i] == DiZhu)
					score[i] -= 3 * times;
				else
					score[i] += times;			}	}
		scene->ShowScene(hMainWnd);
		if (flag==1)	MessageBox(hMainWnd, TEXT("恭喜，地主获胜！"), TEXT("游戏结束"), 0);
		else	MessageBox(hMainWnd, TEXT("恭喜，农民获胜！"), TEXT("游戏结束"), 0);
		GameStart();	}
	else
	{   int j;
				for (int i = 0; i < 3; i++)
				{   if (player[i] == currentplayer)
					{   score[i] += 3 * times;
						j = i;					}
					else
						score[i] -= times;				}
		scene->ShowScene(hMainWnd);
		switch (j)
		{case 0:MessageBox(hMainWnd, TEXT("恭喜,玩家0获胜！"), TEXT("游戏结束"), 0); break;
		case 1:MessageBox(hMainWnd, TEXT("恭喜,玩家1获胜！"), TEXT("游戏结束"), 0); break;
		case 2:MessageBox(hMainWnd, TEXT("恭喜,玩家2获胜！"), TEXT("游戏结束"), 0); break;
		case 3:MessageBox(hMainWnd, TEXT("恭喜,玩家3获胜！"), TEXT("游戏结束"), 0); break;}			
		GameStart();}
}

void Game::SendDiZhuCard()
{
	for (int i = 0; i < 8; i++)
	{
		DiZhu->AddCard(DiZhuCard[i]);
	}
	scene->DrawBackground();//产生了地主，将地主牌正面显示
	scene->ShowScene(hMainWnd);
	status = DiscardPeriod_doudizhu;//当前状态为出牌阶段
	currentplayer = DiZhu;
	lastplayer = DiZhu;
	SetTimer(hMainWnd, 1, 500, NULL);
}

void Game::AskforDiZhu()
{
	if (!calltimes) //初始化
	{
		default_random_engine   e;
		e.seed(time(NULL));
		uniform_int_distribution<unsigned>  u(0, 3);
		currentplayer = player[u(e)];
		status = AskDiZhuPeriod_doudizhu;
	}

	if (!DiZhu&&4==calltimes) //判断是否需要重新发牌，如果没人叫地主的话
	{
		status = SendCardPeriod_doudizhu;
		SetTimer(hMainWnd, 1, 500, NULL);
		calltimes = 0;
		return;
	}

	if (GetcurrentPlayernum()>=(4-robotnumber)) //意思就是判断这个是不是人，这里意思是如果不是人的话就调用自动叫地主的函数
	{  
		calltimes++;
		if (currentplayer->AICall()) //判断是否叫地主的函数
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

void Game::GetideasofDizhu(bool idea)
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

int Game::GetnextPlayernum()
{
	for (int i = 0; i < 4; i++)
	{
		if (Player[i] == currentplayer)
			break;
	}
	return (i + 1) % 4;
}

int Game::GetcurrentPlayernum()
{
	for (int i = 0; i < 4; i++)
	{
		if (Player[i] == currentplayer)
			break;
	}
	return i;
}