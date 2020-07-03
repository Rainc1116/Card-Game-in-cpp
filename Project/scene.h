#pragma once
#include<Windows.h>
//This Part controls the display of graphics
class Game;

class Scene {
public:
	Scene();
	//~Scene();

	SIZE GetSize(void);
	void InitScene(HWND hwnd);
	void ShowScene(HDC hdc);
	void ShowScene(HWND hwnd);

	void DrawBackground(void);
	void DrawPlayerCards1(HDC hdc, int highlight = -1);  //画玩家1、3手牌，即上下两个
	void DrawPlayerCards2(HDC hdc, int highlight = -1);  //画玩家2、4手牌，即左右两个
	void DrawResult(void);
	void ShowQuestionBtn(void); //显示询问是否叫地主
	void HideQuestionBtn(void);//隐藏询问是否叫地主
	void DrawChars(void);
	void SelectCard(POINT point); //选牌后的显示
	void DeleteCard(POINT point); //将选的牌再放下
	int PointInWhich(POINT point);

private:
	Game* game;
	HDC hdcScene;
	HDC hdcBkg;
	HDC hdcPlayerCards1;//玩家2、4的手牌
	HDC hdcPlayerCards2;//玩家1、3的手牌
	HDC hdcCards;//牌的正面
	HDC hdcCardBack;//牌背面
	HDC hdcNoDiscard;//“不出”字样
	HDC hdcDizhu;
	HBITMAP hbmScene;
	HBITMAP hbmBkg;
	HBITMAP hbmPlayer1Cards;
	HBITMAP hbmPlayer2Cards;
	HBITMAP hbmCards;
	HBITMAP hbmCardBack;
	HBITMAP hbmNoDiscard;
	HBITMAP hbmDizhu;
	HBRUSH hbrush;
	SIZE windowSize;
	SIZE cardSize;

public:
	HWND modle, no, yes, none, discard, pass, hint;
};
