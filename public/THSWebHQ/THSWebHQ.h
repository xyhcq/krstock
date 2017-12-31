#ifndef THSWebHQ_h__
#define THSWebHQ_h__

typedef struct _THSWEB_HQDATA
{
	char   szSuoShuBanKuai[100];
	float  fBanKuaiZhangDieFu;
	double dChengJiaoJinE;
	double dChengJiaoLiang;
	
	char szChengJiaoJinE[12];
	char szChengJiaoLiang[12];

	float  fXianJia;
	float  fZhangDieFu;
	float  fZhangDieE;

	float fKaiPan;
	float fZuoShou;
	float fZuiGao;
	float fZuiDi;
	float fHuanShou;
	float fZhengFu;
	float fZhangTingJia;
	float fDieTingJia;
	float fJunJia;
	float fWeibi;
	INT nWeiCha;
	INT	nNeiPan;
	INT	nWaiPan;

	//ÊÐÓ¯
	float  fShiYinLv;

	float fBP1;
	float fBP2;
	float fBP3;
	float fBP4;
	float fBP5;
	
	DWORD dwBN1;
	DWORD dwBN2;
	DWORD dwBN3;
	DWORD dwBN4;
	DWORD dwBN5;

	float fSP1;
	float fSP2;
	float fSP3;
	float fSP4;
	float fSP5;
	
	DWORD dwSN1;
	DWORD dwSN2;
	DWORD dwSN3;
	DWORD dwSN4;
	DWORD dwSN5;
}THSWEB_HQDATA,*LPTHSWEB_HQDATA;

BOOL THSWeb_GetWebHQData(LPCTSTR szCode,LPTHSWEB_HQDATA lpTHSWeb_HQData);



BOOL GetTHSWebGSGY(LPCTSTR szCode,CString & szZYYW,CString & szSSHY,CString & szGN,CString & szSYLD,CString & szSYLJ,CString & szSJL,CString & szZXJJ);


typedef struct  _THSWEB_ZJLRLCINFO
{
	INT  nLRDaDan;
	INT  nLRZhongDan;
	INT  nLRXiaoDan;
	INT  nLCDaDan;
	INT  nLCZhongDan;
	INT  nLCXiaoDan;
}THSWEB_ZJLRLCINFO,*LPTHSWEB_ZJLRLCINFO;

BOOL THSWeb_GetZiJinLiuRuLiuChu(LPCTSTR  szCode,LPTHSWEB_ZJLRLCINFO lpTHSWeb_ZJLRLCInfo);


#endif // THSWebHQ_h__