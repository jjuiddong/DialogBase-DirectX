#include "stdafx.h"
#include "Viewer.h"
#include "MainView.h"

using namespace graphic;


// CMainView
CMainView::CMainView()
	: m_LButtonDown(false)
	, m_RButtonDown(false)
	, m_MButtonDown(false)
{
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

CMainView::~CMainView()
{
}


BEGIN_MESSAGE_MAP(CMainView, CScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

void CMainView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CMainView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}


// CMainView diagnostics
#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainView message handlers
bool CMainView::Init()
{
	CRect cr;
	GetClientRect(cr);
	const int width = cr.Width();
	const int height = cr.Height();
	m_renderer.CreateDirectX(m_hWnd, width, height);

	GetMainCamera()->Init(&m_renderer);
	GetMainCamera()->SetCamera(Vector3(10, 10, -10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GetMainCamera()->SetProjection(D3DX_PI / 4.f, (float)width / (float)height, 1.f, 10000.0f);

	GetMainLight().Init(cLight::LIGHT_DIRECTIONAL,
		Vector4(0.2f, 0.2f, 0.2f, 1), Vector4(0.9f, 0.9f, 0.9f, 1),
		Vector4(0.2f, 0.2f, 0.2f, 1));
	GetMainLight().SetPosition(Vector3(-30000, 30000, -30000));
	GetMainLight().SetDirection(Vector3(1, -1, 1).Normal());

	m_renderer.GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	m_renderer.GetDevice()->LightEnable(0, true);

	return true;
}


bool CMainView::Render(const float deltaSeconds)
{
	m_renderer.Update(deltaSeconds);
	m_renderer.ClearScene();
	m_renderer.BeginScene();

	m_renderer.RenderGrid();
	m_renderer.RenderAxis();
	m_renderer.RenderFPS();

	m_renderer.EndScene();
	m_renderer.Present();

	return true;
}


void CMainView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_LButtonDown = true;
	m_curPos = point;
	CScrollView::OnLButtonDown(nFlags, point);
}


void CMainView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_LButtonDown = false;
	CScrollView::OnLButtonUp(nFlags, point);
}


void CMainView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_RButtonDown = false;
	CScrollView::OnRButtonUp(nFlags, point);
}


void CMainView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_RButtonDown = true;
	m_curPos = point;
	CScrollView::OnRButtonDown(nFlags, point);
}


void CMainView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_RButtonDown)
	{
		const int x = point.x - m_curPos.x;
		const int y = point.y - m_curPos.y;
		m_curPos = point;
		graphic::GetMainCamera()->Yaw2(x * 0.005f);
		graphic::GetMainCamera()->Pitch2(y * 0.005f);
	}

	CScrollView::OnMouseMove(nFlags, point);
}


BOOL CMainView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	const float len = graphic::GetMainCamera()->GetDistance();
	float zoomLen = (len > 100) ? 50 : (len / 4.f);
	GetMainCamera()->Zoom((zDelta<0) ? -zoomLen : zoomLen);
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}
