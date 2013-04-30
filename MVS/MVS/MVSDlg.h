// MVSDlg.h : header file
//

#pragma once
#include "amc.h"
#include "DSetup.h"
#include "DCom.h"
#include "afxwin.h"


// CMVSDlg dialog
class CMVSDlg : public CDialog
{
// Construction
public:
	CMVSDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MVS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CAmc m_AMC;
	//Other object
	DSetup m_Setup;
	DCom m_Com;
	  //General function
	bool SaveConfig (char *FileName); 
	bool LoadConfig (char *FileName);
	bool LogImage (CString LogFolder, CString FileName,IplImage* img);
	bool InitOpenCv (void);
	bool CMVSDlg::SendCom(CString str_Data);

	DECLARE_EVENTSINK_MAP()
	void OnNewImageAmc();
	afx_msg void OnConfigureImageprocess();
	CEdit m_Edit_IpAddress;
	afx_msg void OnBnClickedCancel();
	CEdit m_EditStatus;
	afx_msg void OnBnClickedSample();
	afx_msg void OnBnClickedOk();
	afx_msg void OnComSetup();
	CAmc m_AMC_2;
	void OnNewImageAmc_2();
	CEdit m_Edit_IpAddress_2;
};
