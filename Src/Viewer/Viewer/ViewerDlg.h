#pragma once


// CViewerDlg dialog
class CMainView;
class CViewerDlg : public CDialogEx
{
public:
	CViewerDlg(CWnd* pParent = NULL);	// standard constructor
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEWER_DIALOG };
//#endif

	bool Run();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void Update(const float deltaSeconds);


protected:
	HICON m_hIcon;
	bool m_loop;
	CMainView *m_mainView;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_ANCHOR_MAP();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
