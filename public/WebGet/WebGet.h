
#ifndef THSWebF10_h__
#define THSWebF10_h__

#include "Http.h"
#include <string>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

// #pragma pack(push,1)
// 
// typedef struct _THSDATA
// {	
// 	char  szLabel[10];
// 	char  szZYYW[600];//-��Ӫҵ��
// 	char  szSSHY[15];//-������ҵ
// 	char  szSJGN[180];//-�漰���� || --����ǿ������
// 	float fSYLDT;    //-��ӯ��(��̬)
// 	float fSYLJT;  //-��ӯ��(��̬)
// 	float fSJL;    //-�о���
// 	char strZXJJ[11];//-���½������
// 	DWORD dwGDZS;	//�ɶ�����
// 	float fJSQBH;//-�����ڱ仯
// 	float fJGCBGS;//-�����ɱ�����
// }THSDATA,*LPTHSDATA;
// 
// #pragma pack(pop)

BOOL WebGet_MHDATE_GGCG ( const char * lpStockCode,vector<time_t>& VGntc);
BOOL WebGet_MHDATE_YJBG (const char* lpStockCode,vector<time_t>& VDate);
BOOL WebGet_MHDATE_CWBG( const char * lpStockCode,vector<time_t>& VCwbbsj);
BOOL WebGet_MHDATE_LHB (const char* lpStockCode,vector<time_t>& VDate);

BOOL WebGet_BaseInfo(LPCTSTR szCode,HQBASE_INFO * pOutData);

#endif // THSWebF10_h__










