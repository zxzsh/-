#include "StdAfx.h"
#include "Withplane.h"
#include "resource.h"
CImageList CWithplane::m_Images;
CWithplane::~CWithplane(void)
{
}
CWithplane::CWithplane(int x, int y): m_nVerMotion1(0)
{
	//随机确定X位置
	m_ptPos.x = x;

	//随机确定图像索引
	m_nImgIndex = 0;

	M_v = 1;
	m_ptPos.y = y;
	//随机确定速度
	m_nWait = 0;
}

BOOL CWithplane::LoadImage()
{
	CGameObject::LoadImage(m_Images, IDB_BITMAP10, RGB(0, 0, 0), 25, 25, 1);
	return true;
}
BOOL CWithplane::Draw(CDC* pDC, BOOL bPause)
{
	m_nWait++;
	if (m_nWait > 4)
		m_nWait = 0;
	if (m_ptPos.x <= 27)
	{
		m_ptPos.x = 35;
		M_v = -M_v;
	}
	else if (m_ptPos.x >= GAME_WIDTH - WITHPLANE_HEIGHT - 65)
	{
		m_ptPos.x = GAME_WIDTH - WITHPLANE_HEIGHT - 73;
		M_v = -M_v;
	}
	if (!bPause)
	{
		m_ptPos.x = m_ptPos.x + M_v * 8;
		m_ptPos.y = m_ptPos.y - m_nVerMotion1 * 8;
	}
	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);

	return TRUE;
}
BOOL CWithplane::Fired()
{
	if (m_nWait == 0)
		return TRUE;
	else
		return FALSE;
}