#include "StdAfx.h"
#include "Enemy.h"
#include "resource.h"

CImageList CEnemy::m_Images;
CImageList CEnemy::m_Images1;
CImageList CEnemy::m_Images2;
CEnemy::CEnemy(int x):kind(x)
{
	if (kind == 0)
	{
		ENEMY_HEIGHT=40;
		ENEMY_WIDTH = 40;
		//随机确定X位置
		m_ptPos.x = rand() % (GAME_WIDTH - ENEMY_HEIGHT) + 1;

		//随机确定图像索引
		m_nImgIndex = 0;

		//根据图像索引确定方向
		m_nMotion = 1;
		m_ptPos.y = -ENEMY_HEIGHT;
		//随机确定速度
		m_V = rand() % 6 + 2;
		m_nWait = 0;
	}
	else if (kind == 1)
	{
		ENEMY_HEIGHT = 63;
		ENEMY_WIDTH = 85;
		//随机确定X位置
		m_ptPos.x = rand() % (GAME_WIDTH - ENEMY_HEIGHT) + 1;

		//随机确定图像索引
		m_nImgIndex = 0;

		//根据图像索引确定方向
		m_nMotion = 1;
		m_ptPos.y = -ENEMY_HEIGHT;

		//随机确定速度
		m_V = rand() % 6 + 2;

		m_nWait = 0;
	}
	else
	{
		ENEMY_HEIGHT = 77;
		ENEMY_WIDTH = 150;
		//随机确定y位置
		m_ptPos.y = rand() % 80;

		//随机确定图像索引
		m_nImgIndex = 0;

		//根据图像索引确定方向
		m_nMotion = 1;
		m_ptPos.x = 0;

		//随机确定速度
		m_V = 2;
		m_nWait = 40;
	}
}
int CEnemy::getkind()
{
	return kind;
}
CEnemy::~CEnemy(void)
{
}
BOOL CEnemy::LoadImage()
{
	CGameObject::LoadImage(m_Images, IDB_ENEMY,RGB(0,0,0),40,40,2);
	CGameObject::LoadImage(m_Images1, IDB_BITMAP1, RGB(255, 255, 255), 85, 63, 1);
	CGameObject::LoadImage(m_Images2, IDB_BITMAP2, RGB(255, 255, 255), 150, 77, 1);
	return true;
}
BOOL CEnemy::Draw(CDC* pDC,BOOL bPause)
{
	if (kind == 0)
	{
		m_nWait++;
		if (m_nWait > 20)
			m_nWait = 0;
		if (!bPause)
		{
			m_ptPos.y = m_ptPos.y + m_nMotion * m_V;
		}

		if (m_ptPos.y > GAME_HEIGHT + ENEMY_HEIGHT)
			return FALSE;
		if (m_ptPos.y < -ENEMY_HEIGHT)
			return FALSE;

		m_Images.Draw(pDC, m_nImgIndex, m_ptPos, ILD_TRANSPARENT);
	}
	else if(kind==1)
	{
		m_nWait++;
		if (m_nWait>60)
			m_nWait = 0;

		if (!bPause)
		{
			m_ptPos.y = m_ptPos.y + m_nMotion * m_V;
		}

		if (m_ptPos.y > GAME_HEIGHT + ENEMY_HEIGHT)
			return FALSE;
		if (m_ptPos.y < -ENEMY_HEIGHT)
			return FALSE;

		m_Images1.Draw(pDC, m_nImgIndex, m_ptPos, ILD_TRANSPARENT);
	}
	else
	{
		m_nWait++;
		if (m_nWait>100)
			m_nWait = 0;
		if (!bPause)
		{
			m_ptPos.x = m_ptPos.x + m_nMotion * m_V;
			m_ptPos.y = m_ptPos.y + 1;
		}

		if (m_ptPos.x > GAME_HEIGHT + ENEMY_WIDTH)
			return FALSE;
		if (m_ptPos.x < -ENEMY_WIDTH)
			return FALSE;

		m_Images2.Draw(pDC, m_nImgIndex, m_ptPos, ILD_TRANSPARENT);
	}
	return TRUE;
}
BOOL CEnemy::Fired()
{
	if(m_nWait==0)
		return TRUE;
	else
		return FALSE;
}