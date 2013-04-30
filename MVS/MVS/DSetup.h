#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "highgui.h"
#include "cv.h"

// DSetup dialog

class DSetup : public CDialog
{
	DECLARE_DYNAMIC(DSetup)

public:
	DSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~DSetup();
	void DInit();

// Dialog Data
	enum { IDD = IDD_DSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Check_Canny;
	CButton m_Check_Contour;
	CButton m_Check_Histogram;
	CButton m_Check_Aoi;
	CButton m_Check_Raw;
	CComboBox m_Combo_Resolution;
	CButton m_Check_Select;
	CButton m_Check_Roi;
	CButton m_Check_ReferenceLine;
	CButton m_Check_SampleObj;
	CButton m_Check_Equalize;
	CButton m_Check_Aproximate;
	CButton m_Check_ColorMatching;
	CSliderCtrl m_Slider_CannyUnder;
	CSliderCtrl m_Slider_CannyUpper;
	CSliderCtrl m_Slider_ArcMin;
	CSliderCtrl m_Slider_ArcMax;
	CSliderCtrl m_Slider_AreaMin;
	CSliderCtrl m_Slider_AreaMax;
	CSliderCtrl m_Slider_Aproximate;
	CSliderCtrl m_Slider_ColorMatchTolerance;

	CvPoint ROI_Top;
	CvPoint ROI_Bottom;

	CButton m_Check_LogImage;
	CButton m_Check_ColorSignature;
	CButton m_Check_Border;
};
