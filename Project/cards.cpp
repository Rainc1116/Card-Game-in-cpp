/**************************************************************\
模块：
	类Cards -> 斗地主.exe
文件：
	cards.cpp
功能：
	游戏进行需要的扑克类，用来表示一副扑克。包含洗牌及发牌函数。
作者：
	宋保明
修改历史：
	修改人	修改时间	修改内容
	-------	-----------	-------------------------------
	宋保明	2014.12.5	创建
\**************************************************************/


#include <iostream>
#include <time.h>
#include "cards.h"

using namespace std;

Cards::Cards()
{
	for (int i = 0; i < CARDNUM; ++i)
		cards[i] = i;//初始化为0-53
	//RandCards();//洗牌一次并重置牌堆剩余牌数，貌似initgame里会调用的
}

//洗牌
void Cards::RandCards(void)
{


	//这里洗牌可以采取更简便的方式
	srand(time(0));
	for (int i = CARDNUM; i > 0; i--)
		swap(cards[i - 1], cards[rand() % i]);
	//每一局开始前更新remain
	remain = CARDNUM;
}

//抹牌
int Cards::GetCard(void)
{
	//下标判断越界，上界也判断一下
	if (!remain || remain > CARDNUM)
		throw runtime_error("No more card in cardset!");

	return cards[--remain];
}