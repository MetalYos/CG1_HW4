#pragma once
#include "Vec4.h"

// CSensDialog dialog

class CSensDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSensDialog)

public:
	CSensDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSensDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENS_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	int m_min_rot, m_max_rot;
	int m_min_trans, m_max_trans;
	int m_min_scale, m_max_scale;

	int m_rotSens;
	int m_transSens;
	int m_scaleSens;

	CSliderCtrl m_rotCtrl;
	CSliderCtrl m_transCtrl;
	CSliderCtrl m_scaleCtrl;

public:
	Vec4 GetSensitivity();
	virtual BOOL OnInitDialog();
};
