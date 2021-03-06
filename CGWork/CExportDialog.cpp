// CExportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "CExportDialog.h"
#include "afxdialogex.h"


// CExportDialog dialog

IMPLEMENT_DYNAMIC(CExportDialog, CDialogEx)

CExportDialog::CExportDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SAVE_IMAGE_DLG, pParent)
	, m_height(0)
	, m_width(0)
	, m_fileName(L"exportedImage.png")
{

}

CExportDialog::~CExportDialog()
{
}

void CExportDialog::SetHeight(unsigned int height)
{
	m_height = height;
}

unsigned int CExportDialog::GetHeight()
{
	return m_height;
}

void CExportDialog::SetWidth(unsigned int width)
{
	m_width = width;
}

unsigned int CExportDialog::GetWidth()
{
	return m_width;
}

CString CExportDialog::GetFileName()
{
	return m_fileName;
}

void CExportDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EXPORT_HEIGHT, m_height);
	DDV_MinMaxUInt(pDX, m_height, 0, UINT_MAX);
	DDX_Text(pDX, IDC_EXPORT_WIDTH, m_width);
	DDV_MinMaxUInt(pDX, m_width, 0, UINT_MAX);
}


BEGIN_MESSAGE_MAP(CExportDialog, CDialogEx)
	ON_BN_CLICKED(ID_BUTTON_BROWSE, &CExportDialog::OnClickedButtonBrowse)
END_MESSAGE_MAP()


// CExportDialog message handlers


void CExportDialog::OnClickedButtonBrowse()
{
	TCHAR szFilters[] = _T("PNG image files (*.png)|*.png|");
	CFileDialog dlg(FALSE, _T("png"), _T("*.png"), OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK)
	{
		m_fileName = dlg.GetPathName();
	}
}
