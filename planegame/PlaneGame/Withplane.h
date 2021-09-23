#pragma once
#include "gameobject.h"

class CWithplane :public CGameObject
{
public:
	CWithplane(int x, int y);
	~CWithplane(void);

	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage();
	void SetVerMotion1(int nMotion)
	{
		m_nVerMotion1 = nMotion;
	}
	int GetVerMotion1() const
	{
		return m_nVerMotion1;
	}
	CRect GetRect()
	{
		return CRect(m_ptPos, CPoint(m_ptPos.x + WITHPLANE_HEIGHT, m_ptPos.y + WITHPLANE_HEIGHT));
	}
	//是否可以开火发射子弹
	BOOL Fired();
private:
	static const int WITHPLANE_HEIGHT = 25;
	static CImageList m_Images;
	int    m_nVerMotion1;//飞机垂直运行方向0->静止，1->上 -1->下
	//图像索引
	int m_nImgIndex;
	int m_nWait;//发射延时
	int M_v;
};
