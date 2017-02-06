
#include "stdafx.h"
#include "Viewer.h"
#include "ViewerDlg.h"
#include "afxdialogex.h"
#include "MainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CViewerDlg dialog
CViewerDlg::CViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIEWER_DIALOG, pParent)
	, m_mainView(NULL)
	, m_loop(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViewerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CViewerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CViewerDlg::OnBnClickedCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


BEGIN_ANCHOR_MAP(CViewerDlg)
	ANCHOR_MAP_ENTRY(IDC_STATIC_VIEW, ANF_TOPLEFT | ANF_BOTTOMRIGHT )
END_ANCHOR_MAP()


// CViewerDlg message handlers
BOOL CViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	InitAnchors();

	m_mainView = new CMainView();
	m_mainView->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 100), this, 0);
	m_mainView->ShowWindow(SW_SHOW);

	if (CWnd *wnd = GetDlgItem(IDC_STATIC_VIEW))
	{
		CRect r;
		wnd->GetWindowRect(r);
		ScreenToClient(r);
		m_mainView->MoveWindow(r);
		m_mainView->Init();
	}

	return TRUE;
}

void CViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CViewerDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CViewerDlg::OnBnClickedCancel()
{
	m_loop = false;
	CDialogEx::OnCancel();
}


void CViewerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect wr;
	GetWindowRect(wr);
	HandleAnchors(wr);

	if (m_mainView)
	{
		if (CWnd *wnd = GetDlgItem(IDC_STATIC_VIEW))
		{
			CRect r;
			wnd->GetWindowRect(r);
			ScreenToClient(r);
			m_mainView->MoveWindow(r);
		}
	}
}


// Main Loop
bool CViewerDlg::Run()
{
	while (m_loop)
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
			{
				break;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		const int curT = timeGetTime();
		static int oldT = curT;
		const int deltaT = curT - oldT;
		if (deltaT > 10)
		{
			const float t = deltaT * 0.001f;
			oldT = curT;
			Update(t);
		}

		SwitchToThread();
	}

	return true;
}


void CViewerDlg::Update(const float deltaSeconds)
{
	if (m_mainView)
		m_mainView->Render(deltaSeconds);
}
