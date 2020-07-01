#include <Windows.h>
#include <algorithm>
//#include <vector>
//#include <map>
#include <set>
//#include "cardgroup.h"
//#include "game.h"
#include "player.h"

using namespace std;

Player::Player(Game &game):game(game),
{
}

void Player::NewGame()
{
	selection.Clear();
	cards.clear();
}

void Player::GetCards(int num)
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

bool Player::IsRight(Type type)
{
	if(selection.Analyse() == type)
		return true;
	else
		return false;
}

CardGroup Player::Give()
{
	for(auto num:selection.cards)
		cards.erase(num);
	return selection;
}

void Player::Pass()
{
//似乎不用做任何操作
}

//对于AI玩家，首先判断是否过牌，若否，则选牌，之后出牌
void Player::AISelect()       //AI选牌,包含一些简单的判读分析，比如直接出牌还是跟上家的牌
{
	//还没写
}

bool AIPass(void)        //AI选择不出，过牌
{
	//还没写
}



