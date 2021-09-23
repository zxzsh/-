#include "stdafx.h"
#include "background.h"
#include "resource.h"

background::background()
	: x(0)
	, y(-GAME_HEIGHT)
{
}


background::~background()
{
}
void background::draw(CDC* pDC)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmpDraw;
	bmpDraw.LoadBitmap(IDB_BACK);
	CBitmap* pbmpOld = memDC.SelectObject(&bmpDraw);
	pDC->BitBlt(x, y, GAME_WIDTH, GAME_HEIGHT, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pbmpOld);


}