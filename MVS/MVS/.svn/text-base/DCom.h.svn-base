#pragma once
#include "afxwin.h"


// DCom dialog

class DCom : public CDialog
{
	DECLARE_DYNAMIC(DCom)

public:
	DCom(CWnd* pParent = NULL);   // standard constructor
	virtual ~DCom();

// Dialog Data
	enum { IDD = IDD_DCOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Check_Udp;
	CButton m_Check_Serial;
	CEdit m_Edit_Port;
	CEdit m_Edit_Ip;
	CComboBox m_Combo_Serial;
};
