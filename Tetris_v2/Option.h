#pragma once


// Option 对话框

class Option : public CDialogEx
{
	DECLARE_DYNAMIC(Option)

public:
	Option(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Option();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OPTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
};
