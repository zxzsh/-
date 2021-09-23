#include "StdAfx.h"
#include "bomb1.h"
#include "resource.h"


CImageList CBomb1::m_Images;
CImageList CBomb1::m_Images1;

CBomb1::CBomb1(int x, int y, int z,int n) :CGameObject(x, y), ww(z)
{
	nn = n;
	if (nn == 0)
	{
		BOMB1_HEIGHT = 28;
		BOMB1_WIDTH = 20;
	}
	else
	{
		BOMB1_HEIGHT = 35;
		BOMB1_WIDTH = 10;
	}
}

CBomb1::~CBomb1(void)
{
}
BOOL CBomb1::LoadImage()
{
	    CGameObject::LoadImage(m_Images, IDB_BITMAP4, RGB(255, 255, 255), 20, 28, 1);
		CGameObject::LoadImage(m_Images1, IDB_BITMAP7, RGB(255, 255, 255), 10, 35, 1);
		return true;
}
BOOL CBomb1::Draw(CDC* pDC, BOOL bPause)
{
	if (!bPause)
	{
		m_ptPos.y = m_ptPos.y - 8;
		m_ptPos.x = m_ptPos.x + ww;
	}

	if (m_ptPos.y < -BOMB1_HEIGHT)
		return FALSE;
	if (nn == 1)
	    m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);
	else
		m_Images1.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);
	return TRUE;
}