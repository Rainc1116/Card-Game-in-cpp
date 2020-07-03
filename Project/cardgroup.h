#pragma once

//一组牌的可能类型
enum Type{
	Empty = 0,//空
	Unkown,//未知
	Single,//单张
	Double,//对子
	Three,//三条
	SingleSeq,//单顺，五张或五张以上牌点连续的牌，花色不限(不包括2)
	DoubleSeq,//双顺，三对或更多的连续对牌(如：334455、88991010JJ)
	ThreeSeq,//三顺，二个或更多的连续三张牌(如：333444、444555666777)
	ThreePlus,//三带一（一对）
	Airplane,//飞机，三顺+同数量的一对牌。例如：333444+6677
	//FourSeq,//四带二（两张或两对）
	BombFour,//4个一样的，如四个8，威力小于炮，被称为枪
	BombFive,//5个一样的，如五个9，威力小于火箭，被称为炮
	BombSix,//6个一样的，如六个10，威力小于导弹，被称为火箭(民间称六喜)
	BombSeven,//7个一样的，如七个J，威力小于天炸，被称为导弹(民间称七巧)
	BombEight,//8个一样的，如八个Q，威力紧次于天尊，被称为天炸
	BombKing,//四个王，称为天尊，什么牌都可以打，是最大的牌
};

//牌型结构
class CardGroup{
	friend class Player;
	friend class Game;
	friend class Scene;
public:
	CardGroup();
	CardGroup(enum Type t, int v);
	CardGroup(enum Type t);
	CardGroup& operator=(CardGroup &cg);
	void AddNumber(int num);//添加0-107表示的牌元素
	void DeleteNumber(int num);//去掉一张牌
	void Clear(void);//重置此结构
	//把0-107转换成3-17权值，其中A（14）、2（15）、小王（16）、大王（17）
	int Translate(int num);
	void Analyse();
	
private:
	std::map<int, int> group;//3-17权值集合
	std::set<int> cards;//0-53组成的集合，主要用于方便画面显示
	enum Type type;//牌型类型（单牌、对子等等）
	int value;//权值
	int count;//此结构元素数量（牌数量）
	int typevalue;//牌型权值
	int typecount;//牌型数目
};
