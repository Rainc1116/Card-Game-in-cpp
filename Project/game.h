#pragma once

#include "card.h"
#include<set>

enum status_doudizhuANDgandengyan //小于5就是斗地主，大于等于5就是干瞪眼
{ //斗地主的游戏进程
	SendCardPeriod_doudizhu; //发牌 0
    AskDiZhuPeriod_doudizhu; //叫地主 1
    SendDiZhuCardPeriod_doudizhu; //发地主牌 2
    DiscardPeriod_doudizhu; //出牌 3
    EndingPeriod_doudizhu //结束 4
//干瞪眼的游戏进程
    SendCardPeriod_gandengyan; //发牌 5
    DiscardPeriod_gandengyan; //出牌&抽牌 6
    EndingPeriod_gandengyan; //结束 7
};

class Game {
    friend class Player;
    friend class Scene;
public: 
    Game(HWND hwnd, int gamemode，int robotnum);//gamemod为1是斗地主，2是干瞪眼
    ~Game();
    status_doudizhuANDgandengyan Get_status() { return status; }
    void Gamestart(); //开始阶段
    void Initialization(); //初始化
    void RegisterScene(Scene* scene) { this->scene = scene; }//注册环境

    void Getscore(); //从文件中获取四名玩家分数
    void Putscore(); //将更新的分数放回去

    void SendCard(); //给每一个人发张牌，如果牌还足够的话
    void SendCard_doudizhu(); //斗地主的发牌
    void SendCard_gandengyan(); //干瞪眼的发牌

    void discard(); //出牌 我觉得两个游戏出牌阶段就判断不同和是否要摸牌的区别就不分开了
    void pass();//不出牌
    void givinghint();//给弱智提示

    void Gameover(); //游戏结束

    void SendDiZhuCard(); //发地主牌
    void AskforDiZhu(); //叫地主
    void GetideasofDizhu(bool idea); //是否叫地主

    int GetnextPlayernum(); //获取上一个出牌的人的编号
    int GetperviousPlayernum(); //获取下一个出牌的人的编号

private:
    status_doudizhuANDgandengyan status;
    Player player[4]; //四名玩家
    int times=1; //本局的倍率
    Player* currentplayer,* DiZhu, *lastplayer;//地主指针,当前出牌玩家指针，到某个玩家算该轮无人压死的指针
    int DiZhuCard[8]; //8张地主牌
    Cards Paidui; //斗地主是两副牌，干瞪眼是一副牌
    HWND hMainWnd;
    Scene* scene;
    int callscore[4] = { -1， - 1， - 1， - 1 }; //是否叫地主 0为叫地主，1为不叫，-1为还没有叫
    int calltimes=0; //有几个人进行了叫地主的操作
    int callbegin; //从几号玩家开始叫地主
};