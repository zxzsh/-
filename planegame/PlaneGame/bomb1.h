#pragma once
#include "gameobject.h"

class CBomb1 :
	public CGameObject
{
public:
	CBomb1(int x, int y, int z,int n);
	~CBomb1(void);

	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos, CPoint(m_ptPos.x + BOMB1_WIDTH, m_ptPos.y + BOMB1_HEIGHT));
	}
private:
	int BOMB1_HEIGHT;
	int BOMB1_WIDTH ;
	static CImageList m_Images;
	static CImageList m_Images1;
	int ww;
    int nn;
};
