#pragma once
#include "gameobject.h"

class CEnemy :public CGameObject
{
public:
	CEnemy(int x);
	~CEnemy(void);

	BOOL Draw(CDC* pDC,BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos,CPoint(m_ptPos.x+ENEMY_WIDTH,m_ptPos.y+ENEMY_HEIGHT));
	}

	int GetMontion() const
	{
		return m_nMotion;
	}
	int getkind();
	//是否可以开火发射子弹
	BOOL Fired();
private:
	int ENEMY_HEIGHT;
	int ENEMY_WIDTH;
	static CImageList m_Images;
	static CImageList m_Images1;
	static CImageList m_Images2;
	int    m_nMotion;//方向 1->向下 0->停止 -1->向上
	//图像索引
	int m_nImgIndex;
	//速度
	int m_V;
	int    m_nWait;//发射延时
	int kind;
};
