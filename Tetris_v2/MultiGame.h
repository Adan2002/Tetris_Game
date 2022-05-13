#pragma once


// MultiGame 对话框

class MultiGame : public CDialogEx
{
	DECLARE_DYNAMIC(MultiGame)

public:
	MultiGame(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MultiGame();
	void OnPaint();
	void PaintMapP1();
	void PaintMapP2();
	void PaintSMapP1();
	void PaintSMapP2();

	void ScoreandLevel2();

	LRESULT OnMyMsgHandler(WPARAM, LPARAM);

	void LevelSpeed();

	void whowin();

	void Clear1();

	void Clear2();

	void ScoreandLevel1();

	void OnTimer(UINT_PTR nIDEvent);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MULTIPLAYER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMstart();
	afx_msg void OnBnClickedButtonMpause();
	afx_msg void OnBnClickedButtonMrules();
	afx_msg void OnBnClickedButtonMquit();
	afx_msg void OnBnClickedButtonMeasy();
	afx_msg void OnBnClickedButtonMnormal();
	afx_msg void OnBnClickedButtonMdifficult();
	afx_msg void OnBnClickedButtonMchallange();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonMoption();
};
