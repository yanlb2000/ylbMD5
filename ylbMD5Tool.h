// ylbMD5Tool.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CylbMD5ToolApp:
// See ylbMD5Tool.cpp for the implementation of this class
//

class CylbMD5ToolApp : public CWinApp
{
public:
	CylbMD5ToolApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation


	DECLARE_MESSAGE_MAP()
};


