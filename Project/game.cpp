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

Game::Game(HWND hwnd, int gamemode��int robotnum)
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
	if (status<5) //�жϵ�������һ����Ϸ
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
		status = 0; //��������ʼ
	}
	else
	{
		status = 5; //�ɵ��ۿ�ʼ
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
	if (lastplayer == currentplayer && status >= 5) //�ɵ���������ѹ�������һ����
	{
		sendcard();
	}
	if (GetcurrentPlayernum() < (4 - robotnumber)) {//��ǰ���Ϊ��
		if (currentplayer->selection.count && currentplayer->IsRight()) {//�����ѡ�Ʋ��ҷ��Ϲ涨
			scene->HideDiscardBtn();
			lastplayer = currentplayer;
			//�������ӻ���
			{
				if (curplayer->discard.type == BombFour)//ը��4
					times *= 2;
				else if (curplayer->discard.type == BombFive)//ը��5
					times *= 6;
				else if (curplayer->discard.type == BombSix)//ը��6
					times *= 24;
				else if (curplayer->discard.type == BombSeven)//ը��7
					times *= 120;
				else if (curplayer->discard.type == BombEight)//ը��8
					times *= 720;
				else if (curplayer->discard.type == BombFour)//ը��king
					times *= 100;
			}
		}
		else {//��������ȴ����ѡ��
			scene->ShowScene(hMainWnd);
			scene->ShowDiscardBtn();
			return;
		}
	}
	else {//��ǰ���Ʒ�Ϊ����
		if (!currentplayer->AIPass())
			lastplayer = currentplayer;
		currentplayer->AISelect(); /////////��֪��������ʲô
		//�������ӻ���
		{
			if (curplayer->discard.type == BombFour)//ը��4
				times *= 2;
			else if (curplayer->discard.type == BombFive)//ը��5
				times *= 6;
			else if (curplayer->discard.type == BombSix)//ը��6
				times *= 24;
			else if (curplayer->discard.type == BombSeven)//ը��7
				times *= 120;
			else if (curplayer->discard.type == BombEight)//ը��8
				times *= 720;
			else if (curplayer->discard.type == BombFour)//ը��king
				times *= 100;
		}
	}
	scene->ShowScene(hMainWnd);
	if (currentplayer->cards.empty())//��ǰ���Ʒ���������
	{
        if (status<5)
	        status = EndingPeriod_doudizhu;//doudizhu��Ϸ����
	    if(status>=5)
	        status = EndingPeriod_gandengyan;//gandengyan��Ϸ����
    }
	else
		currentplayer = player[GetnextPlayernum()];//�¼Ҽ�������
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
	currentplayer->AISelect(); ////////��֪��������ʲô
	if (currentplayer->selection.count != 0)
		PostMessage(scene->discand, WM_MYBUTTON, TRUE, 0);
	InvalidateRect(hMainWnd, NULL, FALSE);
}

void Game::Gameover()
{
	if (status = 4)
	{   int flag = -1; //�ж��ǲ��ǵ�����ʤ
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
		if (flag==1)	MessageBox(hMainWnd, TEXT("��ϲ��������ʤ��"), TEXT("��Ϸ����"), 0);
		else	MessageBox(hMainWnd, TEXT("��ϲ��ũ���ʤ��"), TEXT("��Ϸ����"), 0);
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
		{case 0:MessageBox(hMainWnd, TEXT("��ϲ,���0��ʤ��"), TEXT("��Ϸ����"), 0); break;
		case 1:MessageBox(hMainWnd, TEXT("��ϲ,���1��ʤ��"), TEXT("��Ϸ����"), 0); break;
		case 2:MessageBox(hMainWnd, TEXT("��ϲ,���2��ʤ��"), TEXT("��Ϸ����"), 0); break;
		case 3:MessageBox(hMainWnd, TEXT("��ϲ,���3��ʤ��"), TEXT("��Ϸ����"), 0); break;}			
		GameStart();}
}

void Game::SendDiZhuCard()
{
	for (int i = 0; i < 8; i++)
	{
		DiZhu->AddCard(DiZhuCard[i]);
	}
	scene->DrawBackground();//�����˵�������������������ʾ
	scene->ShowScene(hMainWnd);
	status = DiscardPeriod_doudizhu;//��ǰ״̬Ϊ���ƽ׶�
	currentplayer = DiZhu;
	lastplayer = DiZhu;
	SetTimer(hMainWnd, 1, 500, NULL);
}

void Game::AskforDiZhu()
{
	if (!calltimes) //��ʼ��
	{
		default_random_engine   e;
		e.seed(time(NULL));
		uniform_int_distribution<unsigned>  u(0, 3);
		currentplayer = player[u(e)];
		status = AskDiZhuPeriod_doudizhu;
	}

	if (!DiZhu&&4==calltimes) //�ж��Ƿ���Ҫ���·��ƣ����û�˽е����Ļ�
	{
		status = SendCardPeriod_doudizhu;
		SetTimer(hMainWnd, 1, 500, NULL);
		calltimes = 0;
		return;
	}

	if (GetcurrentPlayernum()>=(4-robotnumber)) //��˼�����ж�����ǲ����ˣ�������˼����������˵Ļ��͵����Զ��е����ĺ���
	{  
		calltimes++;
		if (currentplayer->AICall()) //�ж��Ƿ�е����ĺ���
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