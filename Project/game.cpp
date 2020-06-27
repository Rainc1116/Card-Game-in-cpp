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
	status = gamemode * 5;//gamemode 0 ��ֵstatusΪ0 SendCardPeriod_doudizhu��gamemode 1 ��ֵstatusΪ5 SendCardPeriod_gandengyan

	//Ȼ�����������ﴫһ����һ���ƻ��������ƵĲ�����card��

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
	if (!status) //�жϵ�������һ����Ϸ
	{
		SendCard_doudizhu();
	}
	else
	{
		SendCard_gandengyan();
	}
	status++;
	//���漸����ֱ�ӳ��������ĺ���
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
		status = 0; //��������ʼ
	}
	else
	{
		status = 5; //�ɵ��ۿ�ʼ
	}
	//���漸����ֱ�ӳ��������ĺ���
	for (int i = 0; i < 3; ++i) {
		player[i]->NewGame();
	}
	cardheap.RandCards();
}

void Getscore()
{
	ifstream in;
	in.open("data.txt");//���д������
	if (in.is_open())
	{
		for (int i = 0; i < 4; i++)
		{
			in >> player[i]->score; //����player��д������
		}
	}
}

void Putscore()
{
	ofstream out;
	out.open("data.txt");//���д������
	if (out.is_open())
	{
		for (int i = 0; i < 4; i++)
		{
			out << player[i]->score<<"\n"; //����player��д������
		}
	}
}

void SendCard()
{
	if (Paidui.remain() >= 4) //����card��д������
	{
		for (int i = 0; i < 4; i++)
		{
			player[i]->addcard(Paidui.getcard()); //����palyer���card��д������
		}
	}
	else
	{
		while (Paidui.remain())//����card��д������
		{
			currentplayer->addcard(Paidui.getcard());//����card��д������
			currentplayer = player[GetnextPlayernum()];
		}
	}
}

void SendCard_doudizhu()
{
	while (Paidui.remain() > 8)//����card��д������
	{
		SendCard();
	}
	for (int i = 0; i < 8; i++)
	{
		DiZhuCard[i] = Paidui.getcard();//����card��д������
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
	//�����ǳ���
	scene->DrawBackground();//�����˵�������������������ʾ
	scene->ShowScene(hMainWnd);
	status = DiscardPeriod_doudizhu;//��ǰ״̬Ϊ���ƽ׶�
	SetTimer(hMainWnd, 1, 500, NULL);
}

void AskforDiZhu()
{
	if (!calltimes) //��ʼ��
	{
		default_random_engine   e;
		e.seed(time(NULL));
		uniform_int_distribution<unsigned>  u(0, 3);
		callbegin = u(e);
		currentplayer = player[u(e)];
		status = AskDiZhuPeriod_doudizhu;
	}

	if (!DiZhu&&4==calltimes) //�ж��Ƿ���Ҫ���·��ƣ����û�˽е����Ļ�
	{
		status = SendCardPeriod_doudizhu;
	}

	if (!currentplayer->ishuman()) //��д�ĺ�������˼�����ж�����ǲ����ˣ�������˼����������˵Ļ��͵����Զ��е����ĺ���
	{
		calltimes++;
		if (ĳĳ����)
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
	else //���˵Ļ�
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