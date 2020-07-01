#pragma once

#include "card.h"
#include<set>

enum status_doudizhuANDgandengyan //С��5���Ƕ����������ڵ���5���Ǹɵ���
{ //����������Ϸ����
	SendCardPeriod_doudizhu; //���� 0
    AskDiZhuPeriod_doudizhu; //�е��� 1
    SendDiZhuCardPeriod_doudizhu; //�������� 2
    DiscardPeriod_doudizhu; //���� 3
    EndingPeriod_doudizhu //���� 4
//�ɵ��۵���Ϸ����
    SendCardPeriod_gandengyan; //���� 5
    DiscardPeriod_gandengyan; //����&���� 6
    EndingPeriod_gandengyan; //���� 7
};

class Game {
    friend class Player;
    friend class Scene;
public: 
    Game(HWND hwnd, int gamemode��int robotnum);//gamemodΪ1�Ƕ�������2�Ǹɵ���
    ~Game();
    status_doudizhuANDgandengyan Get_status() { return status; }
    void Gamestart(); //��ʼ�׶�
    void Initialization(); //��ʼ��
    void RegisterScene(Scene* scene) { this->scene = scene; }//ע�ỷ��

    void Getscore(); //���ļ��л�ȡ������ҷ���
    void Putscore(); //�����µķ����Ż�ȥ

    void SendCard(); //��ÿһ���˷����ƣ�����ƻ��㹻�Ļ�
    void SendCard_doudizhu(); //�������ķ���
    void SendCard_gandengyan(); //�ɵ��۵ķ���

    void discard(); //���� �Ҿ���������Ϸ���ƽ׶ξ��жϲ�ͬ���Ƿ�Ҫ���Ƶ�����Ͳ��ֿ���
    void pass();//������
    void givinghint();//��������ʾ

    void Gameover(); //��Ϸ����

    void SendDiZhuCard(); //��������
    void AskforDiZhu(); //�е���
    void GetideasofDizhu(bool idea); //�Ƿ�е���

    int GetnextPlayernum(); //��ȡ��һ�����Ƶ��˵ı��
    int GetperviousPlayernum(); //��ȡ��һ�����Ƶ��˵ı��

private:
    status_doudizhuANDgandengyan status;
    Player player[4]; //�������
    int times=1; //���ֵı���
    Player* currentplayer,* DiZhu, *lastplayer;//����ָ��,��ǰ�������ָ�룬��ĳ��������������ѹ����ָ��
    int DiZhuCard[8]; //8�ŵ�����
    Cards Paidui; //�������������ƣ��ɵ�����һ����
    HWND hMainWnd;
    Scene* scene;
    int callscore[4] = { -1�� - 1�� - 1�� - 1 }; //�Ƿ�е��� 0Ϊ�е�����1Ϊ���У�-1Ϊ��û�н�
    int calltimes=0; //�м����˽����˽е����Ĳ���
    int callbegin; //�Ӽ�����ҿ�ʼ�е���
};