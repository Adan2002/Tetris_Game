// Option.cpp: 实现文件
//

#include "pch.h"
#include "Tetris_v2.h"
#include "Option.h"
#include "afxdialogex.h"
#include "windows.h"
#include "mmsystem.h"
#pragma comment(lib, "WINMM.LIB")



using namespace std;


// Option 对话框

IMPLEMENT_DYNAMIC(Option, CDialogEx)

Option::Option(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OPTION, pParent)
{

}

Option::~Option()
{
}

void Option::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Option, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &Option::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Option::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &Option::OnBnClickedButton1)
END_MESSAGE_MAP()


// Option 消息处理程序


void Option::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	PlaySound(LPWSTR(IDR_WAVE3), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
}


void Option::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	PlaySound(NULL, NULL, NULL);
}


void Option::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
