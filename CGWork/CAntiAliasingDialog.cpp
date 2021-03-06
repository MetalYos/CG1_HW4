// CAntiAliasingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "CAntiAliasingDialog.h"
#include "afxdialogex.h"


// CAntiAliasingDialog dialog

IMPLEMENT_DYNAMIC(CAntiAliasingDialog, CDialogEx)

CAntiAliasingDialog::CAntiAliasingDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ANTIALIAS_DLG, pParent)
{

}

CAntiAliasingDialog::~CAntiAliasingDialog()
{
}

void CAntiAliasingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	int helper = antiAliasing.Type;
	DDX_CBIndex(pDX, IDC_FILTER_TYPE, helper);
	antiAliasing.Type = (AAFilterType)helper;

	helper = antiAliasing.Size;
	DDX_CBIndex(pDX, IDC_FILTER_SIZE, helper);
	antiAliasing.Size = (AAKernelSize)helper;
}


BEGIN_MESSAGE_MAP(CAntiAliasingDialog, CDialogEx)
END_MESSAGE_MAP()


// CAntiAliasingDialog message handlers
