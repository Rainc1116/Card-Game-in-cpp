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
	if (status<5) //判断到底是哪一个游戏
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
	default_random_engine   e;
	e.seed(time(NULL));
	uniform_int_distribution<unsigned>  u(0, 3);
	for (i = 0; i < 5; i++)
	{
		SendCard();
	}
	player[u(e)]->addcard(Paidui.getcard());
	currentplayer = player[u(e)];
}

void discard()
{
	currentplayer->discard.Clear();
	currentplayer->nodiscard = false;

	if (lastplayer == currentplayer && status >= 5) //干瞪眼中无人压死则各摸一张牌
	{
		sendcard();
	}

	if (currentplayer->ishuman()) {//当前玩家为人
		if (currentplayer->selection.count && currentplayer->HumanDiscard()) {//玩家已选牌并且符合规定
			scene->HideDiscardBtn();
			lastplayer = currentplayer;
			//倍率增加环节
		}
		else {//否则继续等待玩家选牌
			scene->ShowScene(hMainWnd);
			scene->ShowDiscardBtn();
			return;
		}
	}
	else {//当前出牌方为电脑
		currentplayer->SelectCards();
		if (currentplayer->Discard())
			lastplayer = currentplayer;
		//倍率增加环节
	}
	scene->ShowScene(hMainWnd);

	if (currentplayer->cards.empty())//最后出牌方已无手牌
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

void pass()
{
	currentplayer->pass();
	currentplayer = player[GetnextPlayernum()];
	scene->ShowScene(hMainWnd);
	SetTimer(hMainWnd, 1, 500, NULL);
}

void givinghint()
{

}

void Gameover()
{
	if (status = 4)
	{
		int flag = -1;
		if (DiZhu == currentplayer)
		{
			flag = 1;
			DiZhu->score +=3 * times;
			for (int i = 0; i < 2; i++)
			{
				currentplayer = player[GetnextPlayernum()];
				currentplayer->score -= times;
			}
		}
		else
		{
			flag = 0;
			DiZhu->score -= 5 * times;
			for (int i = 0; i < 3; i++)
			{
				currentplayer = player[GetnextPlayernum()];
				currentplayer->score += times;
			}
		}
		scene->ShowScene(hMainWnd);
		if (flag==1)
			MessageBox(hMainWnd, TEXT("恭喜，地主获胜！"), TEXT("游戏结束"), 0);
		else
			MessageBox(hMainWnd, TEXT("很遗憾，农民获胜！"), TEXT("游戏结束"), 0);
		GameStart();
	}
	else
	{
			currentplayer->score += 3 * times;
			for (int i = 0; i < 2; i++)
			{
				currentplayer = player[GetnextPlayernum()];
				currentplayer->score -= times;
			}
			int i = GetnextPlayernum();
		scene->ShowScene(hMainWnd);
		switch (i)
		{
		case 0:MessageBox(hMainWnd, TEXT("恭喜,玩家0获胜！"), TEXT("游戏结束"), 0); break;
		case 1:MessageBox(hMainWnd, TEXT("恭喜,玩家1获胜！"), TEXT("游戏结束"), 0); break;
		case 2:MessageBox(hMainWnd, TEXT("恭喜,玩家2获胜！"), TEXT("游戏结束"), 0); break;
		case 3:MessageBox(hMainWnd, TEXT("恭喜,玩家3获胜！"), TEXT("游戏结束"), 0); break;
		}			
		GameStart();
	}
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
	currentplayer = DiZhu;
	lastplayer = DiZhu;
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