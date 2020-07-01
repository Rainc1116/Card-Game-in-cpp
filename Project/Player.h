class Player{
	friend class Game;
	friend class Scene;
public:
	Player(Game &game);
	void NewGame();         //开始一局
    //bool CallLord();         //玩家叫地主
    void GetCards(int num);    //发牌（抓一张牌，放入cards数组中）
	int GetRemain();        //获取玩家当前剩余牌数
    void AddSelection(int num);    //（根据玩家操作）增加一张选牌
    void DeleteSelection(int num); //（根据玩家操作）取消一张选牌
	bool IsRight(Type type);         //判断selection里的牌是否符合出牌规则
	CardGroup Give();       //将合规的selection牌组打出，并且更新现有牌
    void Pass();        //玩家选择不出，过牌

//对于AI玩家，首先判断是否过牌，若否，则选牌，之后出牌
	void AISelect();       //AI选牌,包含一些简单的判读分析，比如直接出牌还是跟上家的牌
	bool AIPass(void);        //AI选择不出，过牌

private:
	Game &game;             //游戏对象
	std::set<int> cards;          //手中所持牌的集合
	CardGroup selection;    //玩家选择，待出的牌的集合
};
