// AftRegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageMosaic.h"
#include "AftRegDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// AftRegDlg dialog

IMPLEMENT_DYNAMIC(AftRegDlg, CDialog)

AftRegDlg::AftRegDlg()
: m_bCalImgLoc2(false)
{

}
AftRegDlg::AftRegDlg(CWnd* pParent /*=NULL*/,CImage& image)
		: CDialog(AftRegDlg::IDD, pParent)
{
	if (image.IsNull())
	{
		AfxMessageBox(_T("�㻹û�д�һ��Ҫ�����ͼ���ļ���"));
		return;
	} 
	//p = image;
	//m_AftRegImage = image;
	imageCopy(m_AftRegImage,image);
	//m_AftRegImage_1 = image;
	int dd = 0;
}
void AftRegDlg::imageCopy(CImage &destImg,CImage &srcImg)		//ͼ�ĸ��� 
{
	int width = srcImg.GetWidth();
	int pitch = srcImg.GetPitch();
	int height = srcImg.GetHeight();

	if (!destImg.IsNull())
	{
		destImg.Destroy();
	}

	destImg.Create(width, height, 24);
	while(destImg.IsNull())
	{
		destImg.Create(width, height, 24);
	}
	int _pitch = destImg.GetPitch();

	BYTE *pDestD = (BYTE *)destImg.GetBits();
	BYTE *pSrcD = (BYTE *)srcImg.GetBits();

	for (int y=0; y<height; y++)
	{
		memcpy(pDestD + y * _pitch, pSrcD + y * pitch, abs(pitch));
	}

	if (srcImg.GetBPP() <= 8)
	{
		RGBQUAD  D_pal[256];
		srcImg.GetColorTable(0, 256, D_pal);
		destImg.SetColorTable(0, 256, D_pal);
	}
}


AftRegDlg::~AftRegDlg()
{

}

void AftRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NewImageShow, imageShow);
}


BEGIN_MESSAGE_MAP(AftRegDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ImageSave, &AftRegDlg::Imagesave)
	ON_BN_CLICKED(IDCANCEL, &AftRegDlg::Cancel)
END_MESSAGE_MAP()


// AftRegDlg message handlers
BOOL AftRegDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	CalImageLocation2();
	//ShowWindow(SW_NORMAL);
 
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void AftRegDlg::Imagesave()
{
	// TODO: Add your control notification handler code here
	if(m_AftRegImage.IsNull())
	{
		AfxMessageBox(_T("�㻹û�д�һ��Ҫ�����ͼ���ļ���"));
		return;
	}
	CString strFilter;
	//strFilter = "λͼ�ļ�|*.bmp|JPEG ͼ���ļ�|*.jpg|GIF ͼ���ļ�|*.gif|PNG ͼ���ļ�|*.png||";
	strFilter = "JPEG ͼ���ļ�|*.jpg";
	CFileDialog dlg(FALSE,NULL,NULL,NULL,strFilter);
	if(IDOK != dlg.DoModal())
	{
		return;
	}
	
	CString strFileName;
	CString strExtension;
	strFileName = dlg.m_ofn.lpstrFile;
	strExtension = "jpg";
	strFileName = strFileName + _T(".") + strExtension;
	HRESULT hResult = m_AftRegImage.Save(strFileName);
	if(FAILED(hResult))
	{
		AfxMessageBox(_T("����ͼ���ļ�ʧ�ܣ�"));
	}
}
void AftRegDlg::OnClose(void)
{
	DestroyWindow();
}
void AftRegDlg::PostNcDestroy(void)
{
 delete this;
 CDialog::PostNcDestroy();
}

void AftRegDlg::Cancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
	DestroyWindow();
}

void AftRegDlg::OnPaint(void)
{
	//CPaintDC dc(this);
	CDialog::OnPaint();
    
	//��ʾƴ�Ӻ�ͼ��
	if(!m_AftRegImage.IsNull())
	{
		//CDC* pShowDC = imageShow.GetDC();
		//HDC hdc = pShowDC3->GetSafeHdc();
		CPaintDC dc(GetDlgItem(IDC_NewImageShow));
		HDC hdc = dc.GetSafeHdc();
		CRect m_rcShow;
		imageShow.GetWindowRect(&m_rcShow);
		::SetStretchBltMode(hdc,HALFTONE);
		::SetBrushOrgEx(hdc,0,0,NULL);
		m_AftRegImage.StretchBlt(hdc,0,0,m_rcShow.Width(),m_rcShow.Height(),SRCCOPY);
		//m_AftRegImage_1->Draw(hdc,0,0,m_rcShow.Width(),m_rcShow.Height());
		//imageShow.ReleaseDC(pShowDC);
	}
}

void AftRegDlg::CalImageLocation2(void)
{
	CWnd *pWnd3;
	pWnd3 = GetDlgItem(IDC_NewImageShow);
	pWnd3->SetWindowPos(NULL,20,30,779,450,SWP_NOZORDER);
}
