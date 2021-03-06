// MVSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVS.h"
#include "MVSDlg.h"

#include "highgui.h"
#include "cv.h"

//Ini config file read write
#include "iostream"
#include "IniWriter.h"
#include "IniReader.h"

//IP UDP send packet
#include <windows.h>
#include <stdio.h>
#include "winsock2.h"

//For modbus module
#include "MbusRtuMasterProtocol.hpp"
#include "MbusAsciiMasterProtocol.hpp"

#ifndef GLOBAL_VARIABLE
CRITICAL_SECTION m_CriticalSection;
CRITICAL_SECTION m_CriticalSection2;

//Thread related
char* TempChar; 

//Modbus PLC com
short readRegSet[20];
short writeRegSet[20];
int readBitSet[20];
int writeBitSet[20];

int i_for;
int result_radious_cam_1[10];
int result_radious_cam_2[10];
int result_center_x_cam_1[10];
int result_center_x_cam_2[10];
int result_center_y_cam_1[10];
int result_center_y_cam_2[10];

//Com flag
bool m_comSent_1;
bool m_comSent_2;

MbusAsciiMasterProtocol mbusProtocol;

//---------------OpenCV ---Init variable
IplImage* img = NULL;
//IplImage* frame_gray = NULL;
IplImage* cam1_img = NULL;
IplImage* cam2_img = NULL;

//Circle detection
CvMemStorage* Circle_storage;
CvMemStorage* Circle_storage_2;

//For knotch detection - input classifier
CvHaarClassifierCascade *cascade;
CvMemStorage            *Mem_storage;




int frame_Tick;
bool g_Lock = false;

bool g_draw = FALSE; //drawing using mouse trap
bool g_mouseDown = FALSE;
//CString CoordinateMouse_x,CoordinateMouse_y;

//Configuration parameter
CvPoint config_ROI_Top;
CvPoint config_ROI_Bottom;

//RS-232 Communication bit
  WSADATA wsaData;
  SOCKET SendSocket;
  sockaddr_in RecvAddr;
  int Port = 2000;
  char *SendBuf = new char[24];
  int BufLen = 24;

//Diaglog display string 
CString State_str;

//Debug font
CvFont DebugFont;

// CMVSDlg dialog
CString m_ipText;	//First camera IP address
CString m_ipText_2; //Second camera IP address

//Modbus comm setup
CString m_SerialPort;
CString m_SerialSpeed;


CString m_type = _T("mjpeg");

#endif
#pragma region SOME_CONSTANT
// How many iterations of erosion and/or dilation there should be
#define CVCLOSE_ITR  3
//Color define
#define CVX_RED				CV_RGB(0xff,0x00,0x00)
#define CVX_GREEN			CV_RGB(0x00,0xff,0x00)
#define CVX_BLUE			CV_RGB(0x00,0x00,0xff)
#define CVX_BLACK			CV_RGB(0x00,0x00,0x00)
#define CVX_WHITE			CV_RGB(0xff,0xff,0xff)
//---------------CPU - Processor variable
HANDLE myProc;
DWORD procAffinity, sytemAffinity;

#pragma endregion



#ifndef OPENCV_RELATED
/////////////////////////////////////////////////////////////////////////////
// CVideoDlg dialog
//Do Canny
//-------------------------OPENCV Function------------------

//Partially tested function - might still have bug to fix

CvScalar Sample_RGB(IplImage* Img)
{
	CvScalar s;

	IplImage* r = cvCreateImage( cvGetSize(Img), IPL_DEPTH_8U, 1 );
	IplImage* g = cvCreateImage( cvGetSize(Img), IPL_DEPTH_8U, 1 );
	IplImage* b = cvCreateImage( cvGetSize(Img), IPL_DEPTH_8U, 1 );
	// Split image onto the color planes.
	cvSplit( Img, r, g, b, NULL );
	// Temporary storage.
	
	s.val[0] = cvAvg(r).val[0];
	s.val[1] = cvAvg(g).val[0];
	s.val[2] = cvAvg(b).val[0];

	cvReleaseImage(&r);
	cvReleaseImage(&g);
	cvReleaseImage(&b);

	return(s);
};

void detectKnotch( IplImage *img, CvMemStorage* storage )
{
	int i;
	/* detect knotch */
	CvSeq *knotch = cvHaarDetectObjects(
		img,
		cascade,
		storage,
		1.1,
		3,
		0 /*CV_HAAR_DO_CANNY_PRUNNING*/,
		cvSize( 40, 40 ) );

	/* for each face found, draw a red box */
	for( i = 0 ; i < ( knotch ? knotch->total : 0 ) ; i++ ) {
		CvRect *r = ( CvRect* )cvGetSeqElem( knotch, i );
		cvRectangle( img,
			cvPoint( r->x, r->y ),
			cvPoint( r->x + r->width, r->y + r->height ),
			CV_RGB( 255, 0, 0 ), 1, 8, 0 );
	}

	/* display video */
	//cvShowImage( "SAMPLELOAD", img );
	//cvWaitKey(0);
}


//Define mouseCallBack Function 
void my_mouse_callback( int	event, int x, int y, int flags, void* param)
{
	
	IplImage* image = (IplImage*) param;
	//y = image->height - y;

		switch (event)
		{
		case CV_EVENT_MOUSEMOVE:
			{

			}
			break;

		case CV_EVENT_LBUTTONUP:
			{


			}
			break;
		case CV_EVENT_LBUTTONDOWN:
			{

			}
			break;
	
		}
}


//-------------------------Do Canny
IplImage* doCanny(
    IplImage* in,
    double    lowThresh,
    double    highThresh,
    double    aperture
) {
    if(in->nChannels != 1)
	{
        return (0); 
	}
    IplImage* out = cvCreateImage(
        cvSize( cvGetSize( in ).width, cvGetSize( in ).height),
        IPL_DEPTH_8U,   
        1
    );
    cvCanny( in, out, lowThresh, highThresh, aperture );
    return( out );
};

//

//-------------------------RGB sum and Threshold to binary
void equalize_rgb( IplImage* src, IplImage* dst, int lower, int upper ){
	// Allocate individual image planes.
	// Allocate individual image planes.
	IplImage* r = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
	IplImage* g = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
	IplImage* b = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
	IplImage* er = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
	IplImage* eg = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
	IplImage* eb = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );

	// Split image onto the color planes.
	cvSplit( src, r, g, b, NULL );
	//Equalise each image
	cvEqualizeHist (r,er);
	cvEqualizeHist (g,eg);
	cvEqualizeHist (b,eb);

	// Temporary storage.
	IplImage* s = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );


	// Add equally weighted rgb values.
	cvAddWeighted( er, 1./3., eg, 1./3., 0.0, s );
	cvAddWeighted( s, 2./3., eb, 1./3., 0.0, s );
	// Truncate values above 100.
	//Adaptive threshold
	//cvAdaptiveThreshold( s, dst, lower,  CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 3,3);
	cvThreshold( s, dst, lower, upper, CV_THRESH_BINARY_INV );

	cvReleaseImage( &r );
	cvReleaseImage( &g );
	cvReleaseImage( &b );
	cvReleaseImage( &er );
	cvReleaseImage( &eg );
	cvReleaseImage( &eb );
	cvReleaseImage( &s );
	
}

//-------------------------RGB sum and Threshold to binary
void sum_rgb( IplImage* src, IplImage* dst, int lower, int upper ) {
  // Allocate individual image planes.
 // Allocate individual image planes.
 IplImage* r = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
 IplImage* g = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
 IplImage* b = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
 // Split image onto the color planes.
 cvSplit( src, r, g, b, NULL );
 // Temporary storage.
  IplImage* s = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
  // Add equally weighted rgb values.
  cvAddWeighted( r, 1./3., g, 1./3., 0.0, s );
  cvAddWeighted( s, 2./3., b, 1./3., 0.0, s );
  // Truncate values above 100.
  cvThreshold( s, dst, lower, upper, CV_THRESH_BINARY_INV );
  //Adaptive threshold
  //cvAdaptiveThreshold( s, dst, lower,  CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 3,3);

  cvReleaseImage( &r );
  cvReleaseImage( &g );
  cvReleaseImage( &b );
  cvReleaseImage( &s );
}


//--------------------------Search for color in a specific range, filter out anything else -
void in_rang_filter(IplImage* src, IplImage* dst, CvScalar retain_color, int range_pc)
{
	

//IplImage* test=cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
CvScalar min_color = CV_RGB(retain_color.val[0] - (range_pc/2),
							retain_color.val[1] - (range_pc/2),
							retain_color.val[2] - (range_pc/2));
CvScalar max_color = CV_RGB(retain_color.val[0] + (range_pc/2),
							retain_color.val[1] + (range_pc/2),
							retain_color.val[2] + (range_pc/2));
cvInRangeS(src, min_color,max_color, dst);//search for the color in image

//cvCvtColor( imgResult, test,CV_GRAY2RGB);

}

int ColorMatchIndex (CvScalar Org, CvScalar Sample)
{
	int sum_r = 0;
	int sum_g = 0;
	int sum_b = 0;
	int delta = 0;
	sum_r = Org.val[0] - Sample.val[0];
	sum_g = Org.val[1] - Sample.val[1];
	sum_b = Org.val[2] - Sample.val[2];

	delta = sum_r + sum_g + sum_b;
	return delta;
}






/////////////////////
/////////////////////
/////////////////////
//-------------------------OPENCV Function------------------ END
//////////////////////////////////////////////////////////////////////////
void merge_hist_2D (CvHistogram* src_hist, CvHistogram* dst_hist)
{
	int h_bins;
	int s_bins;
	h_bins = src_hist->mat.dim[0].size;
	s_bins = src_hist->mat.dim[1].size;
	cvNormalizeHist( src_hist, 1.0 );  //Normalize it
	cvNormalizeHist( dst_hist, 1.0 );  //Normalize it

    for( int h = 0; h < h_bins; h++ ) {
        for( int s = 0; s < s_bins; s++ ) 
		{
			float bin_value_s = cvQueryHistValue_2D( src_hist, h, s );
			float bin_value_d = cvQueryHistValue_2D( dst_hist, h, s );
			bin_value_d = bin_value_d + bin_value_s;


			float* bin_set;
			bin_set = cvGetHistValue_2D(
						dst_hist,
						h,
						s);
			*bin_set = bin_value_d;
        }
	}

}


void show_hist_2D (CString WindowName, CvHistogram* hist, int scale)
{
	int h_bins;
	int s_bins;
	h_bins = hist->mat.dim[0].size;
	s_bins = hist->mat.dim[1].size;

        IplImage* hist_img = cvCreateImage( 
          cvSize( h_bins * scale, s_bins * scale ),
          8,
          3
        );
        cvZero( hist_img );
        // populate our visualization with little gray squares.
        //
        float max_value = 0;
        cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
        for( int h = 0; h < h_bins; h++ ) {
            for( int s = 0; s < s_bins; s++ ) 
			{
				float bin_value = cvQueryHistValue_2D( hist, h, s );

                int intensity = cvRound( bin_value * 255 / max_value );
                cvRectangle(
                  hist_img,
                  cvPoint( h*scale, s*scale ),
                  cvPoint( (h+1)*scale - 1, (s+1)*scale - 1),
                  CV_RGB(intensity,intensity,intensity),
                  CV_FILLED
                );
            }
        }
		cvShowImage(WindowName, hist_img );
		cvReleaseImage(&hist_img);

}

void get_hist_2D (CString PictureName, CvHistogram* hist)
{
	IplImage* sample_object;
	IplImage* sample_object_s;
	IplImage* sample_object_h;
	IplImage* sample_object_v;
	IplImage* sample_object_hvs;

	sample_object = cvLoadImage(PictureName);	
	
	cvCvtColor( sample_object, sample_object_hvs, CV_BGR2HSV );

	IplImage* planes[] = { sample_object_h, sample_object_s };
	//Histogram trial
	cvCvtPixToPlane( sample_object_hvs, sample_object_h, sample_object_s, sample_object_v, 0 );
	
	cvCalcHist( planes, hist, 0, 0 ); //Compute histogram

	cvReleaseImage(&sample_object);
	cvReleaseImage(&sample_object_s);
	cvReleaseImage(&sample_object_h);
	cvReleaseImage(&sample_object_v);
	cvReleaseImage(&sample_object_hvs);
}

double length_line (CvPoint p1, CvPoint p2)
{
	double l_x, l_y;
	l_x = abs(p1.x - p2.x);
	l_y = abs(p1.y - p2.y);
	return (sqrt(l_x*l_x + l_y*l_y));
}

CvPoint search_closest_point (CvSeq* contour, CvPoint Origine)
{
	double current_distance ;
	double last_distance;

	CvPoint Current;

	//First point
	CvPoint* p = (CvPoint*)cvGetSeqElem ( contour, 0 );

	last_distance = length_line(Origine,*p);
	Current = *p;

	for( int i=0; i<contour->total; ++i ) 
	{
		CvPoint* p2 = (CvPoint*)cvGetSeqElem ( contour, 0 );
		current_distance = length_line(Origine,*p2);
		if (current_distance <= last_distance)
		{
			Current = *p2;
		}
	}
	return (Current);

}

double distance_3p (CvPoint p1, CvPoint p2, CvPoint org)
{
	double distance_total;
	CvPoint Middle;
	Middle.x = (p1.x + p2.x)/2;
	Middle.y = (p1.y + p2.y)/2;

	
	distance_total = length_line(p1,org);
	distance_total = distance_total + length_line(p2,org);
	distance_total = distance_total + length_line(Middle,org);
	distance_total = distance_total / 3;
	return (distance_total);
}

void search_line (CvSeq* contour, CvPoint* p_1, CvPoint* p_2, int type)
{
	CvPoint Origine;
	Origine.x = 0;
	Origine.y = 0;

	double current_distance;
	double temp_distance;
	CvPoint Current;
	Current.x = 0;
	Current.y = 0;


	for( int i=0; i< (contour->total - 1); ++i ) 
	{
		
		CvPoint* p1 = (CvPoint*)cvGetSeqElem ( contour, i );
		CvPoint* p2 = (CvPoint*)cvGetSeqElem ( contour, i + 1 );
		if (i == 0)
		{
			current_distance = distance_3p(*p1,*p2,Origine);
			p_1 = p1;
			p_2 = p2;

		}
		else
		{
			temp_distance = distance_3p(*p1,*p2,Origine);
			if (current_distance > temp_distance)
			{
				current_distance = temp_distance;
				p_1 = p1;
				p_2 = p2;
			}
		}

	}
								
}
void search_top2p (CvSeq* contour, CvPoint* p_1, CvPoint* p_2)
{
	CvPoint first, second;
	int first_id;

	for( int i=0; i< contour->total ; ++i ) 
	{
		if (i ==0)
		{
			CvPoint* p1 = (CvPoint*)cvGetSeqElem ( contour, i );
			first.x = p1->x;
			first.y = p1->y;
			first_id = i;
		}
		else
		{
			CvPoint* p1 = (CvPoint*)cvGetSeqElem ( contour, i );
			if (p1->y < first.y)
			{
				first.x = p1->x;
				first.y = p1->y;
				first_id = i;
			}
		}	
	}

	//Compare get 2 side point
	CvPoint* forward_p;
	CvPoint* backward_p;

	if (first_id == 0)
	{
		 forward_p = (CvPoint*)cvGetSeqElem ( contour, first_id + 1 );
		 backward_p = (CvPoint*)cvGetSeqElem ( contour, (contour->total - 1) );
	}
	else if (first_id == (contour->total - 1))
	{
		 forward_p = (CvPoint*)cvGetSeqElem ( contour, 0 );
		 backward_p = (CvPoint*)cvGetSeqElem ( contour, (first_id - 1) );
	}
	else
	{
		 forward_p = (CvPoint*)cvGetSeqElem ( contour, (first_id + 1 ));
		 backward_p = (CvPoint*)cvGetSeqElem ( contour, (first_id - 1) );
	}
	//Then compare them
	if (forward_p->y < backward_p->y )
	{
		second.x = forward_p->x;
		second.y = forward_p->y;
	}
	else
	{
		second.x = backward_p->x;
		second.y = backward_p->y;
	}

	*p_1 = first;
	*p_2 = second;
}

#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif




#ifndef INTERNAL_FUNCTION
bool CMVSDlg::SaveConfig (char *FileName)
{
	
	CIniWriter iniWriter(FileName);
	
	char *szIP = m_ipText.GetBuffer(m_ipText.GetLength());
	char *szIP_2 = m_ipText_2.GetBuffer(m_ipText_2.GetLength());
	CString TextComIP;
	m_Com.m_Edit_Ip.GetWindowText(TextComIP); 
	char *szIP_Com = TextComIP.GetBuffer(TextComIP.GetLength());

	CString TextComIP_Port;
	m_Com.m_Edit_Port.GetWindowText(TextComIP_Port); 
	char *szIP_Com_Port = TextComIP_Port.GetBuffer(TextComIP_Port.GetLength());

	iniWriter.WriteString("Setting", "IP", szIP);   
	iniWriter.WriteString("Setting", "IP_2", szIP_2);   
	iniWriter.WriteString("Setting", "IP_Com", szIP_Com);   
	iniWriter.WriteString("Setting", "IP_Com_Port", szIP_Com_Port);
	iniWriter.WriteString("Setting", "State", "Undo");
	iniWriter.WriteInteger("Setting", "CannyUnder", m_Setup.m_Slider_CannyUnder.GetPos()); 
	iniWriter.WriteInteger("Setting", "CannyUpper", m_Setup.m_Slider_CannyUpper.GetPos()); 
	iniWriter.WriteInteger("Setting", "ArcLengthMin", m_Setup.m_Slider_ArcMin.GetPos()); 
	iniWriter.WriteInteger("Setting", "ArcLengthMax", m_Setup.m_Slider_ArcMax.GetPos()); 
	iniWriter.WriteInteger("Setting", "AreaMin", m_Setup.m_Slider_AreaMin.GetPos()); 
	iniWriter.WriteInteger("Setting", "AreaMax",m_Setup.m_Slider_AreaMax.GetPos()); 
	iniWriter.WriteInteger("Setting", "Aproximate",m_Setup.m_Slider_Aproximate.GetPos()); 
	iniWriter.WriteInteger("Setting", "ColorTolerance",m_Setup.m_Slider_ColorMatchTolerance.GetPos()); 
	iniWriter.WriteInteger("Setting", "Resolution",m_Setup.m_Combo_Resolution.GetCurSel()); 
	iniWriter.WriteBoolean("Setting", "Check_Canny",m_Setup.m_Check_Canny.GetCheck());
	iniWriter.WriteBoolean("Setting", "Check_Contour",m_Setup.m_Check_Contour.GetCheck());
	iniWriter.WriteBoolean("Setting", "Check_AOI",m_Setup.m_Check_Aoi.GetCheck());
	iniWriter.WriteBoolean("Setting", "Check_ColorMatching",m_Setup.m_Check_ColorMatching.GetCheck());
	iniWriter.WriteBoolean("Setting", "Check_Histogram",m_Setup.m_Check_Histogram.GetCheck());
	iniWriter.WriteBoolean("Setting", "Check_ReferenceLine",m_Setup.m_Check_ReferenceLine.GetCheck());
	iniWriter.WriteBoolean("Setting", "Check_Equalise",m_Setup.m_Check_Equalize.GetCheck());
	iniWriter.WriteBoolean("Setting", "Check_Approximate",m_Setup.m_Check_Aproximate.GetCheck());
	iniWriter.WriteBoolean("Setting", "Check_LogImage",m_Setup.m_Check_LogImage.GetCheck());
	iniWriter.WriteBoolean("Setting", "Check_ComUDP",m_Com.m_Check_Udp.GetCheck());
	iniWriter.WriteBoolean("Setting", "Check_ColorSignature",m_Setup.m_Check_ColorSignature.GetCheck());
	iniWriter.WriteBoolean("Setting", "Check_Border",m_Setup.m_Check_Border.GetCheck());
	iniWriter.WriteInteger("Setting", "ROI_Top_x",m_Setup.ROI_Top.x);
	iniWriter.WriteInteger("Setting", "ROI_Top_y",m_Setup.ROI_Top.y);
	iniWriter.WriteInteger("Setting", "ROI_Bottom_x",m_Setup.ROI_Bottom.x);
	iniWriter.WriteInteger("Setting", "ROI_Bottom_y",m_Setup.ROI_Bottom.y);

	return (true);
}

bool CMVSDlg::LoadConfig (char *FileName)
{

	CIniReader iniReader(FileName);

	char *szIP = m_ipText.GetBuffer(m_ipText.GetLength());
	char *szIP_2 = m_ipText_2.GetBuffer(m_ipText_2.GetLength());
	
	m_ipText.Format(iniReader.ReadString ("Setting","IP", "192.168.1.1"));
	m_Edit_IpAddress.SetWindowTextA(m_ipText);
	m_ipText_2.Format(iniReader.ReadString ("Setting","IP_2", "192.168.1.2"));
	m_Edit_IpAddress_2.SetWindowTextA(m_ipText_2);

	m_SerialPort.Format(iniReader.ReadString ("Setting","SerialPort", "COM5"));
	m_SerialSpeed.Format(iniReader.ReadString ("Setting","SerialSpeed", "9600"));


	CString Com_IP;
	Com_IP.Format(iniReader.ReadString ("Setting","IP_Com", "192.168.1.1"));
	m_Com.m_Edit_Ip.SetWindowTextA(Com_IP);

	CString Com_IP_Port;
	Com_IP_Port.Format(iniReader.ReadString ("Setting","IP_Com_Port", "192.168.1.1"));
	m_Com.m_Edit_Port.SetWindowTextA(Com_IP_Port);

	State_str = iniReader.ReadString("Setting","State","unknow");
	m_Setup.m_Slider_CannyUnder.SetPos(iniReader.ReadInteger("Setting", "CannyUnder",1)); 
	m_Setup.m_Slider_CannyUpper.SetPos(iniReader.ReadInteger("Setting", "CannyUpper",1)); 
	m_Setup.m_Slider_ArcMin.SetPos(iniReader.ReadInteger("Setting", "ArcLengthMin",1)); 
	m_Setup.m_Slider_ArcMax.SetPos(iniReader.ReadInteger("Setting", "ArcLengthMax",1)); 
	m_Setup.m_Slider_AreaMin.SetPos(iniReader.ReadInteger("Setting", "AreaMin",1)); 
	m_Setup.m_Slider_AreaMax.SetPos(iniReader.ReadInteger("Setting", "AreaMax",1)); 
	m_Setup.m_Slider_Aproximate.SetPos(iniReader.ReadInteger("Setting", "Aproximate",1));
	m_Setup.m_Slider_ColorMatchTolerance.SetPos(iniReader.ReadInteger("Setting", "ColorTolerance",1));
	m_Setup.m_Combo_Resolution.SetCurSel(iniReader.ReadInteger("Setting", "Resolution",1));
	m_Setup.m_Check_Canny.SetCheck(iniReader.ReadBoolean("Setting", "Check_Canny",FALSE));
	m_Setup.m_Check_Contour.SetCheck(iniReader.ReadBoolean("Setting", "Check_Contour",FALSE));
	m_Setup.m_Check_Aoi.SetCheck(iniReader.ReadBoolean("Setting", "Check_AOI",FALSE));
	m_Setup.m_Check_ColorMatching.SetCheck(iniReader.ReadBoolean("Setting", "Check_ColorMatching",FALSE));
	m_Setup.m_Check_Histogram.SetCheck(iniReader.ReadBoolean("Setting", "Check_Histogram",FALSE));
	m_Setup.m_Check_ReferenceLine.SetCheck(iniReader.ReadBoolean("Setting", "Check_ReferenceLine",FALSE));
	m_Setup.m_Check_Equalize.SetCheck(iniReader.ReadBoolean("Setting", "Check_Equalise",FALSE));
	m_Setup.m_Check_Aproximate.SetCheck(iniReader.ReadBoolean("Setting", "Check_Approximate",FALSE));
	m_Setup.m_Check_LogImage.SetCheck(iniReader.ReadBoolean("Setting", "Check_LogImage",FALSE));
	m_Setup.m_Check_ColorSignature.SetCheck(iniReader.ReadBoolean("Setting", "Check_ColorSignature",FALSE));
	m_Setup.m_Check_Border.SetCheck(iniReader.ReadBoolean("Setting", "Check_Border",FALSE));
	m_Com.m_Check_Udp.SetCheck(iniReader.ReadBoolean("Setting", "Check_ComUDP",FALSE));
	m_Setup.ROI_Top.x = iniReader.ReadInteger("Setting", "ROI_Top_x",0);
	m_Setup.ROI_Top.y = iniReader.ReadInteger("Setting", "ROI_Top_y",0);
	m_Setup.ROI_Bottom.x = iniReader.ReadInteger("Setting", "ROI_Bottom_x",0);
	m_Setup.ROI_Bottom.y = iniReader.ReadInteger("Setting", "ROI_Bottom_y",0);


	return (true);
}


bool CMVSDlg::LogImage (CString LogFolder,CString FileName, IplImage* img)
{

    SYSTEMTIME st;
    //GetSystemTime(&st);
	GetLocalTime(&st);

	CString FilePath;
	//Get time
	CString str_Hour;
	CString str_Min;
	CString str_Second;
	
	str_Hour.Format("%d",st.wHour);
	str_Min.Format("%d",st.wMinute);
	str_Second.Format("%d",st.wSecond);

	//Auto create folder if itsnt there
	CreateDirectory(LogFolder, NULL);

	FilePath = LogFolder + FileName + str_Hour + str_Min + str_Second + ".jpg";
	cvSaveImage(FilePath, img);
	return(true);
}



bool CMVSDlg::InitOpenCv (void)
{

#ifndef INIT_OPENCV_F
	
	//-------------Init OpenCV-------------------
	//Prepare classifier ready for detecting knotch
	//cascade = ( CvHaarClassifierCascade* )cvLoad("knot_classifier.xml", 0, 0, 0 );

	//cvNamedWindow("IPC", 1);
	
	/*
	cvSetMouseCallback (
		"IPC",
		my_mouse_callback,
		(void*) frame_select);
	*/
#pragma region SELECT_CPU
	//----------------Init process---------------
	myProc = GetCurrentProcess();
	if (GetProcessAffinityMask(myProc, &procAffinity, &sytemAffinity))
	{
		if (sytemAffinity > 1)
		{
		procAffinity ^= 1;
		SetProcessAffinityMask(myProc, procAffinity);
		}
	}
#pragma endregion

	//Display init font 
	cvInitFont( &DebugFont,
				CV_FONT_HERSHEY_SIMPLEX,
				1.0,
				1.0,
				0,
				1
				);

#endif
	return (true);
}

bool CMVSDlg::SendCom(CString str_Data)
{
	if (m_Com.m_Check_Udp.GetCheck())
	{
		if (str_Data.GetAllocLength() < 24)
		{
			//SendBuf[1] = 's';
			//SendBuf[2] = 's';
			//Flush the send buffer
			memset(SendBuf, 0, sizeof(SendBuf));
			strcpy(SendBuf,str_Data);
			sendto(SendSocket, 
				SendBuf, 
				BufLen, 
				0, 
				(SOCKADDR *) &RecvAddr, 
				sizeof(RecvAddr));
		}
	}
	return (true);
}
#endif


CMVSDlg::CMVSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMVSDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMVSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AMC, m_AMC);
	DDX_Control(pDX, IDC_EDIT_IPADD, m_Edit_IpAddress);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_EditStatus);
	DDX_Control(pDX, IDC_AMC2, m_AMC_2);
	DDX_Control(pDX, IDC_EDIT_IPADD2, m_Edit_IpAddress_2);
}

BEGIN_MESSAGE_MAP(CMVSDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_CONFIGURE_IMAGEPROCESS, &CMVSDlg::OnConfigureImageprocess)
	ON_BN_CLICKED(IDCANCEL, &CMVSDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_SAMPLE, &CMVSDlg::OnBnClickedSample)
	ON_BN_CLICKED(IDOK, &CMVSDlg::OnBnClickedOk)
	ON_COMMAND(ID_COM_SETUP, &CMVSDlg::OnComSetup)
END_MESSAGE_MAP()


// CMVSDlg message handlers

#pragma region THREAD_RELATED
UINT ImageProcess_Thread_1( LPVOID pParam ) 
{
	cvNamedWindow("CAM_1", 1);
	IplImage* frame_img = NULL;
	
	IplImage* frame_gray = NULL;
	IplImage* frame_filtered = NULL;


	while(true)
	{	

		EnterCriticalSection(&m_CriticalSection);
		if (cam1_img != NULL)
		{
			frame_img = cvCloneImage(cam1_img);
		}
		LeaveCriticalSection(&m_CriticalSection);
//#define TRIAL


		if (frame_img != NULL)
		{

#ifdef TRIAL2
		//Initialise all the sub frame
		if (frame_gray == NULL)
		{
			frame_gray = cvCreateImage(cvSize(frame_img->width,frame_img->height),IPL_DEPTH_8U,1);	
		}

		//Convert into gray
		cvCvtColor(frame_img, frame_gray, CV_RGB2GRAY);

		cvSetImageROI(frame_gray, cvRect(0,100,640,250));

		cvSmooth (frame_gray,frame_gray, CV_MEDIAN, 3, 3);


		//For Circle detection
		Circle_storage = cvCreateMemStorage(0);


		CvSeq* results = cvHoughCircles(
			frame_gray,
			Circle_storage,
			CV_HOUGH_GRADIENT,
			2,
			frame_gray->width/10,
			100, // Upper canny
			60,
			80,
			10
			);

		//Draw the founded circle
		for( int i = 0; i < results->total; i++ ) {
			float* p = (float*) cvGetSeqElem( results, i );
			CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
			cvCircle(
				frame_gray,
				pt,
				cvRound( p[2] ),
				CV_RGB(0xff,0x21,0x11)
				);
		}



			
			cvShowImage("CAM_1", frame_gray);
			
			cvResetImageROI (frame_gray);
			//Release AMC varian

			cvReleaseMemStorage(&Circle_storage);
#endif

			//Initialise all the sub frame
			if (frame_gray == NULL)
			{
				frame_gray = cvCreateImage(cvSize(frame_img->width,frame_img->height),IPL_DEPTH_8U,1);	
			}

			//Convert into gray
			cvCvtColor(frame_img, frame_gray, CV_RGB2GRAY);

			cvSetImageROI(frame_gray, cvRect(0,100,640,250));

			cvSmooth (frame_gray,frame_gray, CV_MEDIAN, 3, 3);


			//----------------------Circle detection -----------------------

			//For Circle detection
			Circle_storage = cvCreateMemStorage(0);


			CvSeq* results = cvHoughCircles(
				frame_gray,
				Circle_storage,
				CV_HOUGH_GRADIENT,
				2,
				frame_gray->width/10,
				100,//100, // Upper canny
				200,
				0,
				100
				);

			//Draw the founded circle
			for( int i = 0; i < results->total; i++ ) {
				float* p = (float*) cvGetSeqElem( results, i );
				CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
				cvCircle(
					frame_gray,
					pt,
					cvRound( p[2] ),
					CV_RGB(0xff,0x21,0x11)
					);
			//Write to the com buffer
			EnterCriticalSection(&m_CriticalSection2);
			for( int i = 0; i < 10; i++ ) {
				if (m_comSent_1)
				{
					result_radious_cam_1[i] = 0;
					result_center_x_cam_1[i] = 0;
					result_center_y_cam_1[i] = 0;
				}

				if (i < results->total)
				{
					float* p = (float*) cvGetSeqElem( results, i );					
					result_radious_cam_1[i] = cvRound( p[2] );
					result_center_x_cam_1[i] = cvRound( p[0] );
					result_center_y_cam_1[i] = cvRound( p[1] );
				}
				
			}
			LeaveCriticalSection(&m_CriticalSection2);
			}

			//----------------------Circle detection ----end----------------


			cvShowImage("CAM_1",frame_gray);
			cvReleaseImage(&frame_img);
			cvReleaseImage(&frame_gray);
			cvReleaseMemStorage(&Circle_storage);
			Sleep(30);
			cvWaitKey(1);
			
		}
		

	}

	return 0; 
}

UINT ImageProcess_Thread_2( LPVOID pParam ) 
{
	//Internal variable ----------------------------------------
	CvCapture* capture;
	IplImage* frame_img = NULL;

	IplImage* frame_gray = NULL;
	IplImage* frame_filtered = NULL;
	
	cvNamedWindow("CAM_2", 1 );
	while(true)
	{

		EnterCriticalSection(&m_CriticalSection);
		if (cam2_img != NULL)
		{
			frame_img = cvCloneImage(cam2_img);
		}
		LeaveCriticalSection(&m_CriticalSection);

		if (frame_img != NULL)
		{
			//Initialise all the sub frame
			if (frame_gray == NULL)
			{
				frame_gray = cvCreateImage(cvSize(frame_img->width,frame_img->height),IPL_DEPTH_8U,1);	
			}

			//Convert into gray
			cvCvtColor(frame_img, frame_gray, CV_RGB2GRAY);

			cvSetImageROI(frame_gray, cvRect(0,100,640,250));

			cvSmooth (frame_gray,frame_gray, CV_MEDIAN, 3, 3);
			//----------------------Circle detection -----------------------

			//For Circle detection
			Circle_storage_2 = cvCreateMemStorage(0);


			CvSeq* results = cvHoughCircles(
				frame_gray,
				Circle_storage_2,
				CV_HOUGH_GRADIENT,
				2,
				frame_gray->width/10,
				100, // Upper canny
				200,
				0,
				100
				);

			//Draw the founded circle
			for( int i = 0; i < results->total; i++ ) {
				float* p = (float*) cvGetSeqElem( results, i );
				CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
				cvCircle(
					frame_gray,
					pt,
					cvRound( p[2] ),
					CV_RGB(0xff,0x21,0x11)
					);
			}

			//Write to the com buffer
			EnterCriticalSection(&m_CriticalSection2);
			for( int i = 0; i < 10; i++ ) {
				if (m_comSent_2)
				{
					result_radious_cam_2[i] = 0;
					result_center_x_cam_2[i] = 0;
					result_center_y_cam_2[i] = 0;
				}
				if (i < results->total)
				{
					float* p = (float*) cvGetSeqElem( results, i );					
					result_radious_cam_2[i] = cvRound( p[2] );
					result_center_x_cam_2[i] = cvRound( p[0] );
					result_center_y_cam_2[i] = cvRound( p[1] );
				}

			}
			LeaveCriticalSection(&m_CriticalSection2);


			//----------------------Circle detection ----end----------------

		}
		//Show result image
		cvShowImage("CAM_2",frame_gray);
		cvReleaseImage(&frame_img);
		cvReleaseImage(&frame_gray);
		cvReleaseMemStorage(&Circle_storage_2);
		Sleep(30);
		cvWaitKey(1);
	}


	return 0;
}

//Input address in decimal - then function would convert it to PLC address
int D_AddressRef (int d_Address)
{
	int d_working = 4096;
	d_working = d_working + d_Address + 1;
	return (d_working);
}


UINT Com_Thread_1( LPVOID pParam ) 
{
	int ReadyToRecieve;
	int WriteAddress_PLC;
	int result_write_PLC;
	
	short Data_Arr_Cam1_Radius[10];
	short Data_Arr_Cam1_x[10];
	short Data_Arr_Cam1_y[10];
	short Data_Arr_Cam2_Radius[10];
	short Data_Arr_Cam2_x[10];
	short Data_Arr_Cam2_y[10];






	while(true)
	{

		WriteAddress_PLC = 0x0600;

			
		//Read from com buffer then send to PLC
		//EnterCriticalSection(&m_CriticalSection2);		
		//Move data from one array to the other 
		for (i_for = 0; i_for <10; i_for++)
		{
			
			
			Data_Arr_Cam1_Radius[i_for] = result_radious_cam_1[i_for];
			Data_Arr_Cam2_Radius[i_for] = result_radious_cam_2[i_for];
			Data_Arr_Cam1_x[i_for] = result_center_x_cam_1[i_for];
			Data_Arr_Cam2_x[i_for] = result_center_x_cam_2[i_for];
			Data_Arr_Cam1_y[i_for] = result_center_y_cam_1[i_for];
			Data_Arr_Cam2_y[i_for] = result_center_y_cam_2[i_for];
			

			/*
			Data_Arr_Cam1_Radius[i_for] = i_for;
			Data_Arr_Cam2_Radius[i_for] = i_for + 10;
			Data_Arr_Cam1_x[i_for] = i_for + 100;
			Data_Arr_Cam2_x[i_for] = i_for + 110;
			Data_Arr_Cam1_y[i_for] = i_for + 200;
			Data_Arr_Cam2_y[i_for] = i_for + 210;
			*/
		}

		//LeaveCriticalSection(&m_CriticalSection2);
		/*
		if (Data_Arr_Cam2_Radius[0] != 0)
		{
			mbusProtocol.writeSingleRegister(1, 0x1263, Data_Arr_Cam2_Radius[0]);
			mbusProtocol.writeSingleRegister(1, 0x1265, Data_Arr_Cam2_x[0]);
			mbusProtocol.writeSingleRegister(1, 0x1267, Data_Arr_Cam2_y[0]);
		}

		if (Data_Arr_Cam1_Radius[0] != 0)
		{
			mbusProtocol.writeSingleRegister(1, 0x1259, Data_Arr_Cam1_Radius[0]);
			mbusProtocol.writeSingleRegister(1, 0x125B, Data_Arr_Cam1_x[0]);
			mbusProtocol.writeSingleRegister(1, 0x125D, Data_Arr_Cam1_y[0]);
		}
		*/
		//mbusProtocol.writeSingleRegister(1, D_AddressRef(700), 0x234);

		mbusProtocol.writeSingleRegister(1, D_AddressRef(600), Data_Arr_Cam1_Radius[0]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(602), Data_Arr_Cam1_x[0]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(604), Data_Arr_Cam1_y[0]);

		mbusProtocol.writeSingleRegister(1, D_AddressRef(606), Data_Arr_Cam1_Radius[1]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(608), Data_Arr_Cam1_x[1]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(610), Data_Arr_Cam1_y[1]);
		
		mbusProtocol.writeSingleRegister(1, D_AddressRef(612), Data_Arr_Cam1_Radius[2]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(614), Data_Arr_Cam1_x[2]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(616), Data_Arr_Cam1_y[2]);
		
		mbusProtocol.writeSingleRegister(1, D_AddressRef(618), Data_Arr_Cam1_Radius[3]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(620), Data_Arr_Cam1_x[3]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(622), Data_Arr_Cam1_y[3]);

		mbusProtocol.writeSingleRegister(1, D_AddressRef(624), Data_Arr_Cam1_Radius[4]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(626), Data_Arr_Cam1_x[4]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(628), Data_Arr_Cam1_y[4]);

		mbusProtocol.writeSingleRegister(1, D_AddressRef(630), Data_Arr_Cam2_Radius[0]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(632), Data_Arr_Cam2_x[0]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(634), Data_Arr_Cam2_y[0]);

		mbusProtocol.writeSingleRegister(1, D_AddressRef(636), Data_Arr_Cam2_Radius[1]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(638), Data_Arr_Cam2_x[1]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(640), Data_Arr_Cam2_y[1]);
		
		mbusProtocol.writeSingleRegister(1, D_AddressRef(642), Data_Arr_Cam2_Radius[2]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(644), Data_Arr_Cam2_x[2]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(646), Data_Arr_Cam2_y[2]);
		
		mbusProtocol.writeSingleRegister(1, D_AddressRef(648), Data_Arr_Cam2_Radius[3]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(650), Data_Arr_Cam2_x[3]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(652), Data_Arr_Cam2_y[3]);

		mbusProtocol.writeSingleRegister(1, D_AddressRef(654), Data_Arr_Cam2_Radius[4]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(656), Data_Arr_Cam2_x[4]);
		mbusProtocol.writeSingleRegister(1, D_AddressRef(658), Data_Arr_Cam2_y[4]);


		//Address 0x1000 stand for D register in PLC
		//Address 0x0258 stand for 600 in decimal
		//So to write to D600 register in the PLC 
		//The reference address is 0x1258

		//Sleep(3000);
		
		//mbusProtocol.readRegisters(ReadyToRecieve,0x0601);
		/*
		result_write_PLC = mbusProtocol.writeMultipleRegisters(1, 0x1064, Data_Arr_Cam1_Radius, sizeof(Data_Arr_Cam1_Radius)/2);
		result_write_PLC = mbusProtocol.writeMultipleRegisters(1, 0x10C8, Data_Arr_Cam2_Radius, sizeof(Data_Arr_Cam2_Radius)/2);
		result_write_PLC = mbusProtocol.writeMultipleRegisters(1, 0x1078, Data_Arr_Cam1_x, sizeof(Data_Arr_Cam1_Radius)/2);
		result_write_PLC = mbusProtocol.writeMultipleRegisters(1, 0x10DC, Data_Arr_Cam2_x, sizeof(Data_Arr_Cam2_Radius)/2);
		result_write_PLC = mbusProtocol.writeMultipleRegisters(1, 0x108C, Data_Arr_Cam1_y, sizeof(Data_Arr_Cam1_Radius)/2);
		result_write_PLC = mbusProtocol.writeMultipleRegisters(1, 0x10F0, Data_Arr_Cam2_y, sizeof(Data_Arr_Cam2_Radius)/2);
		*/
		m_comSent_1 = true;
		m_comSent_2 = true;
		Sleep(500);
	}
	return 0;
}
#pragma endregion


BOOL CMVSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_Setup.Create(IDD_DSETUP,this);
	m_Com.Create(IDD_DCOM,this);
	m_Setup.DInit();

	LoadConfig(".\\config.ini");

#pragma region MODBUS_INIT

	//Modbus Init
	CString portName;
	portName = m_SerialPort;
	long l_baudrate;
	if (m_SerialSpeed == "9600")
	{l_baudrate = 9600L;}
	if (m_SerialSpeed == "14400")
	{l_baudrate = 14400L;}
	if (m_SerialSpeed == "19200")
	{l_baudrate = 19200L;}
	if (m_SerialSpeed == "38400")
	{l_baudrate = 38400L;}

	int result;
	int Counter = 1234;
	//mbusProtocol.closeProtocol();
	result = mbusProtocol.openProtocol(portName,
		l_baudrate, // Baudrate
		7, // Databits
		1, // Stopbits
		2); // Parity
	if (result != FTALK_SUCCESS)
	{
		CString displaytext;
		displaytext.Format("%.1f",getBusProtocolErrorText(result));
		displaytext = "Fail to open Com port 5 !";
		
		m_EditStatus.SetWindowText(displaytext);
		
	}
	else
	{
		m_EditStatus.SetWindowText("Com port 5 open successfully");
		AfxBeginThread(Com_Thread_1, TempChar,THREAD_PRIORITY_NORMAL); 
	}




#pragma endregion

#ifndef INIT_SOCKET
	//Create com connection
	//---------------------------------------------
	// Initialize Winsock
	WSAStartup(MAKEWORD(2,2), &wsaData);

	//---------------------------------------------
	// Create a socket for sending data
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//---------------------------------------------
	// Set up the RecvAddr structure with the IP address of
	// the receiver (in this example case "123.456.789.1")
	// and the specified port number.
	RecvAddr.sin_family = AF_INET;
	//Init the port number
	CString TextPort;
	m_Com.m_Edit_Port.GetWindowText(TextPort); 
	//Convert string to int
	Port = _ttoi((LPCTSTR)TextPort);
	RecvAddr.sin_port = htons(Port);
	//Init IP
	CString TextIP;
	m_Com.m_Edit_Ip.GetWindowText(TextIP); 
	//IP
	RecvAddr.sin_addr.s_addr = inet_addr(TextIP);

#endif



	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalSection2);
	//Init a new thread
	//--Start your thread (in this case, LeesThread) 
	AfxBeginThread(ImageProcess_Thread_1, TempChar,THREAD_PRIORITY_LOWEST); 
	AfxBeginThread(ImageProcess_Thread_2, TempChar,THREAD_PRIORITY_LOWEST); 
	
	//InitOpenCv();	

#pragma region AMC_1_SETUP

  CString anURL = m_ipText;

  // Determine protocol to use if not set
  if (m_ipText.Find((CString)"://") == -1)
  {
      anURL = (CString)"http://" + m_ipText;
  }

   // complete URL
  if (!(anURL[anURL.GetLength() - 1] == '/'))
  {
    anURL += (CString)"/";
  }
  if (m_type.CompareNoCase((CString)"mjpeg") == 0)
  {
    anURL = anURL + (CString)"axis-cgi/mjpg/video.cgi";
  }
  else if (m_type.CompareNoCase((CString)"mpeg4") == 0)
  {
    anURL = anURL + (CString)"mpeg4/media.amp";
  }
  else if (m_type.CompareNoCase((CString)"h264") == 0)
  {
    anURL = anURL + (CString)"axis-media/media.amp?videocodec=h264";
  }
  else if (m_type.CompareNoCase((CString)"mpeg2-unicast") == 0)
  {
    anURL = anURL + (CString)"axis-cgi/mpeg2/video.cgi";
  }
  else if (m_type.CompareNoCase((CString)"mpeg2-multicast") == 0)
  {
    anURL = anURL + (CString)"axis-cgi/mpeg2/video.cgi";
  }
	// TODO: Add extra initialization here
	try
	{
		m_AMC.put_ShowStatusBar(true);
		//m_AMC.put_MediaUsername(m_user);
		//m_AMC.put_MediaPassword(m_pass);

		// Set the media URL and the media type
		m_AMC.put_MediaURL(anURL);
		m_AMC.put_MediaType(m_type);
		m_AMC.put_StretchToFit(true);
		m_AMC.put_EnableReconnect(FALSE);

	}
	catch (COleDispatchException *e)
	{
		MessageBox(e->m_strDescription);
	}	

	try
	{
		// Starts the download of the mpeg4 stream from the Axis camera/video server
		m_AMC.Play(); 
		
	}
	catch (COleDispatchException *e)
	{
		if (e->m_scError == E_INVALIDARG)
		{
			MessageBox((CString)"Invalid parameters.");
		}
		else if (e->m_scError == E_ACCESSDENIED)
		{
			MessageBox((CString)"Access denied.");
		}
		else
		{
			MessageBox(e->m_strDescription);
		}
	}	

#pragma endregion

#pragma region AMC_2_SETUP
	
	//CString m_ipText_2 = "192.168.2.3";
	CString anURL_2 = m_ipText_2;

	// Determine protocol to use if not set
	if (m_ipText_2.Find((CString)"://") == -1)
	{
		anURL_2 = (CString)"http://" + m_ipText_2;
	}

	// complete URL
	if (!(anURL_2[anURL_2.GetLength() - 1] == '/'))
	{
		anURL_2 += (CString)"/";
	}
	if (m_type.CompareNoCase((CString)"mjpeg") == 0)
	{
		anURL_2 = anURL_2 + (CString)"axis-cgi/mjpg/video.cgi";
	}
	else if (m_type.CompareNoCase((CString)"mpeg4") == 0)
	{
		anURL_2 = anURL_2 + (CString)"mpeg4/media.amp";
	}
	else if (m_type.CompareNoCase((CString)"h264") == 0)
	{
		anURL_2 = anURL_2 + (CString)"axis-media/media.amp?videocodec=h264";
	}
	else if (m_type.CompareNoCase((CString)"mpeg2-unicast") == 0)
	{
		anURL_2 = anURL_2 + (CString)"axis-cgi/mpeg2/video.cgi";
	}
	else if (m_type.CompareNoCase((CString)"mpeg2-multicast") == 0)
	{
		anURL_2 = anURL_2 + (CString)"axis-cgi/mpeg2/video.cgi";
	}
	// TODO: Add extra initialization here
	try
	{
		m_AMC_2.put_ShowStatusBar(true);
		//m_AMC.put_MediaUsername(m_user);
		//m_AMC.put_MediaPassword(m_pass);

		// Set the media URL and the media type
		m_AMC_2.put_MediaURL(anURL_2);
		m_AMC_2.put_MediaType(m_type);
		m_AMC_2.put_StretchToFit(true);
		m_AMC_2.put_EnableReconnect(FALSE);

	}
	catch (COleDispatchException *e)
	{
		MessageBox(e->m_strDescription);
	}	

	try
	{
		// Starts the download of the mpeg4 stream from the Axis camera/video server
		m_AMC_2.Play(); 

	}
	catch (COleDispatchException *e)
	{
		if (e->m_scError == E_INVALIDARG)
		{
			MessageBox((CString)"Invalid parameters.");
		}
		else if (e->m_scError == E_ACCESSDENIED)
		{
			MessageBox((CString)"Access denied.");
		}
		else
		{
			MessageBox(e->m_strDescription);
		}
	}	

#pragma endregion

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

//Test move

void CMVSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMVSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CMVSDlg, CDialog)
	ON_EVENT(CMVSDlg, IDC_AMC, 2, CMVSDlg::OnNewImageAmc, VTS_NONE)
	ON_EVENT(CMVSDlg, IDC_AMC2, 2, CMVSDlg::OnNewImageAmc_2, VTS_NONE)
END_EVENTSINK_MAP()

void CMVSDlg::OnNewImageAmc()
{
	//Initialise image from AMC
	int imgW = 640;    
	int imgH = 480;
	//HRESULT hResult = S_OK; 
	VARIANT var;
	long bufferSize;
	VariantInit(&var);
	
	
	m_AMC.GetCurrentImage(1,&var,&bufferSize);
	BYTE* buf = var.pbVal;//.pbVal; // get the buffer as a byte array
	
	buf = buf + 64;//sizeof(BITMAPINFOHEADER); // seek the beginnig of the  image  data


	EnterCriticalSection( &m_CriticalSection );
	if (cam1_img == NULL)
	{
		//Auto select frame size / resolution 640x480x3+40
		switch(bufferSize)
		{

		case 518440 : 
			imgW = 480;    
			imgH = 360;
			break;
		case 921640 : 
			imgW = 640;    
			imgH = 480;
			break;
		case 1440040 : 
			imgW = 800;    
			imgH = 600;
			break;
		case 3072040 : 
			imgW = 1280;    
			imgH = 800;
			break;
		case 3932200 : 
			imgW = 1280;    
			imgH = 1024;
			break;
		case 5760040 : 
			imgW = 1600;    
			imgH = 1200;
			break;
		case 6220840 : 
			imgW = 1920;    
			imgH = 1080;
			break;
		}
		//Create first image buffer
		//Critical section - access into share resource
		cam1_img = cvCreateImage(cvSize(imgW,imgH),IPL_DEPTH_8U,3);	
	}
	LeaveCriticalSection( &m_CriticalSection );   

	//hResult = ::SafeArrayAccessData(var.parray, &pImage);
    // copy the data to the image
	EnterCriticalSection( &m_CriticalSection );
    memcpy(cam1_img->imageData,buf,cam1_img->imageSize);
    cvFlip(cam1_img,cam1_img,0);
	LeaveCriticalSection( &m_CriticalSection );   
	VariantClear(&var);


}

void CMVSDlg::OnNewImageAmc_2()
{
	//Initialise image from AMC
	int imgW = 640;    
	int imgH = 480;
	//HRESULT hResult = S_OK; 
	VARIANT var;
	long bufferSize;
	VariantInit(&var);


	m_AMC_2.GetCurrentImage(1,&var,&bufferSize);
	BYTE* buf = var.pbVal;//.pbVal; // get the buffer as a byte array

	buf = buf + 64;//sizeof(BITMAPINFOHEADER); // seek the beginnig of the  image  data


	EnterCriticalSection( &m_CriticalSection );
	if (cam2_img == NULL)
	{
		//Auto select frame size / resolution 640x480x3+40
		switch(bufferSize)
		{

		case 518440 : 
			imgW = 480;    
			imgH = 360;
			break;
		case 921640 : 
			imgW = 640;    
			imgH = 480;
			break;
		case 1440040 : 
			imgW = 800;    
			imgH = 600;
			break;
		case 3072040 : 
			imgW = 1280;    
			imgH = 800;
			break;
		case 3932200 : 
			imgW = 1280;    
			imgH = 1024;
			break;
		case 5760040 : 
			imgW = 1600;    
			imgH = 1200;
			break;
		case 6220840 : 
			imgW = 1920;    
			imgH = 1080;
			break;
		}
		//Create first image buffer
		//Critical section - access into share resource
		cam2_img = cvCreateImage(cvSize(imgW,imgH),IPL_DEPTH_8U,3);	
	}
	LeaveCriticalSection( &m_CriticalSection );   

	//hResult = ::SafeArrayAccessData(var.parray, &pImage);
	// copy the data to the image
	EnterCriticalSection( &m_CriticalSection );
	memcpy(cam2_img->imageData,buf,cam2_img->imageSize);
	cvFlip(cam2_img,cam2_img,0);
	LeaveCriticalSection( &m_CriticalSection );   
	VariantClear(&var);

}

void CMVSDlg::OnConfigureImageprocess()
{
	// TODO: Add your command handler code here
	if (State_str != "Normal")
	{
		m_Setup.ShowWindow(SW_SHOW);
	}
}

void CMVSDlg::OnBnClickedCancel()
{
	mbusProtocol.closeProtocol();
	// TODO: Add your control notification handler code here
	SaveConfig(".\\config.ini");
	
	OnCancel();

}

void CMVSDlg::OnBnClickedSample()
{
	// TODO: Add your control notification handler code here
}

void CMVSDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString str_NewIp;
	
	m_Edit_IpAddress.GetWindowText(str_NewIp);

	m_ipText = str_NewIp;

	m_Edit_IpAddress_2.GetWindowText(str_NewIp);

	m_ipText_2 = str_NewIp;
}

void CMVSDlg::OnComSetup()
{
	// TODO: Add your command handler code here
	if (State_str != "Normal")
	{
		m_Com.ShowWindow(SW_SHOW);
	}	
}

