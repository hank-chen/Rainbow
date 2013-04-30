// DCom.cpp : implementation file
//

#include "stdafx.h"
#include "MVS.h"
#include "DCom.h"


// DCom dialog

IMPLEMENT_DYNAMIC(DCom, CDialog)

DCom::DCom(CWnd* pParent /*=NULL*/)
	: CDialog(DCom::IDD, pParent)
{

}

DCom::~DCom()
{
}

void DCom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_UDP, m_Check_Udp);
	DDX_Control(pDX, IDC_CHECK_SERIAL, m_Check_Serial);
	DDX_Control(pDX, IDC_EDIT_PORT, m_Edit_Port);
	DDX_Control(pDX, IDC_EDIT_IPADD, m_Edit_Ip);
	DDX_Control(pDX, IDC_COMBO_SERIAL, m_Combo_Serial);
}


BEGIN_MESSAGE_MAP(DCom, CDialog)
END_MESSAGE_MAP()


// DCom message handlers
