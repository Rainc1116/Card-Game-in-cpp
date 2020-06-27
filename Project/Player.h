class Player{
	friend class Game;
	friend class Scene;
public:
	Player(Game &game);
	void NewGame();         //开始一局
    int CallLord();         //玩家叫地主，返回分数
    void GetCards();    //发牌（抓一张牌，放入cards数组中）
	int GetRemain();        //获取玩家当前剩余牌数
    void AddSelection();    //（根据玩家操作）增加一张选牌
    void DeleteSelection(); //（根据玩家操作）取消一张选牌
	bool IsRight();         //判断selection里的牌是否符合出牌规则
	CardGroup Give();       //将合规的selection牌组打出，并且更新现有牌
    void Pass(void);        //玩家选择不出，过牌

	void AISelect();       //AI选牌,包含一些简单的判读分析，比如直接出牌还是跟上家的牌
	void AIPass(void);        //AI选择不出，过牌

private:
	Game &game;             //游戏对象
	int score;              //玩家叫地主分数
	int cards[35];          //手中所持牌的集合
	CardGroup selection;    //玩家选择，待出的牌的集合
};