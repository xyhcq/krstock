
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


BOOL THSWebF10_GetInfo(LPCTSTR szCode,HQBASE_INFO * pOutData);

#endif // THSWebF10_h__










