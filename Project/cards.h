#pragma once

#define CARDNUM 108

//发牌堆类
class Cards {
public:
	Cards();
	void RandCards(void);//开局前洗牌，更新remain
	int GetCard();//发出一张牌，更新remain
	int GetRemain(void)const { return remain; };//获取剩余牌数

private:
	int cards[CARDNUM];//牌数组
	int remain = CARDNUM;//剩余牌数
};