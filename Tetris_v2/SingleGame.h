#pragma once


// SingleGame 对话框

class SingleGame : public CDialogEx
{
	DECLARE_DYNAMIC(SingleGame)

public:
	SingleGame(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SingleGame();
	void PaintMap();
	void PaintSMap();
	void Clear();
	void ScoreandLevel();
	void Maker();
	void LevelSpeed();
	BOOL PlaySound(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound);
	afx_msg LRESULT OnMyMsgHandler(WPARAM, LPARAM);



// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void OnPaint();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonSinglestart();
	afx_msg void OnBnClickedButtonSinglepaule();
	afx_msg void OnBnClickedButtonSinglerules();
	afx_msg void OnBnClickedButtonSinglequit();
	afx_msg void OnBnClickedButtonEasy();
	afx_msg void OnBnClickedButtonNormal();
	afx_msg void OnBnClickedButtonDifficult();
	afx_msg void OnBnClickedButtonChallange();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTooption1();
	afx_msg void OnBnClickedButtonSingleoption();
};
