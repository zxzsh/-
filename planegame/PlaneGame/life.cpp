#include "StdAfx.h"
#include "life.h"
#include "resource.h"

CImageList CLife::m_Images;
CImageList CLife::m_Images1;

CLife::CLife(int n)
{
	nn = n;
	if(n==1)
	//随机确定X位置
	m_ptPos.x = rand() % (GAME_WIDTH - CLIFE_HEIGHT) + 1;

	//随机确定图像索引
	m_nImgIndex = 0;

	//根据图像索引确定方向
	m_nMotion = 1;
	m_ptPos.y = -CLIFE_HEIGHT;
	//随机确定速度
	m_V = rand() % 6 + 2;

}

CLife::~CLife(void)
{
}
BOOL CLife::LoadImage()
{
	CGameObject::LoadImage(m_Images, IDB_BITMAP5, RGB(255, 255, 255), 30, 30, 1);
	CGameObject::LoadImage(m_Images1, IDB_BITMAP8, RGB(255, 255, 255), 30, 24, 1);
	return true;
}
BOOL CLife::Draw(CDC* pDC, BOOL bPause)
{
	

	if (!bPause)
	{
		m_ptPos.y = m_ptPos.y + m_nMotion * m_V;
	}

	if (m_ptPos.y > GAME_HEIGHT + CLIFE_HEIGHT)
		return FALSE;
	if (m_ptPos.y < -CLIFE_HEIGHT)
		return FALSE;
	if(nn==3)
		m_Images1.Draw(pDC, m_nImgIndex, m_ptPos, ILD_TRANSPARENT);
	else
		m_Images.Draw(pDC, m_nImgIndex, m_ptPos, ILD_TRANSPARENT);

	return TRUE;
}
