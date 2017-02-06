
#include "stdafx.h"
#include "Viewer.h"
#include "ViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CViewerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CViewerApp::CViewerApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CViewerApp theApp;

// CViewerApp initialization
BOOL CViewerApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	AfxEnableControlContainer();
	CShellManager *pShellManager = new CShellManager;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	CViewerDlg *dlg = new CViewerDlg();
	dlg->Create(CViewerDlg::IDD);
	dlg->ShowWindow(SW_SHOW);
	m_pMainWnd = dlg;

	dlg->Run();
	
	delete dlg;
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
#ifndef _AFXDLL
	ControlBarCleanUp();
#endif
	return FALSE;
}
