#pragma once

#define CALLLORDVALUE 247			//叫地主时参考的常量，暂时取25张牌权值的数学期望总和

class Player{
	friend class Game;
	friend class Scene;
	friend class CardGroup;
public:
	Player(Game &game);
	void NewGame();         //开始一局
    //bool CallLord();         //玩家叫地主
    void AddCard(int num);    //发牌（抓一张牌，放入cards数组中）
	int GetRemain();        //获取玩家当前剩余牌数
    void AddSelection(int num);    //（根据玩家操作）增加一张选牌
    void DeleteSelection(int num); //（根据玩家操作）取消一张选牌
	bool IsRight(CardGroup lastcards);         //判断selection里的牌是否符合出牌规则
	CardGroup Discard();       //将合规的selection牌组打出，并且更新现有牌
    void Pass();        //玩家选择不出，过牌

//对于AI玩家，首先判断是否过牌，若否，则选牌，之后出牌
	void AISelect(CardGroup lastcards);       //AI选牌,包含一些简单的判读分析，比如直接出牌还是跟上家的牌
	bool AIPass(void);        //AI选择不出，过牌
	bool AICall();				//AI叫地主
	CardGroup AIFind(enum Type type, int value, int count);    //在手牌里寻找某类型的牌(count:牌数)
	void AddByValue(CardGroup *cardgroup, int value, int count);  //通过给定的权值，在cards中选出count张牌放到cardgroup里

private:
	int score;
	Game &game;             //游戏对象
	std::set<int> cards;          //手中所持牌的集合
	CardGroup selection;    //玩家选择，待出的牌的集合
};
