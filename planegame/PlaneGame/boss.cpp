#include "StdAfx.h"
#include "boss.h"
#include "resource.h"

CImageList CBoss::m_Images;
CImageList CBoss::m_Images1;

CBoss::CBoss(int b)
{
	    B = b;
		//随机确定y位置
		m_ptPos.y = 40;

		//随机确定图像索引
		m_nImgIndex = 0;

		//根据图像索引确定方向
		m_nMotion = 1;
		m_ptPos.x = 0;
		if (B == 0)
		{
			BOSS_WIDTH = 180;
			BOSS_HEIGHT = 165;
		}
		else
		{
			BOSS_WIDTH = 135;
			BOSS_HEIGHT = 200;
		}
		//随机确定速度
		m_V = 2;
		M_V = 2;
		m_nWait = 60;
	}

CBoss::~CBoss(void)
	{
	}
	BOOL CBoss::LoadImage()
	{
		CGameObject::LoadImage(m_Images, IDB_BITMAP6, RGB(255, 255, 255), 180, 165, 1);
		CGameObject::LoadImage(m_Images1, IDB_BITMAP9, RGB(255, 255, 255), 135, 200, 1);
		return true;
	}
	BOOL CBoss::Draw(CDC* pDC, BOOL bPause)
	{
		m_nWait++;
		if (m_nWait>100)
			m_nWait = 0;

		if (!bPause)
		{
			if (B == 0)
			{
				if (m_ptPos.x == 0)
					m_V = -m_V;
				else if (m_ptPos.x == GAME_WIDTH - BOSS_WIDTH - 21)
					m_V = -m_V;
				if (m_ptPos.y == 0)
					M_V = -M_V;
				else if (m_ptPos.y == GAME_HEIGHT - 500)
					M_V = -M_V;
			}
			else
			{
				if (m_ptPos.x == 0)
					m_V = -m_V;
				else if (m_ptPos.x == GAME_WIDTH - BOSS_WIDTH-20)
					m_V = -m_V;
				if (m_ptPos.y == 0)
					M_V = -M_V;
				else if (m_ptPos.y == GAME_HEIGHT - 500)
					M_V = -M_V;
			}
			m_ptPos.x = m_ptPos.x - m_nMotion * m_V;
		    m_ptPos.y = m_ptPos.y  - M_V;
		}

		//if (m_ptPos.x > GAME_WIDTH - BOSS_WIDTH)
			//return FALSE;
		//if (m_ptPos.x < 0)
			//return FALSE;

		
		if(B==0)
		    m_Images.Draw(pDC, m_nImgIndex, m_ptPos, ILD_TRANSPARENT);
		else
			m_Images1.Draw(pDC, m_nImgIndex, m_ptPos, ILD_TRANSPARENT);
		return TRUE;
	}
	BOOL CBoss::Fired()
	{
		if (m_nWait == 0)
			return TRUE;
		else
			return FALSE;
	}