// PlaneGameView.cpp : CPlaneGameView 类的实现
//
#include "stdafx.h"
#include "PlaneGame.h"
#include <mmsystem.h>
#include "PlaneGameDoc.h"
#include "PlaneGameView.h"
#include "MyPlane.h"
#include "Enemy.h"
#include "Bomb.h"
#include "bomb1.h"
#include "Ball.h"
#include "life.h"
#include "Explosion.h"
#include <atlimage.h>
#include "Withplane.h"
#include "background.h"
#include "boss.h"
#pragma comment(lib,"Winmm.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPlaneGameView

IMPLEMENT_DYNCREATE(CPlaneGameView, CView)

BEGIN_MESSAGE_MAP(CPlaneGameView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()

END_MESSAGE_MAP()

// CPlaneGameView 构造/析构
static int planesTime = 0;//僚机记时
static int score = 0;//计分
static int life = 5;//生命值
static int b_life;//boss生命值
static int wd = 1;//无敌状态
static int level=1;//关卡等级
static int num = 0;//boss发射子弹参数
static int sum = 1;//产生boss计数
static int p_times = 3;//召唤僚机次数
static int destroyTime = 0;//必杀持续时间
static int D_Time = 3;//剩余必杀次数
CPlaneGameView::CPlaneGameView():m_pMe(NULL)
{
	// TODO: 在此处添加构造代码	
}

CPlaneGameView::~CPlaneGameView()
{
}

BOOL CPlaneGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	return CView::PreCreateWindow(cs);
}

// CPlaneGameView 绘制

void CPlaneGameView::OnDraw(CDC* /*pDC*/)
{
	CPlaneGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 在此处为本机数据添加绘制代码
}


// CPlaneGameView 打印

BOOL CPlaneGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPlaneGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPlaneGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CPlaneGameView 诊断

#ifdef _DEBUG
void CPlaneGameView::AssertValid() const
{
	CView::AssertValid();
}

void CPlaneGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlaneGameDoc* CPlaneGameView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneGameDoc)));
	return (CPlaneGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlaneGameView 消息处理程序
void CPlaneGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 在此添加专用代码和/或调用基类
	//初始化游戏
	InitGame();
}
void CPlaneGameView::StopGame()
{
	delete m_pMe;
	delete m_pMemDC;
	delete m_pDC;
	delete m_pMemBitmap;
}

BOOL CPlaneGameView::InitGame()
{
	CRect rc;
	GetClientRect(rc);

	//产生随机数种子
	srand( (unsigned)time( NULL ) );

	//建立设备DC
	m_pDC = new CClientDC(this);

	//建立内存DC
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(m_pDC);

	//建立内存位图
	m_pMemBitmap = new CBitmap;
	m_pMemBitmap->CreateCompatibleBitmap(m_pDC,GAME_WIDTH,GAME_HEIGHT);

	//将位图选入内存DC
	m_pMemDC->SelectObject(m_pMemBitmap);

	CMyPlane::LoadImage();
	CEnemy::LoadImage();
	CBomb::LoadImage();
	CBall::LoadImage();
	CExplosion::LoadImage();
	CBomb1::LoadImage();
	CLife::LoadImage();
	CBoss::LoadImage();
	CWithplane::LoadImage();
	//产生主角(战机)
	m_pMe = new CMyPlane;
	//启动游戏
	SetTimer(1,30,NULL);
	PlaySound((LPCTSTR)IDR_WAVE1, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC | SND_LOOP);
	return TRUE;
}

void CPlaneGameView::UpdateFrame(CDC* pMemDC)
{
	//绘制天空
	pMemDC->FillSolidRect(0,0,GAME_WIDTH,GAME_HEIGHT,RGB(84, 142, 239));
	CPlaneGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	//绘制天空
	pMemDC->FillSolidRect(0, 0, GAME_WIDTH, GAME_HEIGHT, RGB(84, 142, 239));
	//绘制背景 滚动
	if (pDoc->bg[0].y == -GAME_HEIGHT || pDoc->bg[0].y == GAME_HEIGHT)	pDoc->bg[0].y = 0;
	pDoc->bg[0].draw(pMemDC);
	pDoc->bg[0].y += 1;
	pDoc->bg[1].draw(pMemDC);
	pDoc->bg[1].y += 1;
	if (pDoc->bg[1].y > 0) {
		pDoc->bg[1].y += -GAME_HEIGHT;
	}
	//绘制我方战机
	if(m_pMe!=NULL)
	{
		if (score >= 600 && score < 800 && sum == 1)
			b_life = 899;
		else if (score >= 1000 && sum == 2)
			b_life = 1799;
		level = 1+ score / 200;
		CString str1, str2, str3, str4,str5;
		str1.Format(_T("%d"), score);
		str2.Format(_T("%d"), 1+b_life/300);
		str3.Format(_T("%d"), level);
		str4.Format(_T("%d"), p_times);
		str5.Format(_T("%d"), D_Time);
		m_pMemDC->SetBkMode(TRANSPARENT);
		m_pMemDC->SetTextColor(RGB(255,0,0));
		m_pMemDC->TextOutW(0, 20, L"得分：", 3);
		m_pMemDC->TextOutW(0, 40, L"无敌：", 3);
		m_pMemDC->TextOutW(0, 60, L"关卡：", 3);
		m_pMemDC->TextOutW(0, 730, L"生命值：", 4);
		m_pMemDC->TextOutW(0, 80, L"僚机次数：", 5);
		m_pMemDC->TextOutW(0, 100, L"必杀次数：", 5);
		m_pMemDC->TextOutW(0, 120, L"F1:无敌模式");
		m_pMemDC->TextOutW(0, 140, L"Ctrl:召唤僚机");
		m_pMemDC->TextOutW(0, 160, L"C:必杀");
		m_pMemDC->TextOutW(90, 80, str4);
		m_pMemDC->TextOutW(90, 100, str5);
		m_pMemDC->TextOutW(70, 20, str1);
		m_pMemDC->TextOutW(70, 60, str3);
		if (wd % 2 == 0)
			m_pMemDC->TextOutW(70, 40, _T("开启"));
		else
			m_pMemDC->TextOutW(70, 40, _T("关闭"));

		CBrush b1(RGB(255, 0, 0));//选定一个画刷类对象b
		m_pMemDC->SelectObject(&b1);//选定该对象 20);
		for(int i=0;i<life;i++)
		m_pMemDC->Rectangle(80+i*50, 735, 80 + i * 50+40, 745);//画一个矩形
		if (score >= 600&&score<800)
		{
			m_pMemDC->TextOutW(120, 20, L"BOSS：", 5);
			m_pMemDC->TextOutW(200, 20, L" X", 2);
			m_pMemDC->TextOutW(180, 20, str2);
			CBrush b2(RGB(255, 0, 0));//选定一个画刷类对象b
			m_pMemDC->SelectObject(&b2);//选定该对象 20);
			for (int j = 0; j<b_life%300; j++)
				m_pMemDC->Rectangle(220, 20,  220+ j, 40);//画一个矩形	
		}
		else if (score >= 1000)
		{
			m_pMemDC->TextOutW(120, 20, L"BOSS：", 5);
			m_pMemDC->TextOutW(200, 20, L" X", 2);
			m_pMemDC->TextOutW(180, 20, str2);
			CBrush b2(RGB(255, 0, 0));//选定一个画刷类对象b
			m_pMemDC->SelectObject(&b2);//选定该对象 20);
			for (int j = 0; j<b_life % 300; j++)
				m_pMemDC->Rectangle(220, 20, 220 + j, 40);//画一个矩形
		}
	    m_pMe->Draw(m_pMemDC, FALSE);
		if (planesTime != 0)
		{
			plane1->Draw(m_pMemDC, FALSE);//绘制僚机
			plane2->Draw(m_pMemDC, FALSE);
		}
	}
	else
	{   //Game Over
		KillTimer(1);
		CString  strs, strr;
		strs.Format(_T("你的得分为： %d"), score);
		AfxMessageBox(strs);
		if (AfxMessageBox(L"是否重新开始游戏？", MB_YESNO) == 6)
		{
			//清空敌机
			m_ObjList[enEnemy].RemoveAll();
			//清空战机链表
			//m_ObjList[enpMe].RemoveAll();
			//清空战机子弹链表
			m_ObjList[enBall].RemoveAll();
			//清空敌机炸弹链表
			m_ObjList[enBomb].RemoveAll();
			//清空爆炸链表
			m_ObjList[enExplosion].RemoveAll();
			m_ObjList[enBoss].RemoveAll();
			sum = 1;
			//添加新的战机对象
			m_pMe = new CMyPlane;
			life = 5;
			score = 0;
			SetTimer(1, 30, NULL);
			p_times = 3;
			D_Time = 3;
		}
		else
			exit(1);	
	}
	//绘制 导弹、爆炸、敌机、子弹,导弹1,子弹1,道具,boss,僚机
	for(int i=0;i<7;i++)
	{
		POSITION pos1,pos2;
		for( pos1 = m_ObjList[i].GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
		{
			CGameObject* pObj = (CGameObject*)m_ObjList[i].GetNext( pos1 );
			if(!pObj->Draw(pMemDC,FALSE))
			{
				m_ObjList[i].RemoveAt(pos2);
				delete pObj;
			}
		}
	}
	//复制内存DC到设备DC
	m_pDC->BitBlt(0,0,GAME_WIDTH,GAME_HEIGHT,m_pMemDC,0,0,SRCCOPY);
}
static int sm = 5;
static int l_num = 0;//产生道具计数
static int b_bomb = 0;//道具强化攻击
static int b_time = 0;//道具强化攻击计时
static int life1 = 1;//敌机生命
static int life2 = 2;//敌机1生命
static int life3 = 5;//敌机2生命
void CPlaneGameView::AI()
{
	if (GetKey('C') == 1 && D_Time > 0 && destroyTime == 0)
	{
		destroyTime = 6;
		D_Time--;
	}
	if (destroyTime > 0)
		destroyTime--;
	if (sm > 0)
		sm--;
	if (GetKey(VK_CONTROL) == 1 && p_times!=0&& planesTime == 0)
	{
		plane1 = new CWithplane(m_pMe->GetPointx() + 95, m_pMe->GetPointy());//产生僚机
		plane2 = new CWithplane(m_pMe->GetPointx() - 25, m_pMe->GetPointy());
		planesTime = 300;
		p_times--;
	}
	
	if(planesTime!=0)
	    planesTime--;
	if (GetKey(VK_F1) == 1&&sm==0)
	{
		wd++;
		sm = 5;
	}
	if (score >= 600 && sum == 1)
	{
		sum=2;
		m_ObjList[enBoss].AddTail(new CBoss(0));
	}
	if (score >= 1000 && sum == 2)
	{
		sum = 3;
		m_ObjList[enBoss].AddTail(new CBoss(1));
	}
	static int nCreatorl = rand() % 5 + 300;
	//随机产生道具
	if (nCreatorl <= 0)
	{
		nCreatorl = rand() % 5 + 300;
		m_ObjList[enLife].AddTail(new CLife(l_num % 4));
		l_num++;
	}
	nCreatorl--;
	if ((score < 600)|| (score>=800&&score < 1000))
	{
		static int nCreator = rand() % 5 + 30 - (level-2) * 5;

		//随机产生敌机
		if (nCreator <= 0)
		{
			nCreator = rand() % 5 + 30 - (level-2) * 5;
			m_ObjList[enEnemy].AddTail(new CEnemy(0));
		}
		nCreator--;
		if (m_pMe == NULL)
			return;
		static int nCreator1 = rand() % 5 + 60 - (level-2) * 10;

		//随机产生敌机2
		if (nCreator1 <= 0)
		{
			nCreator1 = rand() % 5 + 60 - (level-2) * 8;
			m_ObjList[enEnemy].AddTail(new CEnemy(1));
		}
		nCreator1--;
		if (m_pMe == NULL)
			return;
		//随机产生敌机3
		static int nCreator2 = rand() % 5 + 180 - (level-2) * 15;
		if (nCreator2 <= 0)
		{
			nCreator2 = rand() % 5 + 180 - (level-2) * 12;
			m_ObjList[enEnemy].AddTail(new CEnemy(2));
		}
		nCreator2--;
		if (m_pMe == NULL)
			return;
	}
		//检测四个方向键，移动战机
		for (int i = 0; i < 4; i++)
		{
			int nMeMotion = 0;
			int lMeMotion = 0;
			m_pMe->SetVerMotion(0);
			m_pMe->SetHorMotion(0);
			if (planesTime != 0)
			{
				plane1->SetVerMotion1(0);
				plane2->SetVerMotion1(0);
			}
			nMeMotion = GetKey(VK_UP);
			if (nMeMotion == 1)
			{
				m_pMe->SetVerMotion(1);
				if (planesTime != 0)
				{
					plane1->SetVerMotion1(1);
					plane2->SetVerMotion1(1);
				}
			}

			nMeMotion = GetKey(VK_DOWN);
			if (nMeMotion == 1)
			{
				m_pMe->SetVerMotion(-1);
				if (planesTime != 0)
				{
					plane1->SetVerMotion1(-1);
					plane2->SetVerMotion1(-1);
				}
			}

			nMeMotion = GetKey(VK_RIGHT);
			if (nMeMotion == 1)
				m_pMe->SetHorMotion(1);

			nMeMotion = GetKey(VK_LEFT);
			if (nMeMotion == 1)
				m_pMe->SetHorMotion(-1);
		}
		if (planesTime != 0 && plane1->Fired())
		{
			CPoint pt = plane1->GetPoint();
			CPoint pt1 = plane2->GetPoint();
			m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 5, pt.y, 1, 1));
			m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 5, pt.y, -1, 1));
			m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 5, pt.y, 0, 1));
			m_ObjList[enBomb1].AddTail(new CBomb1(pt1.x +5, pt.y, 0, 1));
			m_ObjList[enBomb1].AddTail(new CBomb1(pt1.x +5, pt.y, 1, 1));
			m_ObjList[enBomb1].AddTail(new CBomb1(pt1.x +5, pt.y, -1, 1));
		}
	//Boss发射子弹
	if (m_pMe != NULL && score >= 600)
	{
		for (POSITION ePos = m_ObjList[enBoss].GetHeadPosition(); ePos != NULL;)
		{
			CBoss* pBoss = (CBoss*)m_ObjList[enBoss].GetNext(ePos);
			if (!pBoss->Fired())
				continue;
			CPoint  ePt = pBoss->GetPoint();
			if (num % 3 == 0)
			{
				m_ObjList[enBall].AddTail(new CBall(ePt.x, ePt.y - 20, pBoss->GetMontion(),0,8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 30, ePt.y - 15, pBoss->GetMontion(),0, 8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 60, ePt.y - 10, pBoss->GetMontion(), 0, 8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 90, ePt.y - 5, pBoss->GetMontion(), 0, 8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 120, ePt.y - 5, pBoss->GetMontion(), 0, 8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 150, ePt.y - 10, pBoss->GetMontion(), 0, 8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 180, ePt.y - 15, pBoss->GetMontion(), 0, 8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 210, ePt.y - 20, pBoss->GetMontion(), 0, 8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x, ePt.y -10, pBoss->GetMontion(), 0, 8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 30, ePt.y - 5, pBoss->GetMontion(), 0, 8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 60, ePt.y - 0, pBoss->GetMontion(), 0, 8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 90, ePt.y + 5, pBoss->GetMontion(), 0, 8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 120, ePt.y + 5, pBoss->GetMontion(), 0, 8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 150, ePt.y - 0, pBoss->GetMontion(), 0, 8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 180, ePt.y - 5, pBoss->GetMontion(), 0, 8,0));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 210, ePt.y - 10, pBoss->GetMontion(), 0, 8,0));
				num++;
			}
			else if(num % 3 == 1)
			{
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 30, ePt.y + 10, pBoss->GetMontion(),-1,8,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 30, ePt.y + 10, pBoss->GetMontion(), 0,8,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 30, ePt.y + 10, pBoss->GetMontion(), 1,8,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 90, ePt.y + 10, pBoss->GetMontion(), -2,8,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 90, ePt.y + 10, pBoss->GetMontion(), 0,8,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 90, ePt.y + 10, pBoss->GetMontion(), 2,8,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 150, ePt.y + 10, pBoss->GetMontion(), -1,8,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 150, ePt.y + 10, pBoss->GetMontion(), 0,8,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 150, ePt.y + 10, pBoss->GetMontion(), 1,8,1));
				num++;
			}
			else if (num % 3 == 2)
			{
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 50, ePt.y + 10, pBoss->GetMontion(), -4,-4,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 50, ePt.y + 10, pBoss->GetMontion(), 4,4,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 50, ePt.y + 10, pBoss->GetMontion(), 4,-4,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 50, ePt.y + 10, pBoss->GetMontion(),-4,4,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 50, ePt.y + 10, pBoss->GetMontion(), -6,0,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 50, ePt.y + 10, pBoss->GetMontion(),6,0,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 50, ePt.y + 10, pBoss->GetMontion(), 0,6,1));
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 50, ePt.y + 10, pBoss->GetMontion(), 0,-6,1));
				num++;
			}
		}
	}
	//敌机1,2,3发射子弹
	CPoint PlanePt = m_pMe->GetPoint();
	for (POSITION ePos = m_ObjList[enEnemy].GetHeadPosition(); ePos != NULL;)
	{
		CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos);
		if (!pEnemy->Fired())
			continue;
		CPoint  ePt = pEnemy->GetPoint();
		BOOL by = FALSE;
		if (pEnemy->getkind() == 0)
			//敌机在战机前面
		{
			if (pEnemy->GetMontion() == 1 && ePt.y < PlanePt.y)
				by = TRUE;
			if (by && ePt.x >= PlanePt.x && ePt.x < PlanePt.x + CMyPlane::PLANE_WIDTH)
				m_ObjList[enBall].AddTail(new CBall(ePt.x + 18, ePt.y + 20, pEnemy->GetMontion(), 0, 8,0));
		}
		else if (pEnemy->getkind() == 1)
		{
			m_ObjList[enBall].AddTail(new CBall(ePt.x + 40, ePt.y + 40, pEnemy->GetMontion(), 8, 8,0));
			m_ObjList[enBall].AddTail(new CBall(ePt.x + 40, ePt.y + 40, pEnemy->GetMontion(), -8, 8,0));
			m_ObjList[enBall].AddTail(new CBall(ePt.x + 40, ePt.y + 40, pEnemy->GetMontion(), -8, -8,0));
			m_ObjList[enBall].AddTail(new CBall(ePt.x + 40, ePt.y + 40, pEnemy->GetMontion(), 8, -8,0));
		}
		else
		{
			m_ObjList[enBall].AddTail(new CBall(ePt.x + 80, ePt.y + 20, pEnemy->GetMontion(), -1,8,1));
			m_ObjList[enBall].AddTail(new CBall(ePt.x + 80, ePt.y + 20, pEnemy->GetMontion(), 0,8,1));
			m_ObjList[enBall].AddTail(new CBall(ePt.x + 80, ePt.y + 20, pEnemy->GetMontion(), 1,8,1));
			m_ObjList[enBall].AddTail(new CBall(ePt.x - 10, ePt.y + 20, pEnemy->GetMontion(), -1,8,1));
			m_ObjList[enBall].AddTail(new CBall(ePt.x - 10, ePt.y + 20, pEnemy->GetMontion(), 0,8,1));
			m_ObjList[enBall].AddTail(new CBall(ePt.x - 10, ePt.y + 20, pEnemy->GetMontion(), 1,8,1));
		}
	}
	//产生战机导弹
	if (GetKey(VK_SPACE) == 1|| planesTime!=0)//按下了空格键
	{
		if (m_pMe != NULL && m_pMe->Fired())
		{
			CPoint pt = m_pMe->GetPoint();

			if (b_time != 0)
			{
				m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 10, pt.y, -2, 2));
				m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 10, pt.y, -1, 1));
				m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 55, pt.y - 10, 0, 1));
				m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 30, pt.y - 10, 0, 1));
				m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 75, pt.y, 1, 1));
				m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 75, pt.y, 2, 2));
				if (b_time != 0)
					b_time--;
			}
			else {
				if (score >= 200 && score < 400)
				{
					m_ObjList[enBomb].AddTail(new CBomb(pt.x + 10, pt.y + 0, 0));
					m_ObjList[enBomb].AddTail(new CBomb(pt.x + 55, pt.y - 10, 0));
					m_ObjList[enBomb].AddTail(new CBomb(pt.x + 30, pt.y - 10, 0));
					m_ObjList[enBomb].AddTail(new CBomb(pt.x + 75, pt.y + 0, 0));
				}
				else if (score < 200)
				{
					m_ObjList[enBomb].AddTail(new CBomb(pt.x + 55, pt.y - 10, 0));
					m_ObjList[enBomb].AddTail(new CBomb(pt.x + 30, pt.y - 10, 0));
				}
				else
				{
					m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 10, pt.y, -1, 0));
					m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 55, pt.y - 10, 0, 0));
					m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 30, pt.y - 10, 0, 0));
					m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 75, pt.y, 1, 0));
				}
			}
		}
	}
	POSITION bPos1 = NULL, bPos2 = NULL;//敌机子弹炸掉战机
	CRect mRect = m_pMe->GetRect();
	for (bPos1 = m_ObjList[enBall].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CBall* pBall = (CBall*)m_ObjList[enBall].GetNext(bPos1);
		CRect bRect = pBall->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			//添加爆炸效果
			m_ObjList[enExplosion].AddTail(
				new CExplosion(mRect.left, mRect.top)
			);
			//删除子弹
			m_ObjList[enBall].RemoveAt(bPos2);
			delete pBall;
			//删除战机
			if (wd % 2 == 1&& planesTime==0)
			{
				if (life == 0)
				{
					delete m_pMe;
					m_pMe = NULL;
				}
				else
					life--;
			}
			break;
		}
	}
	if (m_pMe != NULL)
	{//敌机撞我军
		POSITION nPos1 = NULL, nPos2 = NULL;
		CRect nRect = m_pMe->GetRect();
		for (nPos1 = m_ObjList[enEnemy].GetHeadPosition(); (nPos2 = nPos1) != NULL;)
		{
			CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(nPos1);
			CRect BRect = pEnemy->GetRect();
			CRect tmpRect1;
			if (tmpRect1.IntersectRect(&BRect, nRect))
			{
				m_ObjList[enExplosion].AddTail(
					new CExplosion(nRect.left, nRect.top)
				);
				m_ObjList[enEnemy].RemoveAt(nPos2);
				score++;
				delete pEnemy;
				if (wd % 2 == 1&& planesTime==0)
				{
					if (life == 0)
					{
						delete m_pMe;
						m_pMe = NULL;
					}
					else
						life--;
				}
				break;
			}
		}
	}
	//boss炸我军：撞就死别浪
	if (m_pMe != NULL)
	{	
		POSITION nsbPos1 = NULL, nsbPos2 = NULL;
		CRect nRect = m_pMe->GetRect();
		for (nsbPos1 = m_ObjList[enBoss].GetHeadPosition(); (nsbPos2 = nsbPos1) != NULL;)
		{
			CBoss* pBoss = (CBoss*)m_ObjList[enBoss].GetNext(nsbPos1);
			CRect BRect = pBoss->GetRect();
			CRect tmpRect1;
			if (tmpRect1.IntersectRect(&BRect, nRect))
			{
				m_ObjList[enExplosion].AddTail(
					new CExplosion(nRect.left, nRect.top)
				);
				b_life--;
				if (b_life <= 0)
				{
					score += 200;
					m_ObjList[enBoss].RemoveAt(nsbPos2);
					if (sum == 3)
					{
						delete m_pMe;
						m_pMe = NULL;
					}
				}
				if (wd % 2 == 1&& planesTime==0)
				{
					if (life == 0)
					{
						delete m_pMe;
						m_pMe = NULL;
					}
					else
						life--;
				}
				break;
			}
		}
	}
	//碰撞道具
	if (m_pMe != NULL)
	{
		POSITION nlPos1 = NULL, nlPos2 = NULL;
		CRect nlRect = m_pMe->GetRect();
		for (nlPos1 = m_ObjList[enLife].GetHeadPosition(); (nlPos2 = nlPos1) != NULL;)
		{
			CLife* pLife = (CLife*)m_ObjList[enLife].GetNext(nlPos1);
			CRect BRect = pLife->GetRect();
			CRect tmpRect1;
			if (tmpRect1.IntersectRect(&BRect, nlRect))
			{
				if(l_num%4==0)
					b_time = 80;
				else
				{
					if (life <= 7)
						life++;
				}
				m_ObjList[enLife].RemoveAt(nlPos2);
				delete pLife;
				break;
			}
		}
	}
	if (score < 400)
	{
		//战机导弹炸掉敌机
		POSITION mPos1 = NULL, mPos2 = NULL;
		for (mPos1 = m_ObjList[enBomb].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CBomb* pBomb = (CBomb*)m_ObjList[enBomb].GetNext(mPos1);
			CRect bRect = pBomb->GetRect();

			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enEnemy].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos1);
				CRect mRect = pEnemy->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(
						new CExplosion(mRect.left, mRect.top)
					);
					//删除导弹
					m_ObjList[enBomb].RemoveAt(mPos2);
					delete pBomb;
					//删除敌机
					if (pEnemy->getkind() == 0)
					{
						if (life1 == 0)
						{
							m_ObjList[enEnemy].RemoveAt(ePos2);
							delete pEnemy;
							score++;
							life1 = 1;
						}
						else
							life1--;
					}
					else if (pEnemy->getkind() == 1)
					{
						if (life2 == 0)
						{
							m_ObjList[enEnemy].RemoveAt(ePos2);
							delete pEnemy;
							score+=2;
							life2 = level+1;
						}
						else
							life2--;
					}
					else
					{
						if (life3 == 0)
						{
							m_ObjList[enEnemy].RemoveAt(ePos2);
							delete pEnemy;
							score+=5;
							life3 = 4+level;
						}
						else
							life3--;
					}
					break;
				}
			}
		}
	}
	if(score<2000)
	{
		//战机导弹炸掉敌机
		POSITION mPos1 = NULL, mPos2 = NULL;
		for (mPos1 = m_ObjList[enBomb1].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CBomb1* pBomb1 = (CBomb1*)m_ObjList[enBomb1].GetNext(mPos1);
			CRect bRect = pBomb1->GetRect();

			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enEnemy].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos1);
				CRect mRect = pEnemy->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(
						new CExplosion(mRect.left, mRect.top)
					);
					//删除导弹
					m_ObjList[enBomb1].RemoveAt(mPos2);
					delete pBomb1;
					//删除敌机
					if (pEnemy->getkind() == 0)
					{
						if (life1 == 0)
						{
							m_ObjList[enEnemy].RemoveAt(ePos2);
							delete pEnemy;
							score++;
							life1 = 1;
						}
						else
							life1--;
					}
					else if (pEnemy->getkind() == 1)
					{
						if (life2 == 0)
						{
							m_ObjList[enEnemy].RemoveAt(ePos2);
							delete pEnemy;
							score += 2;
							life2 = 2;
						}
						else
							life2--;
					}
					else
					{
						if (life3 == 0)
						{
							m_ObjList[enEnemy].RemoveAt(ePos2);
							delete pEnemy;
							score += 5;
							life3 = 5;
						}
						else
							life3--;
					}
					break;
				}
			}
		}
		if (destroyTime > 0)
		{
			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enEnemy].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos1);
				CRect mRect = pEnemy->GetRect();
				m_ObjList[enExplosion].AddTail(
					new CExplosion(mRect.left, mRect.top)
				);
				m_ObjList[enEnemy].RemoveAt(ePos2);
				delete pEnemy;
			}
		}
		//战机导弹炸掉boss
		POSITION mbPos1 = NULL, mbPos2 = NULL;
		for (mbPos1 = m_ObjList[enBomb1].GetHeadPosition(); (mbPos2 = mbPos1) != NULL;)
		{
			CBomb1* pBomb1 = (CBomb1*)m_ObjList[enBomb1].GetNext(mbPos1);
			CRect bbRect = pBomb1->GetRect();
			POSITION ebPos1 = NULL, ebPos2 = NULL;
			for (ebPos1 = m_ObjList[enBoss].GetHeadPosition(); (ebPos2 = ebPos1) != NULL;)
			{
				CBoss* pBoss = (CBoss*)m_ObjList[enBoss].GetNext(ebPos1);
				CRect mbRect = pBoss->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bbRect, mbRect))
				{
					//添加爆炸效果
						m_ObjList[enExplosion].AddTail(
							new CExplosion(mbRect.left, mbRect.top)
						);
					//删除导弹
					m_ObjList[enBomb1].RemoveAt(mbPos2);
					delete pBomb1;
					//删除boss
					if (b_life <= 0)
					{
						m_ObjList[enBoss].RemoveAt(ebPos2);
						score += 200;
						if(sum==3)
						    m_pMe = NULL;
						break;
					}
					else
						b_life--;
				}
			}
		}
	}
}
void CPlaneGameView::OnTimer(UINT_PTR nIDEvent)
{
	//刷新游戏帧画面: 在内存DC上绘图
	UpdateFrame(m_pMemDC);
	AI();
	CView::OnTimer(nIDEvent);
}

