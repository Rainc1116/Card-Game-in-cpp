#pragma once

#define CARDNUM 108

//���ƶ���
class Cards {
public:
	Cards();
	void RandCards(void);//����ǰϴ�ƣ�����remain
	int GetCard();//����һ���ƣ�����remain
	int GetRemain(void)const { return remain; };//��ȡʣ������

private:
	int cards[CARDNUM];//������
	int remain = CARDNUM;//ʣ������
};