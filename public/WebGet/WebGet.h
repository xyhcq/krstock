
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
// 	char  szZYYW[600];//-主营业务
// 	char  szSSHY[15];//-所属行业
// 	char  szSJGN[180];//-涉及概念 || --概念强弱排名
// 	float fSYLDT;    //-市盈率(动态)
// 	float fSYLJT;  //-市盈率(静态)
// 	float fSJL;    //-市净率
// 	char strZXJJ[11];//-最新解禁日期
// 	DWORD dwGDZS;	//股东总数
// 	float fJSQBH;//-较上期变化
// 	float fJGCBGS;//-机构成本估算
// }THSDATA,*LPTHSDATA;
// 
// #pragma pack(pop)

BOOL WebGet_MHDATE_GGCG ( const char * lpStockCode,vector<time_t>& VGntc);
BOOL WebGet_MHDATE_YJBG (const char* lpStockCode,vector<time_t>& VDate);
BOOL WebGet_MHDATE_CWBG( const char * lpStockCode,vector<time_t>& VCwbbsj);
BOOL WebGet_MHDATE_LHB (const char* lpStockCode,vector<time_t>& VDate);

BOOL WebGet_BaseInfo(LPCTSTR szCode,HQBASE_INFO * pOutData);

#endif // THSWebF10_h__










