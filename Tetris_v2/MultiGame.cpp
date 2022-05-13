// MultiGame.cpp: 实现文件
//

#include "pch.h"
#include "Tetris_v2.h"
#include "MultiGame.h"
#include "afxdialogex.h"
#include "Option.h"


// MultiGame 对话框

IMPLEMENT_DYNAMIC(MultiGame, CDialogEx)

MultiGame::MultiGame(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MULTIPLAYER, pParent)
{

}

MultiGame::~MultiGame()
{
}


BEGIN_MESSAGE_MAP(MultiGame, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_MESSAGE(WM_MYMSG,OnMyMsgHandler)
	ON_BN_CLICKED(IDC_BUTTON_MSTART, &MultiGame::OnBnClickedButtonMstart)
	ON_BN_CLICKED(IDC_BUTTON_MPAUSE, &MultiGame::OnBnClickedButtonMpause)
	ON_BN_CLICKED(IDC_BUTTON_MRULES, &MultiGame::OnBnClickedButtonMrules)
	ON_BN_CLICKED(IDC_BUTTON_MQUIT, &MultiGame::OnBnClickedButtonMquit)
	ON_BN_CLICKED(IDC_BUTTON_MEASY, &MultiGame::OnBnClickedButtonMeasy)
	ON_BN_CLICKED(IDC_BUTTON_MNORMAL, &MultiGame::OnBnClickedButtonMnormal)
	ON_BN_CLICKED(IDC_BUTTON_MDIFFICULT, &MultiGame::OnBnClickedButtonMdifficult)
	ON_BN_CLICKED(IDC_BUTTON_MCHALLANGE, &MultiGame::OnBnClickedButtonMchallange)
	ON_BN_CLICKED(IDC_BUTTON_MOPTION, &MultiGame::OnBnClickedButtonMoption)
END_MESSAGE_MAP()





LRESULT MultiGame::OnMyMsgHandler(WPARAM, LPARAM)
{
	return LRESULT();
}

void MultiGame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



#define height 24			//游戏网格的高
#define width 10			//游戏网格的宽
#define space 35			//网格的大小
#define init_speed 1000		//游戏的初始速度 1000ms
#define w_space 35			//网格的左边界距离窗口左边界的距离
#define h_space 70			//网格上端距离窗口上端的距离
#define sw_space 490		//小网格...
#define sh_space 105		//小网格...
#define score_x 490			//分数的坐标x
#define score_y 420			//分数的坐标y
#define text_size 160		//字体的大小
#define level_x 490			//等级的坐标x
#define level_y 315			//等级的坐标y
#define maker_x 490
#define maker_y 525
#define easy_speed 1000
#define normal_speed 700
#define difficult_speed 500
#define stop 100000000		//停止时的速度,尽量大,不要随意修改
#define normal_block 3
#define difficult_block 6
#define playerplus 660
#define winscore 2000


using namespace std;
int POX1 = 0, POY1 = 3;
int POX2 = 0, POY2 = 3;
static int score1 = 0;
static int score2 = 0;
static int level1 = 1;
static int level2 = 1;
static int difficulty = 0;
static int speed = init_speed;
static bool ifcan[4];
static int k;
static int tempdifficulty = 0;
static int tempstop = 0;
static int maxlevel = 1;






struct MMAP
{
	COLORREF rgb = RGB(255, 255, 255);
};
MMAP mmap1[height][width];
MMAP smmap1[4][4];
MMAP mcontrol1[4][4];
MMAP mmap2[height][width];
MMAP smmap2[4][4];
MMAP mcontrol2[4][4];
int mn_code;


struct MBLOCK
{
	MMAP mmap1[4][4];
	MMAP mmap2[4][4];
}mblock[24];




void MultiGame::PaintMapP1()
{
	CDC* pDC = GetDC();

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			CBrush mapbrush;
			mapbrush.CreateSolidBrush(mmap1[i][j].rgb);
			pDC->SelectObject(&mapbrush);
			pDC->Rectangle(w_space + space * j, h_space + space * i, w_space + space * (j + 1), h_space + space * (i + 1));
		}
	}
	ReleaseDC(pDC);

}


void MultiGame::PaintMapP2()
{
	CDC* pDC = GetDC();

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			CBrush mapbrush;
			mapbrush.CreateSolidBrush(mmap2[i][j].rgb);
			pDC->SelectObject(&mapbrush);
			pDC->Rectangle(w_space + playerplus + space * j, h_space + space * i, w_space + playerplus + space * (j + 1), h_space + space * (i + 1));
		}
	}
	ReleaseDC(pDC);

}



void MultiGame::PaintSMapP1()
{
	CDC* pDC = GetDC();


	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			CBrush smapbrush;
			smapbrush.CreateSolidBrush(smmap1[i][j].rgb);
			pDC->SelectObject(&smapbrush);
			pDC->Rectangle(sw_space + space * i, sh_space + space * j, sw_space + space * (i + 1), sh_space + space * (j + 1));

		}
	}
	ReleaseDC(pDC);
}




void MultiGame::PaintSMapP2()
{
	CDC* pDC = GetDC();


	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			CBrush smapbrush;
			smapbrush.CreateSolidBrush(smmap2[i][j].rgb);
			pDC->SelectObject(&smapbrush);
			pDC->Rectangle(sw_space + playerplus + space * i, sh_space + space * j, sw_space + playerplus + space * (i + 1), sh_space + space * (j + 1));

		}
	}
	ReleaseDC(pDC);
}






void MultiGame::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;


	}
	else
	{
		CDialogEx::OnPaint();
		PaintMapP1();
		PaintSMapP1();
		PaintMapP2();
		PaintSMapP2();

	}
}





void Init_mblock1()
{
	srand(time(NULL));
	COLORREF temp;
	//1,4
	int x = rand() / 255;
	int y = rand() / 255;
	int z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[0].mmap1[0][1].rgb = temp;
	mblock[0].mmap1[1][1].rgb = temp;
	mblock[0].mmap1[2][1].rgb = temp;
	mblock[0].mmap1[3][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[1].mmap1[2][0].rgb = temp;
	mblock[1].mmap1[2][1].rgb = temp;
	mblock[1].mmap1[2][2].rgb = temp;
	mblock[1].mmap1[2][3].rgb = temp;


	//折线1
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[2].mmap1[0][1].rgb = temp;
	mblock[2].mmap1[1][1].rgb = temp;
	mblock[2].mmap1[1][2].rgb = temp;
	mblock[2].mmap1[2][2].rgb = temp;


	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[3].mmap1[1][1].rgb = temp;
	mblock[3].mmap1[1][2].rgb = temp;
	mblock[3].mmap1[2][0].rgb = temp;
	mblock[3].mmap1[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[4].mmap1[0][1].rgb = temp;
	mblock[4].mmap1[1][1].rgb = temp;
	mblock[4].mmap1[1][2].rgb = temp;
	mblock[4].mmap1[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[5].mmap1[1][1].rgb = temp;
	mblock[5].mmap1[1][2].rgb = temp;
	mblock[5].mmap1[2][0].rgb = temp;
	mblock[5].mmap1[2][1].rgb = temp;

	//折线2
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[6].mmap1[0][1].rgb = temp;
	mblock[6].mmap1[1][1].rgb = temp;
	mblock[6].mmap1[1][2].rgb = temp;
	mblock[6].mmap1[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[7].mmap1[2][0].rgb = temp;
	mblock[7].mmap1[2][1].rgb = temp;
	mblock[7].mmap1[1][1].rgb = temp;
	mblock[7].mmap1[1][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[8].mmap1[0][1].rgb = temp;
	mblock[8].mmap1[1][1].rgb = temp;
	mblock[8].mmap1[1][2].rgb = temp;
	mblock[8].mmap1[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[9].mmap1[2][0].rgb = temp;
	mblock[9].mmap1[2][1].rgb = temp;
	mblock[9].mmap1[1][1].rgb = temp;
	mblock[9].mmap1[1][2].rgb = temp;

	//L1
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[10].mmap1[0][1].rgb = temp;
	mblock[10].mmap1[1][1].rgb = temp;
	mblock[10].mmap1[2][1].rgb = temp;
	mblock[10].mmap1[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[11].mmap1[2][0].rgb = temp;
	mblock[11].mmap1[1][0].rgb = temp;
	mblock[11].mmap1[1][1].rgb = temp;
	mblock[11].mmap1[1][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[12].mmap1[0][1].rgb = temp;
	mblock[12].mmap1[0][2].rgb = temp;
	mblock[12].mmap1[1][2].rgb = temp;
	mblock[12].mmap1[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[13].mmap1[0][2].rgb = temp;
	mblock[13].mmap1[1][0].rgb = temp;
	mblock[13].mmap1[1][1].rgb = temp;
	mblock[13].mmap1[1][2].rgb = temp;

	//L2
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[14].mmap1[0][2].rgb = temp;
	mblock[14].mmap1[1][2].rgb = temp;
	mblock[14].mmap1[2][2].rgb = temp;
	mblock[14].mmap1[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[15].mmap1[1][0].rgb = temp;
	mblock[15].mmap1[2][0].rgb = temp;
	mblock[15].mmap1[2][1].rgb = temp;
	mblock[15].mmap1[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[16].mmap1[0][1].rgb = temp;
	mblock[16].mmap1[0][2].rgb = temp;
	mblock[16].mmap1[1][1].rgb = temp;
	mblock[16].mmap1[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[17].mmap1[1][0].rgb = temp;
	mblock[17].mmap1[1][1].rgb = temp;
	mblock[17].mmap1[1][2].rgb = temp;
	mblock[17].mmap1[2][2].rgb = temp;

	//正方形
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[18].mmap1[1][1].rgb = temp;
	mblock[18].mmap1[1][2].rgb = temp;
	mblock[18].mmap1[2][2].rgb = temp;
	mblock[18].mmap1[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[19].mmap1[1][1].rgb = temp;
	mblock[19].mmap1[1][2].rgb = temp;
	mblock[19].mmap1[2][2].rgb = temp;
	mblock[19].mmap1[2][1].rgb = temp;



	//凸起
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[20].mmap1[1][1].rgb = temp;
	mblock[20].mmap1[2][1].rgb = temp;
	mblock[20].mmap1[2][0].rgb = temp;
	mblock[20].mmap1[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[21].mmap1[0][1].rgb = temp;
	mblock[21].mmap1[1][1].rgb = temp;
	mblock[21].mmap1[2][1].rgb = temp;
	mblock[21].mmap1[1][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[22].mmap1[1][0].rgb = temp;
	mblock[22].mmap1[1][1].rgb = temp;
	mblock[22].mmap1[1][2].rgb = temp;
	mblock[22].mmap1[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[23].mmap1[0][1].rgb = temp;
	mblock[23].mmap1[1][1].rgb = temp;
	mblock[23].mmap1[2][1].rgb = temp;
	mblock[23].mmap1[1][0].rgb = temp;
}



void Init_mblock2()
{
	srand(time(NULL));
	COLORREF temp;
	//1,4
	int x = rand() / 255;
	int y = rand() / 255;
	int z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[0].mmap2[0][1].rgb = temp;
	mblock[0].mmap2[1][1].rgb = temp;
	mblock[0].mmap2[2][1].rgb = temp;
	mblock[0].mmap2[3][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[1].mmap2[2][0].rgb = temp;
	mblock[1].mmap2[2][1].rgb = temp;
	mblock[1].mmap2[2][2].rgb = temp;
	mblock[1].mmap2[2][3].rgb = temp;


	//折线1
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[2].mmap2[0][1].rgb = temp;
	mblock[2].mmap2[1][1].rgb = temp;
	mblock[2].mmap2[1][2].rgb = temp;
	mblock[2].mmap2[2][2].rgb = temp;


	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[3].mmap2[1][1].rgb = temp;
	mblock[3].mmap2[1][2].rgb = temp;
	mblock[3].mmap2[2][0].rgb = temp;
	mblock[3].mmap2[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[4].mmap2[0][1].rgb = temp;
	mblock[4].mmap2[1][1].rgb = temp;
	mblock[4].mmap2[1][2].rgb = temp;
	mblock[4].mmap2[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[5].mmap2[1][1].rgb = temp;
	mblock[5].mmap2[1][2].rgb = temp;
	mblock[5].mmap2[2][0].rgb = temp;
	mblock[5].mmap2[2][1].rgb = temp;

	//折线2
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[6].mmap2[0][1].rgb = temp;
	mblock[6].mmap2[1][1].rgb = temp;
	mblock[6].mmap2[1][2].rgb = temp;
	mblock[6].mmap2[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[7].mmap2[2][0].rgb = temp;
	mblock[7].mmap2[2][1].rgb = temp;
	mblock[7].mmap2[1][1].rgb = temp;
	mblock[7].mmap2[1][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[8].mmap2[0][1].rgb = temp;
	mblock[8].mmap2[1][1].rgb = temp;
	mblock[8].mmap2[1][2].rgb = temp;
	mblock[8].mmap2[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[9].mmap2[2][0].rgb = temp;
	mblock[9].mmap2[2][1].rgb = temp;
	mblock[9].mmap2[1][1].rgb = temp;
	mblock[9].mmap2[1][2].rgb = temp;

	//L1
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[10].mmap2[0][1].rgb = temp;
	mblock[10].mmap2[1][1].rgb = temp;
	mblock[10].mmap2[2][1].rgb = temp;
	mblock[10].mmap2[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[11].mmap2[2][0].rgb = temp;
	mblock[11].mmap2[1][0].rgb = temp;
	mblock[11].mmap2[1][1].rgb = temp;
	mblock[11].mmap2[1][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[12].mmap2[0][1].rgb = temp;
	mblock[12].mmap2[0][2].rgb = temp;
	mblock[12].mmap2[1][2].rgb = temp;
	mblock[12].mmap2[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[13].mmap2[0][2].rgb = temp;
	mblock[13].mmap2[1][0].rgb = temp;
	mblock[13].mmap2[1][1].rgb = temp;
	mblock[13].mmap2[1][2].rgb = temp;

	//L2
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[14].mmap2[0][2].rgb = temp;
	mblock[14].mmap2[1][2].rgb = temp;
	mblock[14].mmap2[2][2].rgb = temp;
	mblock[14].mmap2[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[15].mmap2[1][0].rgb = temp;
	mblock[15].mmap2[2][0].rgb = temp;
	mblock[15].mmap2[2][1].rgb = temp;
	mblock[15].mmap2[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[16].mmap2[0][1].rgb = temp;
	mblock[16].mmap2[0][2].rgb = temp;
	mblock[16].mmap2[1][1].rgb = temp;
	mblock[16].mmap2[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[17].mmap2[1][0].rgb = temp;
	mblock[17].mmap2[1][1].rgb = temp;
	mblock[17].mmap2[1][2].rgb = temp;
	mblock[17].mmap2[2][2].rgb = temp;

	//正方形
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[18].mmap2[1][1].rgb = temp;
	mblock[18].mmap2[1][2].rgb = temp;
	mblock[18].mmap2[2][2].rgb = temp;
	mblock[18].mmap2[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[19].mmap2[1][1].rgb = temp;
	mblock[19].mmap2[1][2].rgb = temp;
	mblock[19].mmap2[2][2].rgb = temp;
	mblock[19].mmap2[2][1].rgb = temp;



	//凸起
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[20].mmap2[1][1].rgb = temp;
	mblock[20].mmap2[2][1].rgb = temp;
	mblock[20].mmap2[2][0].rgb = temp;
	mblock[20].mmap2[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[21].mmap2[0][1].rgb = temp;
	mblock[21].mmap2[1][1].rgb = temp;
	mblock[21].mmap2[2][1].rgb = temp;
	mblock[21].mmap2[1][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[22].mmap2[1][0].rgb = temp;
	mblock[22].mmap2[1][1].rgb = temp;
	mblock[22].mmap2[1][2].rgb = temp;
	mblock[22].mmap2[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	mblock[23].mmap2[0][1].rgb = temp;
	mblock[23].mmap2[1][1].rgb = temp;
	mblock[23].mmap2[2][1].rgb = temp;
	mblock[23].mmap2[1][0].rgb = temp;
}



void ChangeSmap1(const MBLOCK& b)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			smmap1[i][j].rgb = b.mmap1[i][j].rgb;
		}
}




void ChangeSmap2(const MBLOCK& b)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			smmap2[i][j].rgb = b.mmap2[i][j].rgb;
		}
}


void GetBlock1()
{
	int rad = rand() % 24;
	ChangeSmap1(mblock[rad]);
}


void GetBlock2()
{
	int rad = rand() % 24;
	ChangeSmap2(mblock[rad]);
}


void GiveToControl1()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			mcontrol1[i][j].rgb = smmap1[j][i].rgb;
		}
}


void GiveToControl2()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			mcontrol2[i][j].rgb = smmap2[j][i].rgb;
		}
}

void ShowBlock1()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (mmap1[POX1 + i][POY1 + j].rgb == RGB(255, 255, 255))
				mmap1[POX1 + i][POY1 + j].rgb = mcontrol1[i][j].rgb;
		}
}


void ShowBlock2()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (mmap2[POX2 + i][POY2 + j].rgb == RGB(255, 255, 255))
				mmap2[POX2 + i][POY2 + j].rgb = mcontrol2[i][j].rgb;
		}
}


void GetNextBlock1()
{
	GetBlock1();
}


void GetNextBlock2()
{
	GetBlock2();
}


bool GetFail1()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (mcontrol1[i][j].rgb != RGB(255, 255, 255) && mmap1[i][j + 4].rgb != RGB(255, 255, 255))
				return TRUE;
		}
	return FALSE;
}


bool GetFail2()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (mcontrol2[i][j].rgb != RGB(255, 255, 255) && mmap2[i][j + 4].rgb != RGB(255, 255, 255))
				return TRUE;
		}
	return FALSE;
}


bool Check1(int x1, int y1)
{
	for (int j = 0; j < 4; ++j)
		for (int i = 0; i < 4; ++i)
		{
			if (mcontrol1[3 - i][3 - j].rgb != RGB(255, 255, 255))
			{
				if (x1 + 3 - i + 1 >= height)
					return TRUE;
				else if (mmap1[x1 + 3 - i + 1][y1 + 3 - j].rgb != RGB(255, 255, 255))
					return TRUE;
				else
					break;
			}
		}
	return FALSE;
}


bool Check2(int x1, int y1)
{
	for (int j = 0; j < 4; ++j)
		for (int i = 0; i < 4; ++i)
		{
			if (mcontrol2[3 - i][3 - j].rgb != RGB(255, 255, 255))
			{
				if (x1 + 3 - i + 1 >= height)
					return TRUE;
				else if (mmap2[x1 + 3 - i + 1][y1 + 3 - j].rgb != RGB(255, 255, 255))
					return TRUE;
				else
					break;
			}
		}
	return FALSE;
}


void Down11(int x1, int y1)
{
	for (int j = 0; j < 4; ++j)
	{
		int flag1 = 0;
		for (int i = 0; i < 4; ++i)
		{
			if (mcontrol1[i][j].rgb != RGB(255, 255, 255) && flag1 == 0)
			{
				flag1 = 1;
				mmap1[x1 + i][y1 + j].rgb = RGB(255, 255, 255);
				mmap1[x1 + i + 1][y1 + j].rgb = mcontrol1[i][j].rgb;
			}
			if (mcontrol1[i][j].rgb != RGB(255, 255, 255))
			{
				mmap1[x1 + i + 1][y1 + j].rgb = mcontrol1[i][j].rgb;
			}
		}
	}
}


void Down12(int x1, int y1)
{
	for (int j = 0; j < 4; ++j)
	{
		int flag2 = 0;
		for (int i = 0; i < 4; ++i)
		{
			if (mcontrol2[i][j].rgb != RGB(255, 255, 255) && flag2 == 0)
			{
				flag2 = 1;
				mmap2[x1 + i][y1 + j].rgb = RGB(255, 255, 255);
				mmap2[x1 + i + 1][y1 + j].rgb = mcontrol2[i][j].rgb;
			}
			if (mcontrol2[i][j].rgb != RGB(255, 255, 255))
			{
				mmap2[x1 + i + 1][y1 + j].rgb = mcontrol2[i][j].rgb;
			}
		}
	}
}


void ChangeBlock1()
{
	MMAP temp1[4][4];
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			temp1[i][j] = mcontrol1[i][j];
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mcontrol1[j][3 - i] = temp1[i][j];
		}
	}
}


void ChangeBlock2()
{
	MMAP temp2[4][4];
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			temp2[i][j] = mcontrol2[i][j];
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mcontrol2[j][3 - i] = temp2[i][j];
		}
	}
}


void MultiGame::ScoreandLevel1()
{
	CFont font;
	font.CreatePointFont(text_size, L"宋体");
	CDC* pDC = GetDC();
	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->SelectObject(&font);
	CString scoreprint;
	scoreprint.Format(_T("SCORE: %d"), score1);
	pDC->TextOutW(score_x, score_y, scoreprint);
	CString levelprint;
	levelprint.Format(_T("LEVEL: %d"), level1);
	pDC->TextOutW(level_x, level_y, levelprint);
	ReleaseDC(pDC);
}


void MultiGame::ScoreandLevel2()
{
	CFont font;
	font.CreatePointFont(text_size, L"宋体");
	CDC* pDC = GetDC();
	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->SelectObject(&font);
	CString scoreprint;
	scoreprint.Format(_T("SCORE: %d"), score2);
	pDC->TextOutW(score_x+playerplus, score_y, scoreprint);
	CString levelprint;
	levelprint.Format(_T("LEVEL: %d"), level2);
	pDC->TextOutW(level_x+playerplus, level_y, levelprint);
	ReleaseDC(pDC);
}




void SummonBlockNormalform()
{
	int a[normal_block];
	for (int i = 0; i < normal_block; ++i)
	{
		a[i] = rand() % 3 + 1;
	}
	int normalmap[normal_block][width] = {};
	int temp_map[normal_block][width];
	for (int i = 0; i < normal_block; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			temp_map[i][j] = j + 1;
		}
	}

	for (int i = 0; i < normal_block; ++i)
	{
		for (int j = 0; j < a[i]; ++j)
		{
			int k = rand() % (10 - j);
			normalmap[i][temp_map[i][k] - 1] = temp_map[i][k];
			for (int m = k; m < width - j - 1; ++m)
			{
				temp_map[i][m] = temp_map[i][m + 1];
			}
		}
	}

	for (int i = 0; i < normal_block; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (normalmap[i][j] == 0)
			{
				mmap1[height - i - 1][j].rgb = RGB(rand() % 255, rand() % 255, rand() % 255);
				mmap2[height - i - 1][j].rgb = RGB(rand() % 255, rand() % 255, rand() % 255);
			}
		}
	}

}




void SummonBlockDifficultform()
{
	int a[difficult_block];
	for (int i = 0; i < difficult_block; ++i)
	{
		a[i] = rand() % 3 + 1;
	}
	int difficultmap[difficult_block][width] = {};
	int temp_map[difficult_block][width];
	for (int i = 0; i < difficult_block; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			temp_map[i][j] = j + 1;
		}
	}

	for (int i = 0; i < difficult_block; ++i)
	{
		for (int j = 0; j < a[i]; ++j)
		{
			int k = rand() % (10 - j);
			difficultmap[i][temp_map[i][k] - 1] = temp_map[i][k];
			for (int m = k; m < width - j - 1; ++m)
			{
				temp_map[i][m] = temp_map[i][m + 1];
			}
		}
	}

	for (int i = 0; i < difficult_block; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (difficultmap[i][j] == 0)
			{
				mmap1[height - i - 1][j].rgb = RGB(rand() % 255, rand() % 255, rand() % 255);
				mmap2[height - i - 1][j].rgb = RGB(rand() % 255, rand() % 255, rand() % 255);
			}
		}
	}

}




void MultiGame::LevelSpeed()
{
	if (level1 > level2)
	{
		maxlevel = level1;
	}
	else maxlevel = level2;


	if (speed > 100)
	{
		if ((maxlevel - 1) % 2 == 0)
		{
			speed = init_speed - (maxlevel - 1) * 50;
		}
	}
}





void MultiGame::whowin()
{
	if (score1 >= winscore && score2 < winscore)
	{
		CString str1;
		str1.Format(L"Player 1 is Winner!\nCongratulations!\nScore: %d : %d ", score1, score2);
		MessageBox(str1);
		KillTimer(1);
	}
	else if (score2 >= winscore && score1 < winscore)
	{
		CString str2;
		str2.Format(L"Player 2 is Winner!\nCongratulations!\nScore: %d : %d ", score1, score2);
		MessageBox(str2);
		KillTimer(1);
	}
	else if (score1 >= winscore && score2 >= winscore)
	{
		MessageBox(L"Congratulations!\nBoth of You are Winners");
		KillTimer(1);
	}

}



void MultiGame::Clear1()
{
	for (int i = 0; i < height; ++i)
	{
		int j;
		for (j = 0; j < width; ++j)
		{
			if (mmap1[i][j].rgb == RGB(255, 255, 255))
				break;
		}
		if (j == width)
		{
			for (int k = i; k >= 0; --k)
			{
				for (j = 0; j < width; ++j)
				{
					if (k == 0)
					{
						mmap1[k][j].rgb = RGB(255, 255, 255);
					}
					else
					{
						mmap1[k][j].rgb = mmap1[k - 1][j].rgb;
					}
				}
			}
			score1 = score1 + 10;
			if (score1 % 100 == 0)
			{
				level1 = score1 / 100 + 1;
			}
			ScoreandLevel1();
			if (tempdifficulty == 2 && level1 % 6 == 0)
			{
				for (int i = 0; i < height; ++i)
				{
					for (int j = 0; j < width; ++j)
					{
						mmap1[i][j].rgb = RGB(255, 255, 255);
					}
				}
				SummonBlockNormalform();
			}
			if (tempdifficulty == 3 && level1 % 6 == 0)
			{
				for (int i = 0; i < height; ++i)
				{
					for (int j = 0; j < width; ++j)
					{
						mmap1[i][j].rgb = RGB(255, 255, 255);
					}
				}
				SummonBlockNormalform();
			}
			if (tempdifficulty == 4)
			{
				LevelSpeed();
				SetTimer(1, speed, NULL);
			}
			whowin();

		}
	}
}


void MultiGame::Clear2()
{
	for (int i = 0; i < height; ++i)
	{
		int j;
		for (j = 0; j < width; ++j)
		{
			if (mmap2[i][j].rgb == RGB(255, 255, 255))
				break;
		}
		if (j == width)
		{
			for (int k = i; k >= 0; --k)
			{
				for (j = 0; j < width; ++j)
				{
					if (k == 0)
					{
						mmap2[k][j].rgb = RGB(255, 255, 255);
					}
					else
					{
						mmap2[k][j].rgb = mmap2[k - 1][j].rgb;
					}
				}
			}
			score2 = score2 + 10;
			if (score2 % 100 == 0)
			{
				level2 = score2 / 100 + 1;
			}
			
			ScoreandLevel2();
			if (tempdifficulty == 2 && level2 % 6 == 0)
			{
				for (int i = 0; i < height; ++i)
				{
					for (int j = 0; j < width; ++j)
					{
						mmap2[i][j].rgb = RGB(255, 255, 255);
					}
				}
				SummonBlockNormalform();
			}
			if (tempdifficulty == 3  && level2 % 6 == 0)
			{
				for (int i = 0; i < height; ++i)
				{
					for (int j = 0; j < width; ++j)
					{
						mmap2[i][j].rgb = RGB(255, 255, 255);
					}
				}
				SummonBlockNormalform();
			}
			whowin();

		}
	}
}




BOOL MultiGame::PreTranslateMessage(MSG* pMsg)
{
	if (tempstop != stop)
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			switch (pMsg->wParam)
			{
			case 'W':
				for (int i = 0; i < 4; ++i)
					for (int j = 0; j < 4; ++j)
					{
						if (mcontrol1[i][j].rgb != RGB(255, 255, 255))
						{
							mmap1[POX1 + i][POY1 + j].rgb = RGB(255, 255, 255);
						}
					}

				ChangeBlock1();
				for (int i = 0; i < 4; ++i)
				{
					ifcan[i] = false;
				}
				k = 0;

				if (POY1 == -1)
				{
					if (mcontrol1[0][0].rgb != RGB(255, 255, 255) || mcontrol1[1][0].rgb != RGB(255, 255, 255) || mcontrol1[2][0].rgb != RGB(255, 255, 255) || mcontrol1[3][0].rgb != RGB(255, 255, 255))
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol1[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap1[POX1 + i][POY1 + j + 1].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							POY1++;
							ShowBlock1();
						}
						else
						{
							ChangeBlock1();
							ChangeBlock1();
							ChangeBlock1();
							ShowBlock1();
						}

					}
					else
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol1[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap1[POX1 + i][POY1 + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							ShowBlock1();
						}
						else
						{
							ChangeBlock1();
							ChangeBlock1();
							ChangeBlock1();
							ShowBlock1();
						}
					}
				}


				else if (POY1 == -2)
				{

					if (mcontrol1[0][1].rgb != RGB(255, 255, 255) || mcontrol1[1][1].rgb != RGB(255, 255, 255) || mcontrol1[2][1].rgb != RGB(255, 255, 255) || mcontrol1[3][1].rgb != RGB(255, 255, 255))
					{
						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol1[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap1[POX1 + i][POY1 + j + 2].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							POY1 = POY1 + 2;
							ShowBlock1();
						}
						else
						{
							ChangeBlock1();
							ChangeBlock1();
							ChangeBlock1();
							ShowBlock1();
						}

					}
					else
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol1[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap1[POX1 + i][POY1 + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							ShowBlock1();
						}
						else
						{
							ChangeBlock1();
							ChangeBlock1();
							ChangeBlock1();
							ShowBlock1();
						}
					}
				}


				else if (POY1 == 7)
				{

					if (mcontrol1[0][3].rgb != RGB(255, 255, 255) || mcontrol1[1][3].rgb != RGB(255, 255, 255) || mcontrol1[2][3].rgb != RGB(255, 255, 255) || mcontrol1[3][3].rgb != RGB(255, 255, 255))
					{
						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol1[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap1[POX1 + i][POY1 + j - 1].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							POY1 = POY1 - 1;
							ShowBlock1();
						}
						else
						{
							ChangeBlock1();
							ChangeBlock1();
							ChangeBlock1();
							ShowBlock1();
						}
					}
					else
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol1[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap1[POX1 + i][POY1 + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							ShowBlock1();
						}
						else
						{
							ChangeBlock1();
							ChangeBlock1();
							ChangeBlock1();
							ShowBlock1();
						}
					}
				}


				else if (POY1 == 8)
				{

					if (mcontrol1[0][3].rgb != RGB(255, 255, 255) || mcontrol1[1][3].rgb != RGB(255, 255, 255) || mcontrol1[2][3].rgb != RGB(255, 255, 255) || mcontrol1[3][3].rgb != RGB(255, 255, 255))
					{
						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol1[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap1[POX1 + i][POY1 + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							POY1 = POY1 - 2;
							ShowBlock1();
						}
						else
						{
							ChangeBlock1();
							ChangeBlock1();
							ChangeBlock1();
							ShowBlock1();
						}
					}
					else
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol1[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap1[POX1 + i][POY1 + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							ShowBlock1();
						}
						else
						{
							ChangeBlock1();
							ChangeBlock1();
							ChangeBlock1();
							ShowBlock1();
						}
					}
				}


				else
				{

					for (int i = 0; i < 4; ++i)
						for (int j = 0; j < 4; ++j)
						{
							if (mcontrol1[i][j].rgb != RGB(255, 255, 255))
							{
								if (mmap1[POX1 + i][POY1 + j].rgb == RGB(255, 255, 255))
								{
									ifcan[k] = true;
									k++;
								}
							}
						}
					if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
					{
						ShowBlock1();
					}
					else
					{
						ChangeBlock1();
						ChangeBlock1();
						ChangeBlock1();
						ShowBlock1();
					}
				}


				PaintMapP1();
				break;
			case VK_UP:
				for (int i = 0; i < 4; ++i)
					for (int j = 0; j < 4; ++j)
					{
						if (mcontrol2[i][j].rgb != RGB(255, 255, 255))
						{
							mmap2[POX2 + i][POY2 + j].rgb = RGB(255, 255, 255);
						}
					}

				ChangeBlock2();
				for (int i = 0; i < 4; ++i)
				{
					ifcan[i] = false;
				}
				k = 0;

				if (POY2 == -1)
				{
					if (mcontrol2[0][0].rgb != RGB(255, 255, 255) || mcontrol2[1][0].rgb != RGB(255, 255, 255) || mcontrol2[2][0].rgb != RGB(255, 255, 255) || mcontrol2[3][0].rgb != RGB(255, 255, 255))
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol2[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap2[POX2 + i][POY2 + j + 1].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							POY2++;
							ShowBlock2();
						}
						else
						{
							ChangeBlock2();
							ChangeBlock2();
							ChangeBlock2();
							ShowBlock2();
						}

					}
					else
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol2[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap2[POX2 + i][POY2 + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							ShowBlock2();
						}
						else
						{
							ChangeBlock2();
							ChangeBlock2();
							ChangeBlock2();
							ShowBlock2();
						}
					}
				}


				else if (POY2 == -2)
				{

					if (mcontrol2[0][1].rgb != RGB(255, 255, 255) || mcontrol2[1][1].rgb != RGB(255, 255, 255) || mcontrol2[2][1].rgb != RGB(255, 255, 255) || mcontrol2[3][1].rgb != RGB(255, 255, 255))
					{
						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol2[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap2[POX2 + i][POY2 + j + 2].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							POY2 = POY2 + 2;
							ShowBlock2();
						}
						else
						{
							ChangeBlock2();
							ChangeBlock2();
							ChangeBlock2();
							ShowBlock2();
						}

					}
					else
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol2[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap2[POX2 + i][POY2 + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							ShowBlock2();
						}
						else
						{
							ChangeBlock2();
							ChangeBlock2();
							ChangeBlock2();
							ShowBlock2();
						}
					}
				}


				else if (POY2 == 7)
				{

					if (mcontrol2[0][3].rgb != RGB(255, 255, 255) || mcontrol2[1][3].rgb != RGB(255, 255, 255) || mcontrol2[2][3].rgb != RGB(255, 255, 255) || mcontrol2[3][3].rgb != RGB(255, 255, 255))
					{
						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol2[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap2[POX2 + i][POY2 + j - 1].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							POY2 = POY2 - 1;
							ShowBlock2();
						}
						else
						{
							ChangeBlock2();
							ChangeBlock2();
							ChangeBlock2();
							ShowBlock2();
						}
					}
					else
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol2[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap2[POX2 + i][POY2 + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							ShowBlock2();
						}
						else
						{
							ChangeBlock2();
							ChangeBlock2();
							ChangeBlock2();
							ShowBlock2();
						}
					}
				}


				else if (POY2 == 8)
				{

					if (mcontrol2[0][3].rgb != RGB(255, 255, 255) || mcontrol2[1][3].rgb != RGB(255, 255, 255) || mcontrol2[2][3].rgb != RGB(255, 255, 255) || mcontrol2[3][3].rgb != RGB(255, 255, 255))
					{
						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol2[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap2[POX2 + i][POY2 + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							POY2 = POY2 - 2;
							ShowBlock2();
						}
						else
						{
							ChangeBlock2();
							ChangeBlock2();
							ChangeBlock2();
							ShowBlock2();
						}
					}
					else
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol2[i][j].rgb != RGB(255, 255, 255))
								{
									if (mmap2[POX2 + i][POY2 + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							ShowBlock2();
						}
						else
						{
							ChangeBlock2();
							ChangeBlock2();
							ChangeBlock2();
							ShowBlock2();
						}
					}
				}


				else
				{

					for (int i = 0; i < 4; ++i)
						for (int j = 0; j < 4; ++j)
						{
							if (mcontrol2[i][j].rgb != RGB(255, 255, 255))
							{
								if (mmap2[POX2 + i][POY2 + j].rgb == RGB(255, 255, 255))
								{
									ifcan[k] = true;
									k++;
								}
							}
						}
					if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
					{
						ShowBlock2();
					}
					else
					{
						ChangeBlock2();
						ChangeBlock2();
						ChangeBlock2();
						ShowBlock2();
					}
				}


				PaintMapP2();
				break;
			case 'A':

			{

				if ((POY1 >= 1) || (POY1 == 0 && mcontrol1[0][0].rgb == RGB(255, 255, 255) && mcontrol1[1][0].rgb == RGB(255, 255, 255) && mcontrol1[2][0].rgb == RGB(255, 255, 255) && mcontrol1[3][0].rgb == RGB(255, 255, 255)) || (POY1 == -1 && mcontrol1[0][1].rgb == RGB(255, 255, 255) && mcontrol1[1][1].rgb == RGB(255, 255, 255) && mcontrol1[2][1].rgb == RGB(255, 255, 255) && mcontrol1[3][1].rgb == RGB(255, 255, 255)))
				{
					int edge1[4] = { 100,100,100,100 };
					for (int i = 0; i < 4; ++i)
					{
						for (int j = 3; j > -1; --j)
						{
							if (mcontrol1[i][j].rgb != RGB(255, 255, 255))
								edge1[i] = j;
						}
					}

					if ((edge1[0] == 100 && mmap1[POX1 + 1][POY1 + edge1[1] - 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 2][POY1 + edge1[2] - 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 3][POY1 + edge1[3] - 1].rgb == RGB(255, 255, 255))
						|| (edge1[3] == 100 && mmap1[POX1 + 1][POY1 + edge1[1] - 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 2][POY1 + edge1[2] - 1].rgb == RGB(255, 255, 255) && mmap1[POX1][POY1 + edge1[0] - 1].rgb == RGB(255, 255, 255))
						|| (edge1[0] == 100 && edge1[1] == 100 && mmap1[POX1 + 2][POY1 + edge1[2] - 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 3][POY1 + edge1[3] - 1].rgb == RGB(255, 255, 255))
						|| (edge1[0] == 100 && edge1[3] == 100 && mmap1[POX1 + 1][POY1 + edge1[1] - 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 2][POY1 + edge1[2] - 1].rgb == RGB(255, 255, 255))
						|| (edge1[2] == 100 && edge1[3] == 100 && mmap1[POX1][POY1 + edge1[0] - 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 1][POY1 + edge1[1] - 1].rgb == RGB(255, 255, 255))
						|| (edge1[0] == 100 && edge1[1] == 100 && edge1[2] == 100 && mmap1[POX1 + 3][POY1 + edge1[3] - 1].rgb == RGB(255, 255, 255))
						|| (edge1[0] == 100 && edge1[1] == 100 && edge1[3] == 100 && mmap1[POX1 + 2][POY1 + edge1[2] - 1].rgb == RGB(255, 255, 255))
						|| (edge1[3] == 100 && edge1[0] == 100 && edge1[2] == 100 && mmap1[POX1 + 1][POY1 + edge1[1] - 1].rgb == RGB(255, 255, 255))
						|| (mmap1[POX1][POY1 + edge1[0] - 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 1][POY1 + edge1[1] - 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 2][POY1 + edge1[2] - 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 3][POY1 + edge1[3] - 1].rgb == RGB(255, 255, 255))
						)
					{
						for (int i = 0; i < 4; ++i)
						{
							int flag = 0;
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol1[i][3 - j].rgb != RGB(255, 255, 255) && flag == 0)
								{
									flag = 1;
									mmap1[POX1 + i][POY1 + 3 - j].rgb = RGB(255, 255, 255);
									mmap1[POX1 + i][POY1 + 3 - j - 1].rgb = mcontrol1[i][3 - j].rgb;
								}
								if (mcontrol1[i][3 - j].rgb != RGB(255, 255, 255))
								{
									mmap1[POX1 + i][POY1 + 3 - j - 1].rgb = mcontrol1[i][3 - j].rgb;
								}
							}
						}
						PaintMapP1();
						--POY1;
					}


				}


			}
			break;
			case VK_LEFT:

			{

				if ((POY2 >= 1) || (POY2 == 0 && mcontrol2[0][0].rgb == RGB(255, 255, 255) && mcontrol2[1][0].rgb == RGB(255, 255, 255) && mcontrol2[2][0].rgb == RGB(255, 255, 255) && mcontrol2[3][0].rgb == RGB(255, 255, 255)) || (POY2 == -1 && mcontrol2[0][1].rgb == RGB(255, 255, 255) && mcontrol2[1][1].rgb == RGB(255, 255, 255) && mcontrol2[2][1].rgb == RGB(255, 255, 255) && mcontrol2[3][1].rgb == RGB(255, 255, 255)))
				{
					int edge2[4] = { 100,100,100,100 };
					for (int i = 0; i < 4; ++i)
					{
						for (int j = 3; j > -1; --j)
						{
							if (mcontrol2[i][j].rgb != RGB(255, 255, 255))
								edge2[i] = j;
						}
					}

					if ((edge2[0] == 100 && mmap2[POX2 + 1][POY2 + edge2[1] - 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 2][POY2 + edge2[2] - 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 3][POY2 + edge2[3] - 1].rgb == RGB(255, 255, 255))
						|| (edge2[3] == 100 && mmap2[POX2 + 1][POY2 + edge2[1] - 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 2][POY2 + edge2[2] - 1].rgb == RGB(255, 255, 255) && mmap2[POX2][POY2 + edge2[0] - 1].rgb == RGB(255, 255, 255))
						|| (edge2[0] == 100 && edge2[1] == 100 && mmap2[POX2 + 2][POY2 + edge2[2] - 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 3][POY2 + edge2[3] - 1].rgb == RGB(255, 255, 255))
						|| (edge2[0] == 100 && edge2[3] == 100 && mmap2[POX2 + 1][POY2 + edge2[1] - 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 2][POY2 + edge2[2] - 1].rgb == RGB(255, 255, 255))
						|| (edge2[2] == 100 && edge2[3] == 100 && mmap2[POX2][POY2 + edge2[0] - 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 1][POY2 + edge2[1] - 1].rgb == RGB(255, 255, 255))
						|| (edge2[0] == 100 && edge2[1] == 100 && edge2[2] == 100 && mmap2[POX2 + 3][POY2 + edge2[3] - 1].rgb == RGB(255, 255, 255))
						|| (edge2[0] == 100 && edge2[1] == 100 && edge2[3] == 100 && mmap2[POX2 + 2][POY2 + edge2[2] - 1].rgb == RGB(255, 255, 255))
						|| (edge2[3] == 100 && edge2[0] == 100 && edge2[2] == 100 && mmap2[POX2 + 1][POY2 + edge2[1] - 1].rgb == RGB(255, 255, 255))
						|| (mmap2[POX2][POY2 + edge2[0] - 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 1][POY2 + edge2[1] - 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 2][POY2 + edge2[2] - 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 3][POY2 + edge2[3] - 1].rgb == RGB(255, 255, 255))
						)
					{
						for (int i = 0; i < 4; ++i)
						{
							int flag = 0;
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol2[i][3 - j].rgb != RGB(255, 255, 255) && flag == 0)
								{
									flag = 1;
									mmap2[POX2 + i][POY2 + 3 - j].rgb = RGB(255, 255, 255);
									mmap2[POX2 + i][POY2 + 3 - j - 1].rgb = mcontrol2[i][3 - j].rgb;
								}
								if (mcontrol2[i][3 - j].rgb != RGB(255, 255, 255))
								{
									mmap2[POX2 + i][POY2 + 3 - j - 1].rgb = mcontrol2[i][3 - j].rgb;
								}
							}
						}
						PaintMapP2();
						--POY2;
					}


				}


			}
			break;
			case 'D':

			{

				if ((POY1 <= 5) || (POY1 == 6 && mcontrol1[0][3].rgb == RGB(255, 255, 255) && mcontrol1[1][3].rgb == RGB(255, 255, 255) && mcontrol1[2][3].rgb == RGB(255, 255, 255) && mcontrol1[3][3].rgb == RGB(255, 255, 255)) || (POY1 == 7 && mcontrol1[0][2].rgb == RGB(255, 255, 255) && mcontrol1[1][2].rgb == RGB(255, 255, 255) && mcontrol1[2][2].rgb == RGB(255, 255, 255) && mcontrol1[3][2].rgb == RGB(255, 255, 255)))
				{
					int edge1[4] = { 100,100,100,100 };
					for (int i = 0; i < 4; ++i)
					{
						for (int j = 0; j < 4; ++j)
						{
							if (mcontrol1[i][j].rgb != RGB(255, 255, 255))
								edge1[i] = j;
						}
					}

					if ((edge1[0] == 100 && mmap1[POX1 + 1][POY1 + edge1[1] + 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 2][POY1 + edge1[2] + 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 3][POY1 + edge1[3] + 1].rgb == RGB(255, 255, 255))
						|| (edge1[3] == 100 && mmap1[POX1 + 1][POY1 + edge1[1] + 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 2][POY1 + edge1[2] + 1].rgb == RGB(255, 255, 255) && mmap1[POX1][POY1 + edge1[0] + 1].rgb == RGB(255, 255, 255))
						|| (edge1[0] == 100 && edge1[1] == 100 && mmap1[POX1 + 2][POY1 + edge1[2] + 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 3][POY1 + edge1[3] + 1].rgb == RGB(255, 255, 255))
						|| (edge1[0] == 100 && edge1[3] == 100 && mmap1[POX1 + 1][POY1 + edge1[1] + 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 2][POY1 + edge1[2] + 1].rgb == RGB(255, 255, 255))
						|| (edge1[2] == 100 && edge1[3] == 100 && mmap1[POX1][POY1 + edge1[0] + 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 1][POY1 + edge1[1] + 1].rgb == RGB(255, 255, 255))
						|| (edge1[0] == 100 && edge1[1] == 100 && edge1[2] == 100 && mmap1[POX1 + 3][POY1 + edge1[3] + 1].rgb == RGB(255, 255, 255))
						|| (edge1[3] == 100 && edge1[1] == 100 && edge1[2] == 100 && mmap1[POX1][POY1 + edge1[0] + 1].rgb == RGB(255, 255, 255))
						|| (edge1[0] == 100 && edge1[1] == 100 && edge1[3] == 100 && mmap1[POX1 + 2][POY1 + edge1[2] + 1].rgb == RGB(255, 255, 255))
						|| (edge1[3] == 100 && edge1[0] == 100 && edge1[2] == 100 && mmap1[POX1 + 1][POY1 + edge1[1] + 1].rgb == RGB(255, 255, 255))
						|| (mmap1[POX1][POY1 + edge1[0] + 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 1][POY1 + edge1[1] + 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 2][POY1 + edge1[2] + 1].rgb == RGB(255, 255, 255) && mmap1[POX1 + 3][POY1 + edge1[3] + 1].rgb == RGB(255, 255, 255))
						)
					{
						for (int i = 0; i < 4; ++i)
						{
							int flag = 0;
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol1[i][j].rgb != RGB(255, 255, 255) && flag == 0)
								{
									flag = 1;
									mmap1[POX1 + i][POY1 + j].rgb = RGB(255, 255, 255);
									mmap1[POX1 + i][POY1 + j + 1].rgb = mcontrol1[i][j].rgb;
								}
								if (mcontrol1[i][j].rgb != RGB(255, 255, 255))
								{
									mmap1[POX1 + i][POY1 + j + 1].rgb = mcontrol1[i][j].rgb;
								}
							}
						}
						PaintMapP1();
						++POY1;
					}
				}


			}
			break;
			case VK_RIGHT:

			{

				if ((POY2 <= 5) || (POY2 == 6 && mcontrol2[0][3].rgb == RGB(255, 255, 255) && mcontrol2[1][3].rgb == RGB(255, 255, 255) && mcontrol2[2][3].rgb == RGB(255, 255, 255) && mcontrol2[3][3].rgb == RGB(255, 255, 255)) || (POY2 == 7 && mcontrol2[0][2].rgb == RGB(255, 255, 255) && mcontrol2[1][2].rgb == RGB(255, 255, 255) && mcontrol2[2][2].rgb == RGB(255, 255, 255) && mcontrol2[3][2].rgb == RGB(255, 255, 255)))
				{
					int edge2[4] = { 100,100,100,100 };
					for (int i = 0; i < 4; ++i)
					{
						for (int j = 0; j < 4; ++j)
						{
							if (mcontrol2[i][j].rgb != RGB(255, 255, 255))
								edge2[i] = j;
						}
					}

					if ((edge2[0] == 100 && mmap2[POX2 + 1][POY2 + edge2[1] + 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 2][POY2 + edge2[2] + 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 3][POY2 + edge2[3] + 1].rgb == RGB(255, 255, 255))
						|| (edge2[3] == 100 && mmap2[POX2 + 1][POY2 + edge2[1] + 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 2][POY2 + edge2[2] + 1].rgb == RGB(255, 255, 255) && mmap2[POX2][POY2 + edge2[0] + 1].rgb == RGB(255, 255, 255))
						|| (edge2[0] == 100 && edge2[1] == 100 && mmap2[POX2 + 2][POY2 + edge2[2] + 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 3][POY2 + edge2[3] + 1].rgb == RGB(255, 255, 255))
						|| (edge2[0] == 100 && edge2[3] == 100 && mmap2[POX2 + 1][POY2 + edge2[1] + 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 2][POY2 + edge2[2] + 1].rgb == RGB(255, 255, 255))
						|| (edge2[2] == 100 && edge2[3] == 100 && mmap2[POX2][POY2 + edge2[0] + 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 1][POY2 + edge2[1] + 1].rgb == RGB(255, 255, 255))
						|| (edge2[0] == 100 && edge2[1] == 100 && edge2[2] == 100 && mmap2[POX2 + 3][POY2 + edge2[3] + 1].rgb == RGB(255, 255, 255))
						|| (edge2[3] == 100 && edge2[1] == 100 && edge2[2] == 100 && mmap2[POX2][POY2 + edge2[0] + 1].rgb == RGB(255, 255, 255))
						|| (edge2[0] == 100 && edge2[1] == 100 && edge2[3] == 100 && mmap2[POX2 + 2][POY2 + edge2[2] + 1].rgb == RGB(255, 255, 255))
						|| (edge2[3] == 100 && edge2[0] == 100 && edge2[2] == 100 && mmap2[POX2 + 1][POY2 + edge2[1] + 1].rgb == RGB(255, 255, 255))
						|| (mmap2[POX2][POY2 + edge2[0] + 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 1][POY2 + edge2[1] + 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 2][POY2 + edge2[2] + 1].rgb == RGB(255, 255, 255) && mmap2[POX2 + 3][POY2 + edge2[3] + 1].rgb == RGB(255, 255, 255))
						)
					{
						for (int i = 0; i < 4; ++i)
						{
							int flag = 0;
							for (int j = 0; j < 4; ++j)
							{
								if (mcontrol2[i][j].rgb != RGB(255, 255, 255) && flag == 0)
								{
									flag = 1;
									mmap2[POX2 + i][POY2 + j].rgb = RGB(255, 255, 255);
									mmap2[POX2 + i][POY2 + j + 1].rgb = mcontrol2[i][j].rgb;
								}
								if (mcontrol2[i][j].rgb != RGB(255, 255, 255))
								{
									mmap2[POX2 + i][POY2 + j + 1].rgb = mcontrol2[i][j].rgb;
								}
							}
						}
						PaintMapP2();
						++POY2;
					}
				}


			}
			break;
			case 'S':
				if (Check1(POX1, POY1))
				{
					Clear1();
					PaintMapP1();
					break;
				}
				Down11(POX1, POY1);
				++POX1;
				PaintMapP1();
				break;

			case VK_DOWN:
				if (Check2(POX2, POY2))
				{
					Clear2();
					PaintMapP2();
					break;
				}
				Down12(POX2, POY2);
				++POX2;
				PaintMapP2();
				break;
			}
		}
	}
		
	if (pMsg->message == WM_MYMSG)
	{
		if (Check1(POX1, POY1))
		{
			Clear1();
			PaintMapP1();
			POX1 = 0;
			POY1 = 3;
			GiveToControl1();
			GetBlock1();
			
			if (GetFail1())
			{
				CString failmessage;
				failmessage.Format(_T("Player 2 is Winner!\nCongratulations!\nScore: %d : %d "), score1,score2);
				MessageBox(failmessage, _T("GAME OVER!"));
				KillTimer(1);
			}
			ShowBlock1();
			PaintMapP1();
			GetNextBlock1();
			PaintSMapP1();
		}
		if (Check2(POX2, POY2))
		{
			Clear2();
			PaintMapP2();
			POX2 = 0;
			POY2 = 3;
			GiveToControl2();
			GetBlock2();

			if (GetFail2())
			{
				CString failmessage;
				failmessage.Format(_T("Player 1 is Winner!\nCongratulations!\nScore: %d : %d "), score1, score2);
				MessageBox(failmessage, _T("GAME OVER!"));
				KillTimer(1);
			}
			ShowBlock2();
			PaintMapP2();
			GetNextBlock2();
			PaintSMapP2();
		}
		Down11(POX1, POY1);
		Down12(POX2, POY2);
		++POX1;
		++POX2;
		PaintMapP1();
		PaintMapP2();
	}

	return FALSE;
}









void MultiGame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		PostMessage(WM_MYMSG);//不可用SendMessage
	}
}



void MultiGame::OnBnClickedButtonMstart()
{
	// TODO: 在此添加控件通知处理程序代码
	POX1 = 0;
	POY1 = 3;
	POX2 = 0;
	POY2 = 3;
	tempdifficulty = difficulty;
	tempstop = 0;
	if (difficulty == 0)
	{
		MessageBox(L"请选择游戏难度后再进行游戏！", L"廖老师NB！");
	}
	else if (difficulty == 1)
	{
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				mmap1[i][j].rgb = RGB(255, 255, 255);
				mmap2[i][j].rgb = RGB(255, 255, 255);
			}
		}
		

		score1 = 0;
		level1 = 1;
		ScoreandLevel1();
		score2 = 0;
		level2 = 1;
		ScoreandLevel2();
		speed = easy_speed;
		PaintMapP1();
		PaintMapP2();
		Init_mblock1();
		Init_mblock2();
		GetBlock1();
		GetBlock2();
		GiveToControl1();
		GiveToControl2();
		ShowBlock1();
		ShowBlock2();
		PaintMapP1();
		PaintMapP2();
		GetNextBlock1();
		GetNextBlock2();
		PaintSMapP1();
		PaintSMapP2();
		UpdateData();
		SetTimer(1, speed, NULL);
	}

	else if (difficulty == 2)
	{
	
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			mmap1[i][j].rgb = RGB(255, 255, 255);
			mmap2[i][j].rgb = RGB(255, 255, 255);
		}
	}

	score1 = 0;
	score2 = 0;
	level1 = 1;
	level2 = 1;
	speed = normal_speed;
	ScoreandLevel1();
	ScoreandLevel2();
	SummonBlockNormalform();
	PaintMapP1();
	PaintMapP2();
	Init_mblock1();
	Init_mblock2();
	GetBlock1();
	GetBlock2();
	GiveToControl1();
	GiveToControl2();
	ShowBlock1();
	ShowBlock2();
	PaintMapP1();
	PaintMapP2();
	GetNextBlock1();
	GetNextBlock2();
	PaintSMapP1();
	PaintSMapP2();
	UpdateData();
	SetTimer(1, speed, NULL);
	}

	else if (difficulty == 3)
	{
	
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			mmap1[i][j].rgb = RGB(255, 255, 255);
			mmap2[i][j].rgb = RGB(255, 255, 255);
		}
	}
	score1 = 0;
	score2 = 0;
	level1 = 1;
	level2 = 1;
	speed = difficult_speed;
	ScoreandLevel1();
	ScoreandLevel2();
	SummonBlockDifficultform();
	PaintMapP1();
	PaintMapP2();
	Init_mblock1();
	Init_mblock2();
	GetBlock1();
	GetBlock2();
	GiveToControl1();
	GiveToControl2();
	ShowBlock1();
	ShowBlock2();
	PaintMapP1();
	PaintMapP2();
	GetNextBlock1();
	GetNextBlock2();
	PaintSMapP1();
	PaintSMapP2();
	UpdateData();
	SetTimer(1, speed, NULL);
	}

	else if (difficulty == 4)
	{
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				mmap1[i][j].rgb = RGB(255, 255, 255);
				mmap2[i][j].rgb = RGB(255, 255, 255);
			}
		}

		speed = init_speed;
		score1 = 0;
		level1 = 1;
		ScoreandLevel1();
		score2 = 0;
		level2 = 1;
		ScoreandLevel2();
		PaintMapP1();
		PaintMapP2();
		Init_mblock1();
		Init_mblock2();
		GetBlock1();
		GetBlock2();
		GiveToControl1();
		GiveToControl2();
		ShowBlock1();
		ShowBlock2();
		PaintMapP1();
		PaintMapP2();
		GetNextBlock1();
		GetNextBlock2();
		PaintSMapP1();
		PaintSMapP2();
		UpdateData();
		SetTimer(1, speed, NULL);
	}

	else MessageBox(L"？？？你有问题？？？\n你™偷改我的数据！！！",L"NM$L");

}


void MultiGame::OnBnClickedButtonMpause()
{
	// TODO: 在此添加控件通知处理程序代码
	static bool pausem = true;
	if (pausem)
	{
		tempstop = stop;
		SetTimer(1, stop, NULL);
		pausem = false;
		MessageBox(_T("游戏暂停！"), L"暂停");
	}
	else
	{
		tempstop = 0;
		SetTimer(1, speed, NULL);
		pausem = true;
		MessageBox(_T("游戏恢复！"), L"恢复");
	}
}


void MultiGame::OnBnClickedButtonMrules()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("RULE 1：方向键左右或键盘AD控制方块的方向\nRULE 2：方向键上或键盘W使方块顺时针旋转\nRULE 3：方向键下或键盘S加速方块的下落\nRULR 4：方块堆满一行自动消除并记10分\nRULE 5：每100分会使LEVEL+1\nRULE 6：方块达到顶端则游戏结束\nRULE 7：若玩急眼导致损坏电脑，本人概不负责！\n\n\nEASY模式：地图初始无方块，且速度固定\nNORMAL模式：地图随机生成3层无序方块，速度略快于EASY，每6个level便会重置地图\nDIFFICULT模式：地图随机生成6格无序方块，且速度略快于NORMAL，每6个level便会重置地图\nCHALLANGE模式：无初始方块，但速度会随着level增加而增加，游戏整体速度取决于速度最快的玩家\n\n\n多人模式额外说明：\n在任何模式下，先得到2000分的玩家获胜\n如果有任何一名玩家GAMEOVER，则另一玩家直接获胜\n鉴于游戏作者的水平原因，长按操控按键会使另一玩家无法操作\n因此，请公平对决，不要故意长按按键\n\n\nGOOD LUCK AND HAVE FUN!"), L"廖老师NB！");
}


void MultiGame::OnBnClickedButtonMquit()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			mmap1[i][j].rgb = RGB(255, 255, 255);
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			smmap1[i][j].rgb = RGB(255, 255, 255);
		}
	}
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			mmap2[i][j].rgb = RGB(255, 255, 255);
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			smmap2[i][j].rgb = RGB(255, 255, 255);
		}
	}
	PaintMapP1();
	PaintMapP2();
	PaintSMapP1();
	PaintSMapP2();
	difficulty = 0;
	tempdifficulty = 0;
	tempstop = 0;
	OnOK();
}


void MultiGame::OnBnClickedButtonMeasy()
{
	// TODO: 在此添加控件通知处理程序代码
	difficulty = 1;
	MessageBox(L"您已选择了简单模式！", L"模式选择");
}


void MultiGame::OnBnClickedButtonMnormal()
{
	// TODO: 在此添加控件通知处理程序代码
	difficulty = 2;
	MessageBox(L"您已选择了普通模式！", L"模式选择");
}


void MultiGame::OnBnClickedButtonMdifficult()
{
	// TODO: 在此添加控件通知处理程序代码
	difficulty = 3;
	MessageBox(L"您已选择了困难模式！", L"模式选择");
}


void MultiGame::OnBnClickedButtonMchallange()
{
	// TODO: 在此添加控件通知处理程序代码
	difficulty = 4;
	MessageBox(L"您已选择了挑战模式！", L"模式选择");
}



void MultiGame::OnBnClickedButtonMoption()
{
	// TODO: 在此添加控件通知处理程序代码
	Option* p = new Option;
	p->DoModal();
	delete* p;
}
