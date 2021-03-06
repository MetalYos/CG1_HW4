#pragma once


// CResolutionDialog dialog

class CResolutionDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CResolutionDialog)

public:
	CResolutionDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CResolutionDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESOLUTION_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int Resolution;
};
