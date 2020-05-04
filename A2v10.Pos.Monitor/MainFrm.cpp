
// Copyright © 2020 Alex Kukhtin. All rights reserved.

#include "pch.h"
#include "framework.h"
#include "A2v10.Pos.Monitor.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

LPCWSTR MONITOR_WNDCLASS = L"A2V10_MONITOR_WNDCLASS";
LPCWSTR MONITOR_MSG = L"A2v10_POS_MONITOR";

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_MESSAGE(WM_COPYDATA, OnMonitorMessage)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_EDIT_CLEARALL, OnClearAll)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwExtStyle = 0;

	DWORD dwEditStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE |
		WS_HSCROLL | WS_VSCROLL | ES_READONLY |
		ES_AUTOVSCROLL | ES_NOHIDESEL | ES_SAVESEL |
		ES_SELECTIONBAR | ES_AUTOHSCROLL;


	if (!m_wndView.CreateEx(dwExtStyle, dwEditStyle, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST))
	{
		ATLTRACE(L"Failed to create view window\n");
		return -1;
	}

	CString strfontFamily = L"Consolas";
	CHARFORMAT2W cfDefault;
	memset(&cfDefault, 0, sizeof(CHARFORMAT2W));
	cfDefault.cbSize = sizeof(CHARFORMAT2W);
	cfDefault.dwEffects = CFE_PROTECTED;
	cfDefault.dwMask = CFM_BOLD | CFM_FACE | CFM_SIZE |
		CFM_CHARSET | CFM_PROTECTED;
	cfDefault.yHeight = 180;
	cfDefault.bCharSet = RUSSIAN_CHARSET;
	_tcscpy_s(cfDefault.szFaceName, LF_FACESIZE, strfontFamily);

	m_wndView.SetDefaultCharFormat(cfDefault);
	m_wndView.SetBackgroundColor(FALSE, RGB(230, 231, 232));
	m_wndView.SetOptions(ECOOP_OR, ECO_NOHIDESEL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !__super::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	WNDCLASS wndClass = {
		CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		AfxWndProc,
		0, 0,
		AfxGetInstanceHandle(),
		AfxGetApp()->LoadIcon(IDR_MAINFRAME),
		nullptr, //::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_BACKGROUND + 1),
		MAKEINTRESOURCE(IDR_MAINFRAME),
		MONITOR_WNDCLASS
	};
	AfxRegisterClass(&wndClass);
	cs.lpszClass = MONITOR_WNDCLASS;

	return TRUE;
}

// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return __super::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// afx_msg 
LRESULT CMainFrame::OnMonitorMessage(WPARAM wParam, LPARAM lParam)
{
	COPYDATASTRUCT* copyData = reinterpret_cast<COPYDATASTRUCT*>(lParam);
	LPCWSTR szMessage = reinterpret_cast<LPCWSTR>(copyData->lpData);
	CString strResult(szMessage);
	strResult += L"\r";
	m_wndView.SetSel(-1, -1);
	m_wndView.ReplaceSel(strResult);
	return 0L;
}

// afx_msg 
void CMainFrame::OnClearAll()
{
	m_wndView.LockWindowUpdate();
	m_wndView.SetSel(0, -1);
	m_wndView.ReplaceSel(L"");
	m_wndView.SetSel(1, 1);
	m_wndView.UnlockWindowUpdate();
}

// afx_msg
void CMainFrame::OnEditCopy()
{
	m_wndView.Copy();
}

// afx_msg
void CMainFrame::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	CHARRANGE cr;
	m_wndView.GetSel(cr);
	pCmdUI->Enable(cr.cpMin != cr.cpMax);
}
