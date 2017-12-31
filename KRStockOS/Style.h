#ifndef Style_h__
#define Style_h__



#define  COLOR_RED			RGB(255,0,0)		//��ɫ
#define  COLOR_GREEN		RGB(0,255,0)		//��ɫ
#define  COLOR_BLUE			RGB(0,0,255)		//��ɫ

#define  COLOR_BLACK		RGB(0,0,0)			//��ɫ
#define  COLOR_WHITE		RGB(255,255,255)	//��ɫ
#define  COLOR_GRAY			RGB(192,192,192)    //ǳ��ɫ MA120
#define  COLOR_SHENHUI      RGB(132,130,123)    //���ɫ ȱ��

#define  COLOR_YELLOW		RGB(255,255,0)		//��ɫ
#define  COLOR_VIOLET		RGB(192,0,192)		//��ɫ


#define	 COLOR_TIANLAN		RGB(84,252,252)		//����ɫ 
#define	 COLOR_SHENLAN		RGB(2,226,244 )		//����ɫ 



COLORREF GetColor(float f);
COLORREF GetColor(float fLastClose , float fCur);
COLORREF GetAmountColor(float f);
COLORREF GetHuanShouColor(float f);
COLORREF GetLiangBiColor(float f);

void FormatAmount(float f, CString & szText ,LPCTSTR szDefault);
void FormatVolume(float f, CString & szText ,LPCTSTR szDefault);

INT  FormatSignedInt(INT i , CString & szText);


float StockFun_WeiBi(float TotleBuyVol, float TotleSellVol);
float StockFun_ZhengFu(float fHigh,float fLow,float fLastClose);

float StockFun_ZhangTing(float fLastClose);
float StockFun_DieTing(float fLastClose);

float StockFun_ZhangTing(float fLastClose,const char * szName);
float StockFun_DieTing(float fLastClose,const char * szName);



#endif // Style_h__