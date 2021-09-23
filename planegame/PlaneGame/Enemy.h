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
	//�Ƿ���Կ������ӵ�
	BOOL Fired();
private:
	int ENEMY_HEIGHT;
	int ENEMY_WIDTH;
	static CImageList m_Images;
	static CImageList m_Images1;
	static CImageList m_Images2;
	int    m_nMotion;//���� 1->���� 0->ֹͣ -1->����
	//ͼ������
	int m_nImgIndex;
	//�ٶ�
	int m_V;
	int    m_nWait;//������ʱ
	int kind;
};
