// DSetup.cpp : implementation file
//

#include "stdafx.h"
#include "MVS.h"
#include "DSetup.h"


// DSetup dialog

IMPLEMENT_DYNAMIC(DSetup, CDialog)

DSetup::DSetup(CWnd* pParent /*=NULL*/)
	: CDialog(DSetup::IDD, pParent)
{

}

DSetup::~DSetup()
{
}

void DSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_CANNY, m_Check_Canny);
	DDX_Control(pDX, IDC_CHECK_CONTOUR, m_Check_Contour);
	DDX_Control(pDX, IDC_CHECK_HISTOGRAM, m_Check_Histogram);
	DDX_Control(pDX, IDC_CHECK_AOI, m_Check_Aoi);
	DDX_Control(pDX, IDC_CHECK_RAW, m_Check_Raw);
	DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_Combo_Resolution);
	DDX_Control(pDX, IDC_CHECK_SELECT, m_Check_Select);
	DDX_Control(pDX, IDC_CHECK_ROI, m_Check_Roi);
	DDX_Control(pDX, IDC_CHECK_REFERENCELINE, m_Check_ReferenceLine);
	DDX_Control(pDX, IDC_CHECK_SAMPLEOBJECT, m_Check_SampleObj);
	DDX_Control(pDX, IDC_CHECK_EQUALIZE, m_Check_Equalize);
	DDX_Control(pDX, IDC_CHECK_APROXIMATE, m_Check_Aproximate);
	DDX_Control(pDX, IDC_CHECK_COLORMATCHING, m_Check_ColorMatching);
	DDX_Control(pDX, IDC_SLIDER_CANNY_UNDER, m_Slider_CannyUnder);
	DDX_Control(pDX, IDC_SLIDER_CANNY_UPPER, m_Slider_CannyUpper);
	DDX_Control(pDX, IDC_SLIDER_ARC_MIN, m_Slider_ArcMin);
	DDX_Control(pDX, IDC_SLIDER_ARC_MAX, m_Slider_ArcMax);
	DDX_Control(pDX, IDC_SLIDER_AREA_MIN, m_Slider_AreaMin);
	DDX_Control(pDX, IDC_SLIDER_AREA_MAX, m_Slider_AreaMax);
	DDX_Control(pDX, IDC_SLIDER_APROXIMATE, m_Slider_Aproximate);
	DDX_Control(pDX, IDC_SLIDER_COLORMATCH, m_Slider_ColorMatchTolerance);
	DDX_Control(pDX, IDC_CHECK_LOGIMAGE, m_Check_LogImage);
	DDX_Control(pDX, IDC_CHECK_COLORSIGN, m_Check_ColorSignature);
	DDX_Control(pDX, IDC_CHECK_BORDER, m_Check_Border);
}


BEGIN_MESSAGE_MAP(DSetup, CDialog)
END_MESSAGE_MAP()



void DSetup::DInit()
{

#ifndef DSETUP_INIT
	//Setup the max and min of the bar
	DSetup::m_Slider_CannyUnder.SetRangeMin(1);
	DSetup::m_Slider_CannyUnder.SetRangeMax(250);
	DSetup::m_Slider_CannyUpper.SetRangeMin(1);
	DSetup::m_Slider_CannyUpper.SetRangeMax(250);
	DSetup::m_Slider_ArcMin.SetRangeMin(1);
	DSetup::m_Slider_ArcMin.SetRangeMax(260);
	DSetup::m_Slider_ArcMax.SetRangeMin(1);
	DSetup::m_Slider_ArcMax.SetRangeMax(260);

	DSetup::m_Slider_AreaMin.SetRangeMin(1);
	DSetup::m_Slider_AreaMin.SetRangeMax(32000);

	DSetup::m_Slider_AreaMax.SetRangeMin(1);
	DSetup::m_Slider_AreaMax.SetRangeMax(32000);

	DSetup::m_Slider_Aproximate.SetRangeMin(1);
	DSetup::m_Slider_Aproximate.SetRangeMax(50);

	DSetup::m_Slider_ColorMatchTolerance.SetRangeMin(1);
	DSetup::m_Slider_ColorMatchTolerance.SetRangeMax(1000);
	
	DSetup::m_Combo_Resolution.AddString("480 x 360");
	DSetup::m_Combo_Resolution.AddString("640 x 480");
	DSetup::m_Combo_Resolution.AddString("800 x 600");
	DSetup::m_Combo_Resolution.AddString("1280 x 800");
	DSetup::m_Combo_Resolution.AddString("1280 x 1024");
	DSetup::m_Combo_Resolution.AddString("1600 x 1200");
	DSetup::m_Combo_Resolution.AddString("1920 x 1080");


#endif

}

// DSetup message handlers
