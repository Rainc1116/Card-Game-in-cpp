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
	if (status<5) //�жϵ�������һ����Ϸ
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

	if (lastplayer == currentplayer && status >= 5) //�ɵ���������ѹ�������һ����
	{
		sendcard();
	}

	if (currentplayer->ishuman()) {//��ǰ���Ϊ��
		if (currentplayer->selection.count && currentplayer->HumanDiscard()) {//�����ѡ�Ʋ��ҷ��Ϲ涨
			scene->HideDiscardBtn();
			lastplayer = currentplayer;
			//�������ӻ���
		}
		else {//��������ȴ����ѡ��
			scene->ShowScene(hMainWnd);
			scene->ShowDiscardBtn();
			return;
		}
	}
	else {//��ǰ���Ʒ�Ϊ����
		currentplayer->SelectCards();
		if (currentplayer->Discard())
			lastplayer = currentplayer;
		//�������ӻ���
	}
	scene->ShowScene(hMainWnd);

	if (currentplayer->cards.empty())//�����Ʒ���������
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
			MessageBox(hMainWnd, TEXT("��ϲ��������ʤ��"), TEXT("��Ϸ����"), 0);
		else
			MessageBox(hMainWnd, TEXT("���ź���ũ���ʤ��"), TEXT("��Ϸ����"), 0);
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
		case 0:MessageBox(hMainWnd, TEXT("��ϲ,���0��ʤ��"), TEXT("��Ϸ����"), 0); break;
		case 1:MessageBox(hMainWnd, TEXT("��ϲ,���1��ʤ��"), TEXT("��Ϸ����"), 0); break;
		case 2:MessageBox(hMainWnd, TEXT("��ϲ,���2��ʤ��"), TEXT("��Ϸ����"), 0); break;
		case 3:MessageBox(hMainWnd, TEXT("��ϲ,���3��ʤ��"), TEXT("��Ϸ����"), 0); break;
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
	//�����ǳ���
	scene->DrawBackground();//�����˵�������������������ʾ
	scene->ShowScene(hMainWnd);
	status = DiscardPeriod_doudizhu;//��ǰ״̬Ϊ���ƽ׶�
	currentplayer = DiZhu;
	lastplayer = DiZhu;
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