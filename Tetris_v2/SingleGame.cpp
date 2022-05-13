// SingleGame.cpp: 实现文件
//

#include "pch.h"
#include "Tetris_v2.h"
#include "SingleGame.h"
#include "afxdialogex.h"
#include "time.h"
#include "framework.h"
#include "thread"
#include "WinUser.rh"
#include "Tetris_v2Dlg.h"
#include "Option.h"



// SingleGame 对话框

IMPLEMENT_DYNAMIC(SingleGame, CDialogEx)

SingleGame::SingleGame(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SINGLEGAME, pParent)
{

}

SingleGame::~SingleGame()
{
}

void SingleGame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SingleGame, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(WM_MYMSG,OnMyMsgHandler)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SINGLESTART, &SingleGame::OnBnClickedButtonSinglestart)
	ON_BN_CLICKED(IDC_BUTTON_SINGLEPAULE, &SingleGame::OnBnClickedButtonSinglepaule)
	ON_BN_CLICKED(IDC_BUTTON_SINGLERULES, &SingleGame::OnBnClickedButtonSinglerules)
	ON_BN_CLICKED(IDC_BUTTON_SINGLEQUIT, &SingleGame::OnBnClickedButtonSinglequit)
	ON_BN_CLICKED(IDC_BUTTON_EASY, &SingleGame::OnBnClickedButtonEasy)
	ON_BN_CLICKED(IDC_BUTTON_NORMAL, &SingleGame::OnBnClickedButtonNormal)
	ON_BN_CLICKED(IDC_BUTTON_DIFFICULT, &SingleGame::OnBnClickedButtonDifficult)
	ON_BN_CLICKED(IDC_BUTTON_CHALLANGE, &SingleGame::OnBnClickedButtonChallange)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_SINGLEOPTION, &SingleGame::OnBnClickedButtonSingleoption)
END_MESSAGE_MAP()



BOOL SingleGame::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
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
#define crazy_block 10
#define crazy_speed 300


using namespace std;
int Px = 0, Py = 3;			//方块移动跟踪的坐标,x对应竖直,y对应水平
static int speed = init_speed;	//定义速度的静态变量,使之在后续可以不断改变
static int score = 0;		//分数初始化
static int level = 1;		//等级初始化
static int difficulty = 0;
static int crazy = 0;
static bool ifcan[4];
static int k;
static int tempdifficulty = 0;
static int tempcrazy = 0;
static int tempstop = 0;



struct MAP
{
	COLORREF rgb = RGB(255, 255, 255);
};
MAP map[height][width];
MAP smap[4][4];
MAP control[4][4];


struct BLOCK
{
	MAP map[4][4];
}block[24];




void SingleGame::PaintMap()
{
	CDC* pDC = GetDC();

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			CBrush mapbrush;
			mapbrush.CreateSolidBrush(map[i][j].rgb);
			pDC->SelectObject(&mapbrush);
			pDC->Rectangle(w_space + space * j, h_space + space * i, w_space + space * (j + 1), h_space + space * (i + 1));
		}
	}
	ReleaseDC(pDC);

}



void SingleGame::PaintSMap()
{
	CDC* pDC = GetDC();


	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			CBrush smapbrush;
			smapbrush.CreateSolidBrush(smap[i][j].rgb);
			pDC->SelectObject(&smapbrush);
			pDC->Rectangle(sw_space + space * i, sh_space + space * j, sw_space + space * (i + 1), sh_space + space * (j + 1));

		}
	}
	ReleaseDC(pDC);
}





void SingleGame::OnPaint()
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
		PaintMap();
		PaintSMap();
		Maker();
		
	}
}





void Init_block()
{
	srand(time(NULL));
	COLORREF temp;
	//1,4
	int x = rand() / 255;
	int y = rand() / 255;
	int z = rand() / 255;
	temp = RGB(x, y, z);
	block[0].map[0][1].rgb = temp;
	block[0].map[1][1].rgb = temp;
	block[0].map[2][1].rgb = temp;
	block[0].map[3][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[1].map[2][0].rgb = temp;
	block[1].map[2][1].rgb = temp;
	block[1].map[2][2].rgb = temp;
	block[1].map[2][3].rgb = temp;


	//折线1
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[2].map[0][1].rgb = temp;
	block[2].map[1][1].rgb = temp;
	block[2].map[1][2].rgb = temp;
	block[2].map[2][2].rgb = temp;


	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[3].map[1][1].rgb = temp;
	block[3].map[1][2].rgb = temp;
	block[3].map[2][0].rgb = temp;
	block[3].map[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[4].map[0][1].rgb = temp;
	block[4].map[1][1].rgb = temp;
	block[4].map[1][2].rgb = temp;
	block[4].map[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[5].map[1][1].rgb = temp;
	block[5].map[1][2].rgb = temp;
	block[5].map[2][0].rgb = temp;
	block[5].map[2][1].rgb = temp;

	//折线2
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[6].map[0][2].rgb = temp;
	block[6].map[1][2].rgb = temp;
	block[6].map[1][1].rgb = temp;
	block[6].map[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[7].map[1][0].rgb = temp;
	block[7].map[1][1].rgb = temp;
	block[7].map[2][1].rgb = temp;
	block[7].map[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[8].map[0][2].rgb = temp;
	block[8].map[1][2].rgb = temp;
	block[8].map[1][1].rgb = temp;
	block[8].map[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[9].map[1][0].rgb = temp;
	block[9].map[1][1].rgb = temp;
	block[9].map[2][1].rgb = temp;
	block[9].map[2][2].rgb = temp;

	//L1
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[10].map[0][1].rgb = temp;
	block[10].map[1][1].rgb = temp;
	block[10].map[2][1].rgb = temp;
	block[10].map[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[11].map[2][0].rgb = temp;
	block[11].map[1][0].rgb = temp;
	block[11].map[1][1].rgb = temp;
	block[11].map[1][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[12].map[0][1].rgb = temp;
	block[12].map[0][2].rgb = temp;
	block[12].map[1][2].rgb = temp;
	block[12].map[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[13].map[0][2].rgb = temp;
	block[13].map[1][0].rgb = temp;
	block[13].map[1][1].rgb = temp;
	block[13].map[1][2].rgb = temp;

	//L2
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[14].map[0][2].rgb = temp;
	block[14].map[1][2].rgb = temp;
	block[14].map[2][2].rgb = temp;
	block[14].map[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[15].map[1][0].rgb = temp;
	block[15].map[2][0].rgb = temp;
	block[15].map[2][1].rgb = temp;
	block[15].map[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[16].map[0][1].rgb = temp;
	block[16].map[0][2].rgb = temp;
	block[16].map[1][1].rgb = temp;
	block[16].map[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[17].map[1][0].rgb = temp;
	block[17].map[1][1].rgb = temp;
	block[17].map[1][2].rgb = temp;
	block[17].map[2][2].rgb = temp;

	//正方形
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[18].map[1][1].rgb = temp;
	block[18].map[1][2].rgb = temp;
	block[18].map[2][2].rgb = temp;
	block[18].map[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[19].map[1][1].rgb = temp;
	block[19].map[1][2].rgb = temp;
	block[19].map[2][2].rgb = temp;
	block[19].map[2][1].rgb = temp;



	//凸起
	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[20].map[1][1].rgb = temp;
	block[20].map[2][1].rgb = temp;
	block[20].map[2][0].rgb = temp;
	block[20].map[2][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[21].map[0][1].rgb = temp;
	block[21].map[1][1].rgb = temp;
	block[21].map[2][1].rgb = temp;
	block[21].map[1][2].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[22].map[1][0].rgb = temp;
	block[22].map[1][1].rgb = temp;
	block[22].map[1][2].rgb = temp;
	block[22].map[2][1].rgb = temp;

	x = rand() / 255;
	y = rand() / 255;
	z = rand() / 255;
	temp = RGB(x, y, z);
	block[23].map[0][1].rgb = temp;
	block[23].map[1][1].rgb = temp;
	block[23].map[2][1].rgb = temp;
	block[23].map[1][0].rgb = temp;
}




void ChangeSmap(const BLOCK& b)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			smap[i][j].rgb = b.map[i][j].rgb;
		}
}




void GetBlock()
{
	int rad = rand() % 24;
	ChangeSmap(block[rad]);
}




void GiveToControl()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			control[i][j].rgb = smap[j][i].rgb;
		}
}




void ShowBlock()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (map[Px + i][Py + j].rgb == RGB(255, 255, 255))
				map[Px + i][Py + j].rgb = control[i][j].rgb;
		}
}




void GetNextBlock()
{
	GetBlock();
}



bool GetFail()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (control[i][j].rgb != RGB(255, 255, 255) && map[i][j + 4].rgb != RGB(255, 255, 255))
				return TRUE;
		}
	return FALSE;
}




bool Check(int x1, int y1)
{
	for (int j = 0; j < 4; ++j)
		for (int i = 0; i < 4; ++i)
		{
			if (control[3 - i][3 - j].rgb != RGB(255, 255, 255))
			{
				if (x1 + 3 - i + 1 >= height)
					return TRUE;
				else if (map[x1 + 3 - i + 1][y1 + 3 - j].rgb != RGB(255, 255, 255))
					return TRUE;
				else
					break;
			}
		}
	return FALSE;
}




void Down1(int x1, int y1)
{
	for (int j = 0; j < 4; ++j)
	{
		int flag = 0;
		for (int i = 0; i < 4; ++i)
		{
			if (control[i][j].rgb != RGB(255, 255, 255) && flag == 0)
			{
				flag = 1;
				map[x1 + i][y1 + j].rgb = RGB(255, 255, 255);
				map[x1 + i + 1][y1 + j].rgb = control[i][j].rgb;
			}
			if (control[i][j].rgb != RGB(255, 255, 255))
			{
				map[x1 + i + 1][y1 + j].rgb = control[i][j].rgb;
			}
		}
	}
}




void ChangeBlock()
{
	MAP temp[4][4];
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			temp[i][j] = control[i][j];
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			control[j][3 - i] = temp[i][j];
		}
	}
}





void SingleGame::ScoreandLevel()
{
	CFont font;
	font.CreatePointFont(text_size, L"宋体");
	CDC* pDC = GetDC();
	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->SelectObject(&font);
	CString scoreprint;
	scoreprint.Format(_T("SCORE: %d"), score);
	pDC->TextOutW(score_x, score_y, scoreprint);
	CString levelprint;
	levelprint.Format(_T("LEVEL: %d"), level);
	pDC->TextOutW(level_x, level_y, levelprint);
	ReleaseDC(pDC);
}




void SingleGame::Maker()
{
	CFont font;
	font.CreatePointFont(text_size, L"楷体");
	CDC* pDC = GetDC();
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SelectObject(&font);
	CString str;
	str.Format(_T("作者：第四组"));
	pDC->TextOutW(maker_x, maker_y, str);
	ReleaseDC(pDC);
}





void SingleGame::LevelSpeed()
{

	if (speed > 100)
	{
		if ((level - 1) % 2 == 0)
		{
			speed = init_speed - (level - 1) * 50;
		}
	}
}


LRESULT SingleGame::OnMyMsgHandler(WPARAM, LPARAM)
{
	return LRESULT();
}





void SummonBlockNormal()
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
				map[height - i - 1][j].rgb = RGB(rand() % 255, rand() % 255, rand() % 255);
			}
		}
	}

}




void SummonBlockDifficult()
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
				map[height - i - 1][j].rgb = RGB(rand() % 255, rand() % 255, rand() % 255);
			}
		}
	}

}



void SummonBlockCrazy()
{
	int a[crazy_block];
	for (int i = 0; i < crazy_block; ++i)
	{
		a[i] = rand() % 3 + 1;
	}
	int crazymap[crazy_block][width] = {};
	int temp_map[crazy_block][width];
	for (int i = 0; i < crazy_block; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			temp_map[i][j] = j + 1;
		}
	}

	for (int i = 0; i < crazy_block; ++i)
	{
		for (int j = 0; j < a[i]; ++j)
		{
			int k = rand() % (10 - j);
			crazymap[i][temp_map[i][k] - 1] = temp_map[i][k];
			for (int m = k; m < width - j - 1; ++m)
			{
				temp_map[i][m] = temp_map[i][m + 1];
			}
		}
	}

	for (int i = 0; i < crazy_block; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (crazymap[i][j] == 0)
			{
				map[height - i - 1][j].rgb = RGB(rand() % 255, rand() % 255, rand() % 255);
			}
		}
	}

}






void SingleGame::Clear()
{
	for (int i = 0; i < height; ++i)
	{
		int j;
		for (j = 0; j < width; ++j)
		{
			if (map[i][j].rgb == RGB(255, 255, 255))
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
						map[k][j].rgb = RGB(255, 255, 255);
					}
					else
					{
						map[k][j].rgb = map[k - 1][j].rgb;
					}
				}
			}
			score = score + 10;
			if (score % 100 == 0)
			{
				level = score / 100 + 1;
			}
			if (tempdifficulty == 4)
			{
				LevelSpeed();
				SetTimer(1, speed, NULL);
			}
			
			ScoreandLevel();

			if (tempdifficulty == 2 && level % 6 == 0)
			{
				for (int i = 0; i < height; ++i)
				{
					for (int j = 0; j < width; ++j)
					{
						map[i][j].rgb = RGB(255, 255, 255);
					}
				}
				SummonBlockNormal();
			}

			if (tempdifficulty == 3 && level % 6 == 0)
			{
				for (int i = 0; i < height; ++i)
				{
					for (int j = 0; j < width; ++j)
					{
						map[i][j].rgb = RGB(255, 255, 255);
					}
				}
				SummonBlockDifficult();
			}

			if (tempdifficulty == 0 && tempcrazy == 5 && level % 6 == 0)
			{
				for (int i = 0; i < height; ++i)
				{
					for (int j = 0; j < width; ++j)
					{
						map[i][j].rgb = RGB(255, 255, 255);
					}
				}
				SummonBlockCrazy();
			}

		}
	}
}






BOOL SingleGame::PreTranslateMessage(MSG* pMsg)
{

	// TODO: 在此添加专用代码和/或调用基类
	
	if (pMsg->message == WM_KEYDOWN)
	{
		if (tempstop != stop)
		{
			switch (pMsg->wParam)
			{
			case 'W':
			case VK_UP:
				for (int i = 0; i < 4; ++i)
					for (int j = 0; j < 4; ++j)
					{
						if (control[i][j].rgb != RGB(255, 255, 255))
						{
							map[Px + i][Py + j].rgb = RGB(255, 255, 255);
						}
					}

				ChangeBlock();
				for (int i = 0; i < 4; ++i)
				{
					ifcan[i] = false;
				}
				k = 0;

				if (Py == -1)
				{
					if (control[0][0].rgb != RGB(255, 255, 255) || control[1][0].rgb != RGB(255, 255, 255) || control[2][0].rgb != RGB(255, 255, 255) || control[3][0].rgb != RGB(255, 255, 255))
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (control[i][j].rgb != RGB(255, 255, 255))
								{
									if (map[Px + i][Py + j + 1].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							Py++;
							ShowBlock();
						}
						else
						{
							ChangeBlock();
							ChangeBlock();
							ChangeBlock();
							ShowBlock();
						}

					}
					else
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (control[i][j].rgb != RGB(255, 255, 255))
								{
									if (map[Px + i][Py + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							ShowBlock();
						}
						else
						{
							ChangeBlock();
							ChangeBlock();
							ChangeBlock();
							ShowBlock();
						}
					}
				}


				else if (Py == -2)
				{

					if (control[0][1].rgb != RGB(255, 255, 255) || control[1][1].rgb != RGB(255, 255, 255) || control[2][1].rgb != RGB(255, 255, 255) || control[3][1].rgb != RGB(255, 255, 255))
					{
						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (control[i][j].rgb != RGB(255, 255, 255))
								{
									if (map[Px + i][Py + j + 2].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							Py = Py + 2;
							ShowBlock();
						}
						else
						{
							ChangeBlock();
							ChangeBlock();
							ChangeBlock();
							ShowBlock();
						}

					}
					else
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (control[i][j].rgb != RGB(255, 255, 255))
								{
									if (map[Px + i][Py + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							ShowBlock();
						}
						else
						{
							ChangeBlock();
							ChangeBlock();
							ChangeBlock();
							ShowBlock();
						}
					}
				}


				else if (Py == 7)
				{

					if (control[0][3].rgb != RGB(255, 255, 255) || control[1][3].rgb != RGB(255, 255, 255) || control[2][3].rgb != RGB(255, 255, 255) || control[3][3].rgb != RGB(255, 255, 255))
					{
						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (control[i][j].rgb != RGB(255, 255, 255))
								{
									if (map[Px + i][Py + j - 1].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							Py = Py - 1;
							ShowBlock();
						}
						else
						{
							ChangeBlock();
							ChangeBlock();
							ChangeBlock();
							ShowBlock();
						}
					}
					else
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (control[i][j].rgb != RGB(255, 255, 255))
								{
									if (map[Px + i][Py + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							ShowBlock();
						}
						else
						{
							ChangeBlock();
							ChangeBlock();
							ChangeBlock();
							ShowBlock();
						}
					}
				}


				else if (Py == 8)
				{

					if (control[0][3].rgb != RGB(255, 255, 255) || control[1][3].rgb != RGB(255, 255, 255) || control[2][3].rgb != RGB(255, 255, 255) || control[3][3].rgb != RGB(255, 255, 255))
					{
						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (control[i][j].rgb != RGB(255, 255, 255))
								{
									if (map[Px + i][Py + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							Py = Py - 2;
							ShowBlock();
						}
						else
						{
							ChangeBlock();
							ChangeBlock();
							ChangeBlock();
							ShowBlock();
						}
					}
					else
					{

						for (int i = 0; i < 4; ++i)
							for (int j = 0; j < 4; ++j)
							{
								if (control[i][j].rgb != RGB(255, 255, 255))
								{
									if (map[Px + i][Py + j].rgb == RGB(255, 255, 255))
									{
										ifcan[k] = true;
										k++;
									}
								}
							}
						if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
						{
							ShowBlock();
						}
						else
						{
							ChangeBlock();
							ChangeBlock();
							ChangeBlock();
							ShowBlock();
						}
					}
				}


				else
				{

					for (int i = 0; i < 4; ++i)
						for (int j = 0; j < 4; ++j)
						{
							if (control[i][j].rgb != RGB(255, 255, 255))
							{
								if (map[Px + i][Py + j].rgb == RGB(255, 255, 255))
								{
									ifcan[k] = true;
									k++;
								}
							}
						}
					if (ifcan[0] == true && ifcan[1] == true && ifcan[2] == true && ifcan[3] == true)
					{
						ShowBlock();
					}
					else
					{
						ChangeBlock();
						ChangeBlock();
						ChangeBlock();
						ShowBlock();
					}
				}

				PaintMap();
				break;
			case 'A':
			case VK_LEFT:

			{

				if ((Py >= 1) || (Py == 0 && control[0][0].rgb == RGB(255, 255, 255) && control[1][0].rgb == RGB(255, 255, 255) && control[2][0].rgb == RGB(255, 255, 255) && control[3][0].rgb == RGB(255, 255, 255)) || (Py == -1 && control[0][1].rgb == RGB(255, 255, 255) && control[1][1].rgb == RGB(255, 255, 255) && control[2][1].rgb == RGB(255, 255, 255) && control[3][1].rgb == RGB(255, 255, 255)))
				{
					int edge[4] = { 100,100,100,100 };
					for (int i = 0; i < 4; ++i)
					{
						for (int j = 3; j > -1; --j)
						{
							if (control[i][j].rgb != RGB(255, 255, 255))
								edge[i] = j;
						}
					}

					if ((edge[0] == 100 && map[Px + 1][Py + edge[1] - 1].rgb == RGB(255, 255, 255) && map[Px + 2][Py + edge[2] - 1].rgb == RGB(255, 255, 255) && map[Px + 3][Py + edge[3] - 1].rgb == RGB(255, 255, 255))
						|| (edge[3] == 100 && map[Px + 1][Py + edge[1] - 1].rgb == RGB(255, 255, 255) && map[Px + 2][Py + edge[2] - 1].rgb == RGB(255, 255, 255) && map[Px][Py + edge[0] - 1].rgb == RGB(255, 255, 255))
						|| (edge[0] == 100 && edge[1] == 100 && map[Px + 2][Py + edge[2] - 1].rgb == RGB(255, 255, 255) && map[Px + 3][Py + edge[3] - 1].rgb == RGB(255, 255, 255))
						|| (edge[0] == 100 && edge[3] == 100 && map[Px + 1][Py + edge[1] - 1].rgb == RGB(255, 255, 255) && map[Px + 2][Py + edge[2] - 1].rgb == RGB(255, 255, 255))
						|| (edge[2] == 100 && edge[3] == 100 && map[Px][Py + edge[0] - 1].rgb == RGB(255, 255, 255) && map[Px + 1][Py + edge[1] - 1].rgb == RGB(255, 255, 255))
						|| (edge[0] == 100 && edge[1] == 100 && edge[2] == 100 && map[Px + 3][Py + edge[3] - 1].rgb == RGB(255, 255, 255))
						|| (edge[0] == 100 && edge[1] == 100 && edge[3] == 100 && map[Px + 2][Py + edge[2] - 1].rgb == RGB(255, 255, 255))
						|| (edge[3] == 100 && edge[0] == 100 && edge[2] == 100 && map[Px + 1][Py + edge[1] - 1].rgb == RGB(255, 255, 255))
						|| (map[Px][Py + edge[0] - 1].rgb == RGB(255, 255, 255) && map[Px + 1][Py + edge[1] - 1].rgb == RGB(255, 255, 255) && map[Px + 2][Py + edge[2] - 1].rgb == RGB(255, 255, 255) && map[Px + 3][Py + edge[3] - 1].rgb == RGB(255, 255, 255))
						)
					{
						for (int i = 0; i < 4; ++i)
						{
							int flag = 0;
							for (int j = 0; j < 4; ++j)
							{
								if (control[i][3 - j].rgb != RGB(255, 255, 255) && flag == 0)
								{
									flag = 1;
									map[Px + i][Py + 3 - j].rgb = RGB(255, 255, 255);
									map[Px + i][Py + 3 - j - 1].rgb = control[i][3 - j].rgb;
								}
								if (control[i][3 - j].rgb != RGB(255, 255, 255))
								{
									map[Px + i][Py + 3 - j - 1].rgb = control[i][3 - j].rgb;
								}
							}
						}
						PaintMap();
						--Py;
					}


				}


			}
			break;
			case 'D':
			case VK_RIGHT:

			{

				if ((Py <= 5) || (Py == 6 && control[0][3].rgb == RGB(255, 255, 255) && control[1][3].rgb == RGB(255, 255, 255) && control[2][3].rgb == RGB(255, 255, 255) && control[3][3].rgb == RGB(255, 255, 255)) || (Py == 7 && control[0][2].rgb == RGB(255, 255, 255) && control[1][2].rgb == RGB(255, 255, 255) && control[2][2].rgb == RGB(255, 255, 255) && control[3][2].rgb == RGB(255, 255, 255)))
				{
					int edge[4] = { 100,100,100,100 };
					for (int i = 0; i < 4; ++i)
					{
						for (int j = 0; j < 4; ++j)
						{
							if (control[i][j].rgb != RGB(255, 255, 255))
								edge[i] = j;
						}
					}

					if ((edge[0] == 100 && map[Px + 1][Py + edge[1] + 1].rgb == RGB(255, 255, 255) && map[Px + 2][Py + edge[2] + 1].rgb == RGB(255, 255, 255) && map[Px + 3][Py + edge[3] + 1].rgb == RGB(255, 255, 255))
						|| (edge[3] == 100 && map[Px + 1][Py + edge[1] + 1].rgb == RGB(255, 255, 255) && map[Px + 2][Py + edge[2] + 1].rgb == RGB(255, 255, 255) && map[Px][Py + edge[0] + 1].rgb == RGB(255, 255, 255))
						|| (edge[0] == 100 && edge[1] == 100 && map[Px + 2][Py + edge[2] + 1].rgb == RGB(255, 255, 255) && map[Px + 3][Py + edge[3] + 1].rgb == RGB(255, 255, 255))
						|| (edge[0] == 100 && edge[3] == 100 && map[Px + 1][Py + edge[1] + 1].rgb == RGB(255, 255, 255) && map[Px + 2][Py + edge[2] + 1].rgb == RGB(255, 255, 255))
						|| (edge[2] == 100 && edge[3] == 100 && map[Px][Py + edge[0] + 1].rgb == RGB(255, 255, 255) && map[Px + 1][Py + edge[1] + 1].rgb == RGB(255, 255, 255))
						|| (edge[0] == 100 && edge[1] == 100 && edge[2] == 100 && map[Px + 3][Py + edge[3] + 1].rgb == RGB(255, 255, 255))
						|| (edge[3] == 100 && edge[1] == 100 && edge[2] == 100 && map[Px][Py + edge[0] + 1].rgb == RGB(255, 255, 255))
						|| (edge[0] == 100 && edge[1] == 100 && edge[3] == 100 && map[Px + 2][Py + edge[2] + 1].rgb == RGB(255, 255, 255))
						|| (edge[3] == 100 && edge[0] == 100 && edge[2] == 100 && map[Px + 1][Py + edge[1] + 1].rgb == RGB(255, 255, 255))
						|| (map[Px][Py + edge[0] + 1].rgb == RGB(255, 255, 255) && map[Px + 1][Py + edge[1] + 1].rgb == RGB(255, 255, 255) && map[Px + 2][Py + edge[2] + 1].rgb == RGB(255, 255, 255) && map[Px + 3][Py + edge[3] + 1].rgb == RGB(255, 255, 255))
						)
					{
						for (int i = 0; i < 4; ++i)
						{
							int flag = 0;
							for (int j = 0; j < 4; ++j)
							{
								if (control[i][j].rgb != RGB(255, 255, 255) && flag == 0)
								{
									flag = 1;
									map[Px + i][Py + j].rgb = RGB(255, 255, 255);
									map[Px + i][Py + j + 1].rgb = control[i][j].rgb;
								}
								if (control[i][j].rgb != RGB(255, 255, 255))
								{
									map[Px + i][Py + j + 1].rgb = control[i][j].rgb;
								}
							}
						}
						PaintMap();
						++Py;
					}
				}


			}
			break;

			case 'S':
			case VK_DOWN:
				if (Check(Px, Py))
				{
					Clear();
					PaintMap();
					break;
				}
				Down1(Px, Py);
				++Px;
				PaintMap();
				break;
			}
		}
	}

	if (pMsg->message == WM_MYMSG)
	{
		if (Check(Px, Py))
		{
			Clear();
			PaintMap();
			Px = 0;
			Py = 3;
			GiveToControl();
			GetBlock();
			if (GetFail())
			{
				CString failmessage;
				failmessage.Format(_T("GAME OVER!\nYOUR FINAL SCORE is %d"), score);
				MessageBox(failmessage, _T("GAME OVER!"));
				KillTimer(1);
			}
			ShowBlock();
			PaintMap();
			GetNextBlock();
			PaintSMap();
		}
		Down1(Px, Py);
		++Px;
		PaintMap();

	}

	return FALSE;


}





void SingleGame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		PostMessage(WM_MYMSG);//不可用SendMessage
	}
}

void SingleGame::OnBnClickedButtonSinglestart()
{
	// TODO: 在此添加控件通知处理程序代码
	tempdifficulty = difficulty;
	tempcrazy = crazy;
	tempstop = 0;

	if (difficulty == 0 && crazy != 5)
	{
		MessageBox(L"请选择游戏难度后再进行游戏！", L"廖老师NB！");
	}

	else if (difficulty == 1)
	{
		Px = 0;
		Py = 3;

		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				map[i][j].rgb = RGB(255, 255, 255);
			}
		}

		score = 0;
		level = 1;
		speed = easy_speed;
		ScoreandLevel();

		PaintMap();
		Init_block();
		GetBlock();
		GiveToControl();
		ShowBlock();
		PaintMap();
		GetNextBlock();
		PaintSMap();
		UpdateData();
		SetTimer(1, speed, NULL);
	}

	else if (difficulty == 2)
	{
		Px = 0;
		Py = 3;

		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				map[i][j].rgb = RGB(255, 255, 255);
			}
		}

		score = 0;
		level = 1;
		speed = normal_speed;
		ScoreandLevel();
		SummonBlockNormal();
		PaintMap();
		Init_block();
		GetBlock();
		GiveToControl();
		ShowBlock();
		PaintMap();
		GetNextBlock();
		PaintSMap();
		UpdateData();
		SetTimer(1, speed, NULL);
	}

	else if (difficulty == 3)
	{
		Px = 0;
		Py = 3;

		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				map[i][j].rgb = RGB(255, 255, 255);
			}
		}

		score = 0;
		level = 1;
		speed = difficult_speed;
		ScoreandLevel();
		SummonBlockDifficult();
		PaintMap();
		Init_block();
		GetBlock();
		GiveToControl();
		ShowBlock();
		PaintMap();
		GetNextBlock();
		PaintSMap();
		UpdateData();
		SetTimer(1, speed, NULL);
	}

	else if (difficulty == 4)
	{
		Px = 0;
		Py = 3;

		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				map[i][j].rgb = RGB(255, 255, 255);
			}
		}

		score = 0;
		level = 1;
		speed = init_speed;
		ScoreandLevel();
	
		PaintMap();
		Init_block();	
		GetBlock();
		GiveToControl();
		ShowBlock();
		PaintMap();
		GetNextBlock();
		PaintSMap();
		UpdateData();
		SetTimer(1, speed, NULL);
	}
	else if (difficulty == 0 && crazy == 5)
	{
		Px = 0;
		Py = 3;
	
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				map[i][j].rgb = RGB(255, 255, 255);
			}
		}

		score = 0;
		level = 1;
		speed = crazy_speed;

		ScoreandLevel();
		SummonBlockCrazy();
		PaintMap();	
		Init_block();
		GetBlock();
		GiveToControl();
		ShowBlock();
		PaintMap();
		GetNextBlock();
		PaintSMap();
		UpdateData();
		SetTimer(1, speed, NULL);
	}
	else MessageBox(L"？？？你有问题？？？\n你™偷改我的数据！！！",L"NM$L");


}


void SingleGame::OnBnClickedButtonSinglepaule()
{
	// TODO: 在此添加控件通知处理程序代码
	static bool pause = true;
	if (pause)
	{
		tempstop = stop;
		SetTimer(1, stop, NULL);
		pause = false;
		MessageBox(_T("游戏暂停！"),L"暂停");
	}
	else
	{
		tempstop = 0;
		SetTimer(1, speed, NULL);
		pause = true;
		MessageBox(_T("游戏恢复！"),L"恢复");
	}

}


void SingleGame::OnBnClickedButtonSinglerules()
{
	// TODO: 在此添加控件通知处理程序代码
	static bool rule = true;
	if (rule)
	{
		MessageBox(_T("RULE 1：方向键左右或键盘AD控制方块的方向\nRULE 2：方向键上或键盘W使方块顺时针旋转\nRULE 3：方向键下或键盘S加速方块的下落\nRULR 4：方块堆满一行自动消除并记10分\nRULE 5：每100分会使LEVEL+1\nRULE 6：方块达到顶端则游戏结束\nRULE 7：若玩急眼导致损坏电脑，本人概不负责！\n\n\nEASY模式：地图初始无方块，且速度固定\nNORMAL模式：地图随机生成3层无序方块，速度略快于EASY，每6个level会重置地图\nDIFFICULT模式：地图随机生成6格无序方块，且速度略快于NORMAL，每6个level会重置地图\nCHALLANGE模式：无初始方块，但速度会随着LEVEL等级而增加\n\n\nGOOD LUCK AND HAVE FUN!"), L"廖老师NB！");
		rule = false;
	}
	else
	{
		MessageBox(_T("都™看过一遍了\n你还™看\n你看NM呢？"),L"连按五次有彩蛋哟！");
		crazy++;
	}
}


void SingleGame::OnBnClickedButtonSinglequit()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			map[i][j].rgb = RGB(255, 255, 255);
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			smap[i][j].rgb = RGB(255, 255, 255);
		}
	}
	PaintMap();
	PaintSMap();
	difficulty = 0;
	tempdifficulty = 0;
	crazy = 0;
	tempcrazy = 0;
	tempstop = 0;
	OnOK();
}



void SingleGame::OnBnClickedButtonEasy()
{
	// TODO: 在此添加控件通知处理程序代码
	difficulty = 1;
	MessageBox(L"您已选择了简单模式！",L"模式选择");
}


void SingleGame::OnBnClickedButtonNormal()
{
	// TODO: 在此添加控件通知处理程序代码
	difficulty = 2;
	MessageBox(L"您已选择了普通模式！", L"模式选择");
}


void SingleGame::OnBnClickedButtonDifficult()
{
	// TODO: 在此添加控件通知处理程序代码
	difficulty = 3;
	MessageBox(L"您已选择了困难模式！", L"模式选择");
}


void SingleGame::OnBnClickedButtonChallange()
{
	// TODO: 在此添加控件通知处理程序代码
	difficulty = 4;
	MessageBox(L"您已选择了挑战模式！", L"模式选择");
}


int SingleGame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码


	return 0;
}




void SingleGame::OnBnClickedButtonSingleoption()
{
	// TODO: 在此添加控件通知处理程序代码
	Option* p = new Option;
	p->DoModal();
	delete* p;
}
