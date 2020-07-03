#include<Windows.h>
#include"WinMain.h"
#include"scene.h"
#include "resource.h"

Scene::Scene()
{
	windowSize.cx = 850;
	windowSize.cy = 540;
	cardSize.cx = 71;
	cardSize.cy = 96;
}

void Scene::InitScene(HWND hwnd) {
	int x = windowSize.cx / 2;
	int y = windowSize.cy - 130 - 40;
	//创建按钮
	modle = CreateWindow(TEXT("mybutton"), TEXT("modle"), WS_CHILD,
		x - 160, y, 68, 35, hwnd, (HMENU)modle, GetModuleHandle(NULL), NULL);
	no = CreateWindow(TEXT("mybutton"), TEXT("no"), WS_CHILD,
		x - 160, y, 68, 35, hwnd, (HMENU)no, GetModuleHandle(NULL), NULL);
	yes = CreateWindow(TEXT("mybutton"), TEXT("yes"), WS_CHILD,
		x - 80, y, 68, 35, hwnd, (HMENU)yes, GetModuleHandle(NULL), NULL);
	none = CreateWindow(TEXT("mybutton"), TEXT("none"), WS_CHILD,
		x, y, 68, 35, hwnd, (HMENU)none, GetModuleHandle(NULL), NULL);
	discard = CreateWindow(TEXT("mybutton"), TEXT("discard"), WS_CHILD,
		windowSize.cx - 340, y, 68, 35, hwnd, (HMENU)discard, GetModuleHandle(NULL), NULL);
	pass = CreateWindow(TEXT("mybutton"), TEXT("pass"), WS_CHILD,
		windowSize.cx - 260, y, 68, 35, hwnd, (HMENU)pass, GetModuleHandle(NULL), NULL);
	hint = CreateWindow(TEXT("mybutton"), TEXT("hint"), WS_CHILD,
		windowSize.cx - 180, y, 68, 35, hwnd, (HMENU)hint, GetModuleHandle(NULL), NULL);
	//创建相关位图内存设备环境
	HDC hdc = GetDC(hwnd);
	HINSTANCE hInst = GetModuleHandle(NULL);

	HBITMAP hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	hbrush = CreatePatternBrush(hbitmap);
	DeleteObject(hbitmap);

	hbmScene = CreateCompatibleBitmap(hdc, windowSize.cx, windowSize.cy);
	hbmBkg = CreateCompatibleBitmap(hdc, windowSize.cx, windowSize.cy);
	hbmPlayer1Cards = CreateCompatibleBitmap(hdc, 650, 128);

	hbmCardBack = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
	hbmCards = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
	hbmNoDiscard = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));
	hbmDizhu = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));

	hdcScene = CreateCompatibleDC(hdc);
	hdcBkg = CreateCompatibleDC(hdc);
	hdcPlayerCards1 = CreateCompatibleDC(hdc);
	hdcPlayerCards2 = CreateCompatibleDC(hdc);
	hdcCardBack = CreateCompatibleDC(hdc);
	hdcCards = CreateCompatibleDC(hdc);
	hdcNoDiscard = CreateCompatibleDC(hdc);
	hdcDizhu = CreateCompatibleDC(hdc);

	SelectObject(hdcScene, hbmScene);
	SelectObject(hdcBkg, hbmBkg);
	SelectObject(hdcBkg, hbrush);
	SelectObject(hdcPlayerCards1, hbmPlayer1Cards);
	SelectObject(hdcPlayerCards1, hbrush);
	SelectObject(hdcPlayerCards2, hbmPlayer2Cards);
	SelectObject(hdcPlayerCards2, hbrush);
	SelectObject(hdcCardBack, hbmCardBack);
	SelectObject(hdcDizhu, hbmDizhu);
	SelectObject(hdcCards, hbmCards);
	SelectObject(hdcNoDiscard, hbmNoDiscard);

	ReleaseDC(hwnd, hdc);
}

void Scene::DrawBackground(void)
{
	PatBlt(hdcBkg, 0, 0, windowSize.cx, windowSize.cy, PATCOPY);
	HPEN hpen = CreatePen(PS_SOLID, 3, RGB(192, 192, 192));
	HPEN oldpen = (HPEN)SelectObject(hdcBkg, hpen);
	Rectangle(hdcBkg, 100, 130, windowSize.cx - 100, windowSize.cy - 130);
	Rectangle(hdcBkg, 280, 140, 280 + cardSize.cx, 140 + cardSize.cy);
	Rectangle(hdcBkg, 360, 140, 360 + cardSize.cx, 140 + cardSize.cy);
	Rectangle(hdcBkg, 440, 140, 440 + cardSize.cx, 140 + cardSize.cy);
	Rectangle(hdcBkg, 520, 140, 520 + cardSize.cx, 140 + cardSize.cy);

	Rectangle(hdcBkg, 280, 256, 280 + cardSize.cx, 256 + cardSize.cy);
	Rectangle(hdcBkg, 360, 256, 360 + cardSize.cx, 256 + cardSize.cy);
	Rectangle(hdcBkg, 440, 256, 440 + cardSize.cx, 256 + cardSize.cy);
	Rectangle(hdcBkg, 520, 256, 520 + cardSize.cx, 256 + cardSize.cy);

	//BitBlt(hdcScene, 0, 0, sceneSize.cx, sceneSize.cy, hdcBkg, 0, 0, SRCCOPY);
	//if(dizhu meiyou chuxian)
	for (int i = 0; i < 4; ++i) {
		TransparentBlt(hdcBkg, 280 + 80 * i, 140, cardSize.cx, cardSize.cy,
			hdcCardBack, 0, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
	}
	for (int i = 0; i < 4; ++i) {
		TransparentBlt(hdcBkg, 280 + 80 * i, 256, cardSize.cx, cardSize.cy,
			hdcCardBack, 0, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
	}
	int DizhuCard[8] = { 12,43,2,3,18,23,12,7};
	//TransparentBlt(hdcBkg, 0, 0, 50, 43, hdcDizhu, 0, 0, 50, 43, RGB(255, 255, 255));//显示地主
	//else
	
	for (int i = 0; i < 4; ++i) {
		TransparentBlt(hdcBkg, 280 + 80 * i, 140, cardSize.cx, cardSize.cy,
			hdcCards, cardSize.cx * DizhuCard[i], 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
	}
	for (int i = 0; i < 4; ++i) {
		TransparentBlt(hdcBkg, 280 + 80 * i, 256, cardSize.cx, cardSize.cy,
			hdcCards, cardSize.cx * DizhuCard[i + 4], 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
	}
	
	SelectObject(hdcBkg, oldpen);
	DeleteObject(hpen);
}

void Scene::ShowScene(HDC hdc)
{

	BitBlt(hdcScene, 0, 0, windowSize.cx, windowSize.cy, hdcBkg, 0, 0, SRCCOPY);
	DrawPlayerCards1(hdcScene);
	DrawPlayerCards2(hdcScene);
	BitBlt(hdc, 0, 0, windowSize.cx, windowSize.cy, hdcScene, 0, 0, SRCCOPY);
}

void Scene::ShowScene(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);
	ShowScene(hdc);
	ReleaseDC(hwnd, hdc);
}

SIZE Scene::GetSize()
{
	return windowSize;
}

void Scene::DrawPlayerCards1(HDC hdc, int highlight)
{
	PatBlt(hdcPlayerCards1, 0, 0, windowSize.cx, windowSize.cy, PATCOPY);

	int i = 0, y1,
		c = 18,
		x = (650 - cardSize.cx - 22 * c + 22) / 2,
		y = 128 - cardSize.cy - 15;

	/*for (auto rb = 0; rb != 18; ++rb) {//显示玩家手牌
		y1 = y;
		TransparentBlt(hdcPlayerCards1, x + 22 * i, y1, cardSize.cx, cardSize.cy, hdcCards,
			cardSize.cx * rb, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
		++i;
	}*/
	/*
	for (auto rb = game->player[0]->cards.rbegin(); rb != game->player[0]->cards.rend(); ++rb){//显示玩家手牌
		if (game->player[0]->selection.cards.find(*rb) !=
			game->player[0]->selection.cards.end())//将已选择的牌上移一段，突出显示
			y1 = y - 15;
		else
			y1 = y;
		TransparentBlt(hdcHumanCards, x + 22 * i, y1, cardSize.cx, cardSize.cy, hdcCards,
			cardSize.cx * *rb, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
			*/
	for (auto rb = 0; rb != 18; ++rb) {//显示玩家1手牌
		y1 = y;
		TransparentBlt(hdcPlayerCards1, x + 22 * i, y1, cardSize.cx, cardSize.cy, hdcCards,
			cardSize.cx * rb, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
		++i;
	}

	/*
	for (auto rb = game->player[2]->cards.rbegin(); rb != game->player[0]->cards.rend(); ++rb){//显示玩家手牌
		if (game->player[0]->selection.cards.find(*rb) !=
			game->player[0]->selection.cards.end())//将已选择的牌上移一段，突出显示
			y1 = y - 15;
		else
			y1 = y;
		TransparentBlt(hdcHumanCards, x + 22 * i, 10, cardSize.cx, cardSize.cy, hdcCards,
			cardSize.cx * *rb, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
			*/

	i = 0;
	for (auto rb = 0; rb != 18; ++rb) {//显示玩家3手牌
		y1 = y;
		TransparentBlt(hdcScene, x + 22 * i, 10, cardSize.cx, cardSize.cy, hdcCardBack,
			0, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
		++i;
	} // 还有bug

	BitBlt(hdc, 100, 412, 650, 128, hdcPlayerCards1, 0, 0, SRCCOPY);
}

void Scene::DrawPlayerCards2(HDC hdc, int highlight)
{
	int c, i = 0;

	/*if (currentPlayer){//游戏结束，所有手牌正面显示

		for (auto rb = game->player[1]->cards.rbegin(); rb != game->player[1]->cards.rend();++rb){
			TransparentBlt(hdcScene, sceneSize.cx - 50 - cardSize.cx / 2, 65 + 16 * i,
				cardSize.cx, cardSize.cy, hdcCards, cardSize.cx * *rb, 0,
				cardSize.cx, cardSize.cy, RGB(0, 0, 255));
			++i;
		}
	}*/
	//else

	//否则，手牌反面显示，并标出剩余手牌数
	SIZE size;
	TCHAR szText[5];
	HFONT hfont = CreateFont(72, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("Calibri"));
	HFONT oldfont = (HFONT)SelectObject(hdcScene, hfont);
	COLORREF color = SetTextColor(hdcScene, RGB(255, 0, 128));
	SetBkMode(hdcScene, TRANSPARENT);
	int x = windowSize.cx - 50 - cardSize.cx / 2;
	int x1;
	i = 18;
	// 如果不是他们出牌
	if (i) {// 玩家2
		for (int j = 0; j < i; j++) {
			TransparentBlt(hdcScene, windowSize.cx - 50 - cardSize.cx / 2, 65 + 15 * j, cardSize.cx, cardSize.cy,
				hdcCardBack, 0, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
		}

	}

	i = 18;
	if (i) {// 玩家4
		for (int j = 0; j < i; j++) {
			TransparentBlt(hdcScene, 50 - cardSize.cx / 2, 65 + 15 * j, cardSize.cx, cardSize.cy,
				hdcCardBack, 0, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
		}
	}
	//如果是他们出牌
	//else
	/*
	for (auto rb = game->player[3]->cards.rbegin(); rb != game->player[0]->cards.rend(); ++rb){//显示玩家手牌
		if (game->player[3]->selection.cards.find(*rb) !=
			game->player[3]->selection.cards.end())//将已选择的牌上移一段，突出显示
			x1 = x - 15;
		else
			x1 = x;
		TransparentBlt(hdcPlayerCards2, windowSize.cx - 50 - cardSize.cx / 2 + x, 65 + 15 * i, cardSize.cx, cardSize.cy, hdcCards,
			cardSize.cx * *rb, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
		i++;
	}*/

	SetTextColor(hdcScene, color);
	SelectObject(hdcScene, oldfont);
	DeleteObject(hfont);

}

int Scene::PointInWhich(POINT point)
{
	//if player1 chupai
	/*auto player1 = game->player[0];

	int c = player1->cards.size(),
		x = (windowSize.cx - cardSize.cx - 22 * c + 22) / 2,
		y = windowSize.cy - cardSize.cy - 15;
	RECT rect;
	auto b = player1->cards.begin();

	for (int i = c; i > 0 && b != player1->cards.end(); --i, ++b) {
		if (player1->selection.cards.find(*b) != player1->selection.cards.end())
			rect.top = y - 15;
		else
			rect.top = y;
		rect.left = x + 22 * (i - 1);
		rect.right = rect.left + cardSize.cx;
		rect.bottom = rect.top + cardSize.cy;
		if (PtInRect(&rect, point))
			return *b;
	}*/

	/*
	//if player2 chupai
	auto player2 = game->player[1];

	int c = player1->cards.size(),
		x = (windowSize.cx - cardSize.cx - 22 * c + 22) / 2,
		y = windowSize.cy - cardSize.cy - 15;
	RECT rect;
	auto b = player1->cards.begin();

	for (int i = c; i > 0 && b != player1->cards.end(); --i, ++b) {
		if (player2->selection.cards.find(*b) != player1->selection.cards.end())
			rect.top = y - 15;
		else
			rect.top = y;
		rect.left = x + 22 * (i - 1);
		rect.right = rect.left + cardSize.cx;
		rect.bottom = rect.top + cardSize.cy;
		if (PtInRect(&rect, point))
			return *b;
	}
	*/
	return -1;
}

void Scene::ShowDiscardBtn()
{
	PostMessage(discard, WM_MYBUTTON, FALSE, 0);

	if (/*!game->lastone*/1)
		PostMessage(pass, WM_MYBUTTON, FALSE, 0);
	else
		PostMessage(pass, WM_MYBUTTON, TRUE, 0);
	ShowWindow(discard, SW_SHOW);
	ShowWindow(pass, SW_SHOW);
	ShowWindow(hint, SW_SHOW);
	InvalidateRgn(hint, NULL, FALSE);
}

void Scene::HideDiscardBtn()
{
	ShowWindow(discard, SW_HIDE);
	ShowWindow(pass, SW_HIDE);
	ShowWindow(hint, SW_HIDE);
}

void Scene::DrawResult() {

}

void Scene::ShowQuestionBtn()
{
	PostMessage(yes, WM_MYBUTTON, TRUE, 0);
	PostMessage(none, WM_MYBUTTON, TRUE, 0);


	ShowWindow(none, SW_SHOW);
	ShowWindow(yes, SW_SHOW);
}

void Scene::HideQuestionBtn()
{
	ShowWindow(none, SW_HIDE);
	ShowWindow(yes, SW_HIDE);

}
