#include <Windows.h>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include "cardgroup.h"
#include "game.h"
#include "player.h"

using namespace std;

Player::Player(Game &game):game(game)
{
}

void Player::NewGame()
{
	selection.Clear();
	cards.clear();
}

void Player::AddCard(int num)
{
	cards.insert(num);
}

void Player::GetRemain()
{
	return cards.size();
}

void Player::AddSelection(int num)
{
	selection.AddNumber(num);
}

void Player::DeleteSelection(int num)
{
	selection.DeleteNumber(num);
}

bool Player::IsRight(CardGroup lastcards)
{
	selection.Analyse();
	if (selection.type != Unkown)
	{
		if (lastcards.type == Empty)
		{
			if (selection.type != Empty)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if(selection.type == lastcards.type && selection.count == lastcards.count)
		{
			if(selection.value > lastcards.value)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (selection.type == Empty)
		{
			return true;
		}
		else
		{
			if (selection.typevalue > lastcards.typevalue)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
}

CardGroup Player::Discard()
{
	for(auto num:selection.cards){
		cards.erase(num);
	}
	return selection;
}

void Player::Pass()
{
//似乎不用做任何操作
}

//对于AI玩家，首先判断是否过牌，若否，则选牌，之后出牌
void Player::AISelect(CardGroup lastcards)       //人工智障选牌
{
	enum Type i;
	int j;
	switch (lastcards.type)
	{
		case Empty:
			for (i = Single; i <= Three; i++)
			{
				selection = AIFind(i, 0, lastcards.typecount);
				if (selection.type != Empty)
				{
					break;
				}
			}
			if (selection.type != Empty)
			{
				break;
			}
			i = ThreePlus;
			selection = AIFind(i, 0, lastcards.typecount);
			if (selection.type != Empty)
			{
				break;
			}
			i = SingleSeq;
			for (j = 5; j < cards.size(); j++)
			{
				selection = AIFind(i, 0, j);
				if (selection.type != Empty)
				{
					break;
				}
			}
			if (selection.type != Empty)
			{
				break;
			}
			i = DoubleSeq;
			for (j = 3; j < cards.size(); j++)
			{
				selection = AIFind(i, 0, j);
				if (selection.type != Empty)
				{
					break;
				}
			}
			if (selection.type != Empty)
			{
				break;
			}
			i = ThreeSeq;
			for (j = 2; j < cards.size(); j++)
			{
				selection = AIFind(i, 0, j);
				if (selection.type != Empty)
				{
					break;
				}
			}
			if (selection.type != Empty)
			{
				break;
			}
			i = Airplane;
			for (j = 4; j < cards.size(); j++)
			{
				selection = AIFind(i, 0, j);
				if (selection.type != Empty)
				{
					break;
				}
			}
			if (selection.type != Empty)
			{
				break;
			}
			for (i = BombFour; i <= BombKing; i++)
			{
				selection = AIFind(i, 0, lastcards.typecount);
				if (selection.type != Empty)
				{
					break;
				}
			}
			break;
		case Single:
			selection = AIFind(Single, lastcards.value, lastcards.typecount);
			break;
		case Double:
			selection = AIFind(Double, lastcards.value, lastcards.typecount);
			break;
		case Three:
			selection = AIFind(Three, lastcards.value, lastcards.typecount);
			break;
		case SingleSeq:
			selection = AIFind(SingleSeq, lastcards.value, lastcards.typecount);
			if (selection.type == Empty)
			{
				for (i = BombFour; i < BombSix; i++)
				{
					selection = AIFind(i, 0, 0);
					break;
				}
				if (selection.type == Empty && lastcards.count >=15)
				{
					for (i = BombEight; i <= BombKing; i++)
					{
						selection = AIFind(i, 0, 0);
						break;
					}
				}
			}
			break;
		case DoubleSeq:
			selection = AIFind(DoubleSeq, lastcards.value, lastcards.typecount);
			if (selection.type == Empty)
			{
				for (i = BombFour; i < BombSix; i++)
				{
					selection = AIFind(i, 0, 0);
					break;
				}
				if (selection.type == Empty && lastcards.count >=10)
				{
					for (i = BombEight; i <= BombKing; i++)
					{
						selection = AIFind(i, 0, 0);
						break;
					}
				}
			}
			break;
		case ThreeSeq:
			selection = AIFind(ThreeSeq, lastcards.value, lastcards.typecount);
			if (selection.type == Empty)
			{
				for (i = BombFour; i < BombSix; i++)
				{
					selection = AIFind(i, 0, 0);
					break;
				}
				if (selection.type == Empty && lastcards.count >=10)
				{
					for (i = BombEight; i <= BombKing; i++)
					{
						selection = AIFind(i, 0, 0);
						break;
					}
				}
			}
			break;
		case ThreePlus:
			selection = AIFind(ThreePlus, lastcards.value, lastcards.typecount);
			if (selection.type == Empty)
			{
				for (i = BombFour; i < BombSix; i++)
				{
					selection = AIFind(i, 0, 0);
					break;
				}
			}
			break;
		case Airplane:
			selection = AIFind(Airplane, lastcards.value, lastcards.typecount);
			if (selection.type == Empty)
			{
				for (i = BombFour; i < BombEight; i++)
				{
					selection = AIFind(i, 0, 0);
					break;
				}
				if (selection.type == Empty && lastcards.count >=15)
				{
					for (i = BombEight; i <= BombKing; i++)
					{
						selection = AIFind(i, 0, 0);
						break;
					}
				}
			}
			break;
		default:
			selection = AIFind(lastcards.type, lastcards.value, lastcards.typecount);
			for (i = lastcards.type + 1; i <= BombKing; i++)
			{
				selection = AIFind(i, 0, 0);
				break;
			}
			break;
	}
}

bool Player::AIPass(void)        //AI选择不出，过牌
{
	//还没写
}

CardGroup Player::AIFind(enum Type type, int value，int count)
{
	int i, j, k;
	CardGroup temp;
	CardGroup result;
	bool ifget = false;		//标志：是否已经找到合规的牌
	bool ifright = false;    //标志，牌是否合规
	if (type == Empty)
	{
		return result;
	}
	for(auto num:selection.cards){
		temp.AddNumber(num);
	}
	switch (type)
	{
		case Single:
			for (i = value + 1; i < 18; i++)
			{
				if (temp.group[i] == 1)		//有单牌时
				{
					AddByValue(&result, i, 1);
					ifget = true;
					break;
				}
			}
			if (ifget == false)			//没找到，需要拆分
			{
				for (i = value; i < 18; i++)
				{
					if (temp.group[i] > 1)
					{
						AddByValue(&result, i, 1);
						ifget = true;
						break;
					}
				}
			}
			break;
		case Double:
			for (i = value + 1; i < 18; i++)
			{
				if (temp.group[i] == 2)		//不需要拆分时
				{
					AddByValue(&result, i, 2);
					ifget = true;
					break;
				}
			}
			if (ifget == false)			//没找到，需要拆牌
			{
				for (i = value; i < 18; i++)
				{
					if (temp.group[i] > 2)
					{
						AddByValue(&result, i, 2);
						ifget = true;
						break;
					}
				}
			}
			break;
		case Three:
			for (i = value + 1; i < 16; i++)
			{
				if (temp.group[i] == 3)		//不需要拆分时
				{
					AddByValue(&result, i, 3);
					ifget = true;
					break;
				}
			}
			if (ifget == false)			//没找到，需要拆牌
			{
				for (i = value; i < 16; i++)
				{
					if (temp.group[i] > 3)
					{
						AddByValue(&result, i, 3);
						ifget = true;
						break;
					}
				}
			}
			break;
		case SingleSeq:
			for (i = value + 1; i < 16 - count; i++)
			{
				for (j = 0; j < count; j++)
				{
					if (temp.group[i + j] == 0)
					{
						break;
					}
				}
				if (j == count)
				{
					for (j = 0; j < count; j++)
					{
						AddByValue(&result, i + j, 1);
					}
					break;
				}
			}
			break;
		case DoubleSeq:
			for (i = value + 1; i < 16 - count; i++)
			{
				for (j = 0; j < count; j++)
				{
					if (temp.group[i + j] < 2)
					{
						break;
					}
				}
				if (j == count)
				{
					for (j = 0; j < count; j++)
					{
						AddByValue(&result, i + j, 2);
					}
					break;
				}
			}
			break;
		case ThreeSeq:
			for (i = value + 1; i < 16 - count; i++)
			{
				for (j = 0; j < count; j++)
				{
					if (temp.group[i + j] < 3)
					{
						break;
					}
				}
				if (j == count)
				{
					for (j = 0; j < count; j++)
					{
						AddByValue(&result, i + j, 3);
					}
					break;
				}
			}
			break;
		case ThreePlus:
			//找“三”
			for (i = value + 1; i < 16; i++)
			{
				if (temp.group[i] == 3)		//不需要拆分时
				{
					AddByValue(&result, i, 3);
					ifget = true;
					break;
				}
			}
			if (ifget == false)			//没找到，需要拆牌
			{
				for (i = value; i < 16; i++)
				{
					if (temp.group[i] > 3)
					{
						AddByValue(&result, i, 3);
						ifget = true;
						break;
					}
				}
			}
			if (ifget == false)
			{
				result.Clear();
				break;
			}
			//找“二”
			ifget == false;
			for (j = value + 1; j < 16; j++)
			{
				if (j == i)
				{
					continue;
				}
				if (temp.group[j] == 2)		//不需要拆分时
				{
					AddByValue(&result, j, 2);
					ifget = true;
					break;
				}
			}
			if (ifget == false)			//没找到，需要拆牌
			{
				for (j = value; j < 16; j++)
				{
					if (j == i)
					{
						continue;
					}
					if (temp.group[j] > 2)
					{
						AddByValue(&result, j, 2);
						ifget = true;
						break;
					}
				}
			}
			if (ifget == false)
			{
				result.Clear();
				break;
			}
			break;
		case Airplane:
			//找连续的三
			for (i = value + 1; i < 16 - count / 2; i++)
			{
				for (j = 0; j < count / 2; j++)
				{
					if (temp.group[i + j] < 3)
					{
						break;
					}
				}
				if (j == count / 2)
				{
					for (j = 0; j < count / 2; j++)
					{
						AddByValue(&result, i + j, 3);
					}
					break;
				}
			}
			if (i == 16 - count / 2)
			{
				result.Clear();
				break;
			}
			//找连续的二
			for (j = i; j < 16 - count / 2; j++)
			{
				if (j >= i && j < i + count / 2)
				{
					break;
				}
				for (k = 0; k < count / 2; k++)
				{
					if (temp.group[j + k] < 2)
					{
						break;
					}
				}
				if (k == count)
				{
					for (k = 0; k < count; k++)
					{
						AddByValue(&result, j + k, 2);
					}
					break;
				}
			}
			if (i == 16 - count / 2)
			{
				result.Clear();
				break;
			}
			break;
		case BombFour:
			for (i = value + 1; i < 16; i++)
			{
				if (temp.group[i] == 4)		//不需要拆分时
				{
					AddByValue(&result, i, 4);
					ifget = true;
					break;
				}
			}
			if (ifget == false)			//没找到，需要拆牌
			{
				for (i = value; i < 16; i++)
				{
					if (temp.group[i] > 4)
					{
						AddByValue(&result, i, 4);
						ifget = true;
						break;
					}
				}
			}
			break;
		case BombFive:
		for (i = value + 1; i < 16; i++)
			{
				if (temp.group[i] == 5)		//不需要拆分时
				{
					AddByValue(&result, i, 5);
					ifget = true;
					break;
				}
			}
			if (ifget == false)			//没找到，需要拆牌
			{
				for (i = value; i < 16; i++)
				{
					if (temp.group[i] > 5)
					{
						AddByValue(&result, i, 5);
						ifget = true;
						break;
					}
				}
			}
			break;
		case BombSix:
		for (i = value + 1; i < 16; i++)
			{
				if (temp.group[i] == 6)		//不需要拆分时
				{
					AddByValue(&result, i, 6);
					ifget = true;
					break;
				}
			}
			if (ifget == false)			//没找到，需要拆牌
			{
				for (i = value; i < 16; i++)
				{
					if (temp.group[i] > 6)
					{
						AddByValue(&result, i, 6);
						ifget = true;
						break;
					}
				}
			}
			break;
		case BombSeven:
		for (i = value + 1; i < 16; i++)
			{
				if (temp.group[i] == 7)		//不需要拆分时
				{
					AddByValue(&result, i, 7);
					ifget = true;
					break;
				}
			}
			if (ifget == false)			//没找到，需要拆牌
			{
				for (i = value; i < 16; i++)
				{
					if (temp.group[i] > 7)
					{
						AddByValue(&result, i, 7);
						ifget = true;
						break;
					}
				}
			}
			break;
		case BombEight:
		for (i = value + 1; i < 16; i++)
			{
				if (temp.group[i] == 8)		//不需要拆分时
				{
					AddByValue(&result, i, 8);
					ifget = true;
					break;
				}
			}
			if (ifget == false)			//没找到，需要拆牌
			{
				for (i = value; i < 16; i++)
				{
					if (temp.group[i] > 8)
					{
						AddByValue(&result, i, 8);
						ifget = true;
						break;
					}
				}
			}
			break;
		case BombKing:
			if (temp.group[16] == 2 && temp.group[17] == 2)		//不需要拆分时
			{
				AddByValue(&result, 16, 2);
				AddByValue(&result, 16, 2);
				ifget = true;
				break;
			}
			break;
		default:
			break;
	}



	/*if (ifget == true)
	{
		result.Analyse();
	}*/
	result.Analyse();
	return result;
	//分析过类型后再返回
}


void Player::AddByValue(CardGroup *cardgroup, int value, int count)
{
	int i, j;
	int num = 0;    //加入的数量
	if (value < 16)
	{
		for (j = (value - 3) * 4; j < (value - 3) * 4 + 4; j++)		//前四个
		{
			if (cards.find(j) != cards.end())
			{
				(*cardgroup).AddNumber(j);
				num++;
				if (num == count)
				{
					return;
				}
			}
		}
		for (j = (value - 3) * 4 + 54; j < (value - 3) * 4 + 58; j++)	//后四个
		{
			if (cards.find(j) != cards.end())
			{
				(*cardgroup).AddNumber(j);
				num++;
				if (num == count)
				{
					return;
				}
			}
		}
	}
	else
	{
		j = value + 36;
		if (cards.find(j) != cards.end())
		{
			(*cardgroup).AddNumber(j);
			num++;
			if (num == count)
			{
				return;
			}
		}
		j = value + 54 + 36
		if (cards.find(j) != cards.end())
		{
			(*cardgroup).AddNumber(j);
			num++;
			if (num == count)
			{
				return;
			}
		}
	}
	return;
}

bool Player::AICall()
{
	int sum;
	CardGroup temp;
	for (auto num:selection.cards){
		temp.AddNumber(num);
	}
	for (int i = 0; i < 18; i++)
	{
		sum = sum + temp.group[i];
	}
	if (sum > CALLLORDVALUE)
	{
		return true;
	}
	else
	{
		return false;
	}
}
