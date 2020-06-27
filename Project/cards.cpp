/**************************************************************\
ģ�飺
	��Cards -> ������.exe
�ļ���
	cards.cpp
���ܣ�
	��Ϸ������Ҫ���˿��࣬������ʾһ���˿ˡ�����ϴ�Ƽ����ƺ�����
���ߣ�
	�α���
�޸���ʷ��
	�޸���	�޸�ʱ��	�޸�����
	-------	-----------	-------------------------------
	�α���	2014.12.5	����
\**************************************************************/


#include <iostream>
#include <time.h>
#include "cards.h"

using namespace std;

Cards::Cards()
{
	for (int i = 0; i < CARDNUM; ++i)
		cards[i] = i;//��ʼ��Ϊ0-53
	//RandCards();//ϴ��һ�β������ƶ�ʣ��������ò��initgame�����õ�
}

//ϴ��
void Cards::RandCards(void)
{


	//����ϴ�ƿ��Բ�ȡ�����ķ�ʽ
	srand(time(0));
	for (int i = CARDNUM; i > 0; i--)
		swap(cards[i - 1], cards[rand() % i]);
	//ÿһ�ֿ�ʼǰ����remain
	remain = CARDNUM;
}

//Ĩ��
int Cards::GetCard(void)
{
	//�±��ж�Խ�磬�Ͻ�Ҳ�ж�һ��
	if (!remain || remain > CARDNUM)
		throw runtime_error("No more card in cardset!");

	return cards[--remain];
}