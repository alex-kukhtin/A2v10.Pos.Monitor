
// A2v10.Pos.Monitor.h : main header file for the A2v10.Pos.Monitor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMainApp:
// See A2v10.Pos.Monitor.cpp for the implementation of this class
//

class CMainApp : public CWinApp
{
public:
	CMainApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMainApp theApp;
