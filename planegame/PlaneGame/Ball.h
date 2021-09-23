#pragma once
#include "GameObject.h"

class CBall :public CGameObject
{
public:
	CBall(int x,int y,int nMontion,int xx,int yy,int z);
	~CBall(void);

	BOOL Draw(CDC* pDC,BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos,CPoint(m_ptPos.x+BALL_HEIGHT,m_ptPos.y+BALL_HEIGHT));
	}
private:
	int BALL_HEIGHT;
	static CImageList m_Images;
	static CImageList m_Images1;
	int    m_nMotion;
	int wx;
	int hy;
	int kind;
};
