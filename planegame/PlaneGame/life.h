#pragma once
#include "gameobject.h"

class CLife :public CGameObject
{
public:
	CLife(int nn);
	~CLife(void);

	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage();

	CRect GetRect()
	{
		return CRect(m_ptPos, CPoint(m_ptPos.x + CLIFE_HEIGHT, m_ptPos.y + CLIFE_WIDTH));
	}

	int GetMontion() const
	{
		return m_nMotion;
	}
private:
	static const int CLIFE_HEIGHT = 30;
	static const int CLIFE_WIDTH = 30;
	static CImageList m_Images;
	static CImageList m_Images1;
	int    m_nMotion;//���� 1->���� 0->ֹͣ -1->����
					 //ͼ������
	int m_nImgIndex;
	//�ٶ�
	int m_V;
	int nn;
};
