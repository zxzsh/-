#include "StdAfx.h"
#include "Ball.h"
#include "resource.h"

CImageList CBall::m_Images;
CImageList CBall::m_Images1;
CBall::CBall(int x,int y,int nMontion,int xx,int yy,int z):CGameObject(x,y),m_nMotion(nMontion),wx(xx),hy(yy),kind(z)
{

}

CBall::~CBall(void)
{
}
BOOL CBall::Draw(CDC* pDC,BOOL bPause)
{
	if(!bPause)
	{
		m_ptPos.y = m_ptPos.y + m_nMotion * hy;
		m_ptPos.x = m_ptPos.x + m_nMotion * wx;
	}
	if (kind == 0)
	{
		BALL_HEIGHT = 8;
		if (m_ptPos.y > GAME_HEIGHT + BALL_HEIGHT)
			return FALSE;
		if (m_ptPos.y < -BALL_HEIGHT)
			return FALSE;
		m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);
	}
	if (kind == 1)
	{
		BALL_HEIGHT = 30;
		if (m_ptPos.y > GAME_HEIGHT + BALL_HEIGHT)
			return FALSE;
		if (m_ptPos.y < -BALL_HEIGHT)
			return FALSE;
		m_Images1.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);
	}
	return TRUE;
}

BOOL CBall::LoadImage()
{
	CGameObject::LoadImage(m_Images, IDB_BALL,RGB(0,0,0),8,8,1);
	CGameObject::LoadImage(m_Images1, IDB_BITMAP3, RGB(255, 255, 255), 30, 30, 1);
	return true;
}