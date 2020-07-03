/**************************************************************\
模块：
	类CardGroup -> 斗地主.exe
文件：
	cardgroup.cpp
功能：
	存储符合基本牌的一组牌，包含这组牌的类型、牌面、数量等。
作者：
	宋保明
修改历史：
	修改人	修改时间	修改内容
	-------	-----------	-------------------------------
	宋保明	2014.12.5	创建
\**************************************************************/

#include <map>
#include <set>
#include "cardgroup.h"


CardGroup::CardGroup()
:type(Empty))
,value(0)
,typevalue(0)
，typecount(0)
,count(0)
{

}

CardGroup::CardGroup(Type t,int v)
: type(t)
, value(v)
,typevalue(0)
，typecount(0)
,count(0)
{

}//目测不会用到

CardGroup::CardGroup(Type t)
: type(t)
,value(0)
,typevalue(0)
，typecount(0)
,count(0)
{

}

CardGroup &CardGroup::operator=(CardGroup &cg)
{
	this->group = cg.group;
	this->cards = cg.cards;
	this->type = cg.type;
	this->value = cg.value;
	this->count = cg.count;
	this->typevalue = cg.typevalue;
	this->typecount = cg.typecount;
	return *this;
}
//重置牌型
void CardGroup::Clear(void)
{
	group.clear();
	cards.clear();
	type = Unkown;
	value = 0;
	count = 0;
	typevalue = 0;
	typecount = 0;
	return;
}
//添加0-53表示的牌
void CardGroup::AddNumber(int num)
{
	++count;
	cards.insert(num);
	++group[Translate(num)];
	if (group[Translate(num)] == 1)
	{
		typecount++;
	}
	
}
//去掉一张牌
void CardGroup::DeleteNumber(int num)
{
	if (cards.find(num) == cards.end())//确定要去掉的牌在结构内
		return;
	--count;
	cards.erase(num);
	if (--group[Translate(num)] == 0)
	{
		group.erase(Translate(num));
		typecount--;
	}
}

void CardGroup::Analyse()
{
	int i, j, k;
	if (count == 0)
	{
		type = Empty;
		return;
	}
	else if (count == 1)
	{
		type = Single;
		typevalue = 1;
		value = Translate(*(cards.begin()));
		return;
	}
	else if (count == 2)
	{
		if (group[Translate(*(cards.begin()))] == 2)
		{
			type = Double;
			typevalue = 1;
			value = Translate(*(cards.begin()));
			return;
		}
		else
		{
			type = Unkown;
			typevalue = 0;
			value = 0;
			return;
		}
	}
	else if (count == 3)
	{
		if (group[Translate(*(cards.begin()))] == 3)
		{
			type = Three;
			typevalue = 1;
			value = Translate(*(cards.begin()));
			return;
		}
		else
		{
			type = Unkown;
			typevalue = 0;
			value = 0;
			return;
		}
	}
	else if (count == 4)
	{
		if (group[Translate(*(cards.begin()))] == 4)
		{
			type = BombFour;
			typevalue = 2;
			value = Translate(*(cards.begin()));
			return;
		}
		else if (group[16] == 2 && group[17] == 2)
		{
			type = BombKing;
			typevalue = 7;
			value = 17;
			return;
		}
		else
		{
			type = Unkown;
			typevalue = 0;
			value = 0;
			return;
		}
	}
	else
	{
		if (typecount == 1)
		{
			switch (count)
			{
			case 5:
				type = BombFive;
				typevalue = 3;
				value = Translate(*(cards.begin()));
				return;
				break;
			case 6:
				type = BombSix;
				typevalue = 4;
				value = Translate(*(cards.begin()));
				return;
				break;
			case 7:
				type = BombSeven;
				typevalue = 5;
				value = Translate(*(cards.begin()));
				return;
				break;
			case 8:
				type = BombEight;
				typevalue = 6;
				value = Translate(*(cards.begin()));
				return;
				break;
			}
		}
		else if (typecount == count)//可能是单顺
		{
			for (i = 3; i < 16 - typecount; i++)
			{
				if (group[i] == 1)
				{
					for (j = 0; j < typecount; j++)
					{
						if (group[i + j] == 0)
						{
							type = Unkown;
							typevalue = 0;
							value = 0;
							return;
						}
					}
					type = SingleSeq;
					typevalue = 1;
					value = i;
					return;
				}
				
			}
			
		}
		else if (count == 5 && typecount == 2) //三带一对
		{
			for (i = 3; i < 16; i++)
			{
				if (group[i] == 3)
				{
					type = ThreePlus;
					typevalue = 1;
					value = i;
					return;
				}
			}
			type = Unkown;
			typevalue = 0;
			value = 0;
			return;
		}
		else if (count == 2 * typecount)//双顺
		{
			for (i = 3; i < 16 - typecount; i++)
			{
				if (group[i] == 2)
				{
					for (j = 0; j < typecount; j++)
					{
						if (group[i + j] != 2)
						{
							type = Unkown;
							typevalue = 0;
							value = 0;
							return;
						}
					}
					type = DoubleSeq;
					typevalue = 1;
					value = i;
					return;
				}
			}
			type = Unkown;
			typevalue = 0;
			value = 0;
			return;
		}
		else if (count == 3 * typecount)//三顺
		{
			for (i = 3; i < 16 - typecount; i++)
			{
				if (group[i] == 3)
				{
					for (j = 0; j < typecount; j++)
					{
						if (group[i + j] != 3)
						{
							type = Unkown;
							typevalue = 0;
							value = 0;
							return;
						}
					}
					type = ThreeSeq;
					typevalue = 1;
					value = i;
					return;
				}
			}
			type = Unkown;
			typevalue = 0;
			value = 0;
			return;
		}
		else if (count >= 10 && count % 5 == 0 && count/5*2 == typecount)//飞机带翅膀
		{
			for (i = 3; i < 16 - typecount/2; i++)
			{
				if (group[i] == 3)
				{
					for (j = 0; j < typecount/2; j++)
					{
						if (group[i + j] != 3)
						{
							type = Unkown;
							typevalue = 0;
							value = 0;
							return;
						}
					}
					for (j = 3; j < 16 - typecount/2; j++)
					{
						if (group[j] == 2)
						{
							for (k = 0; k < typecount/2; k++)
							{
								if (group[j + k] != 2)
								{
									type = Unkown;
									typevalue = 0;
									value = 0;
									return;
								}
							}
							type = Airplane;
							typevalue = 1;
							value = i;
							return;
						}
					}
				}
			}
			type = Unkown;
			typevalue = 0;
			value = 0;
			return;
		}
		else
		{
			type = Unkown;
			typevalue = 0;
			value = 0;
			return;
		}
		
	}	
}

//把0-53转换成3-17权值，其中A（14）、2（15）、小王（16）、大王（17）
int CardGroup::Translate(int num)
{
	if (num > 53)
		num = num-54;
	
	if (num < 52)
		return num / 4 + 3;
	else
		return num - 36;
}
