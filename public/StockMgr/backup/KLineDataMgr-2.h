#ifndef KLineDataMgr_h__
#define KLineDataMgr_h__

#include "Common/Common.h"
#include "ComHeader/ComHeader.h"
#include "DirectoryMgr.h"

#include <set>
#include <map>
#include <vector>
#include <string>
#include <assert.h>
using namespace std;

typedef vector <KLINEDATA>  VKL;
#define  LOAD_BUFFER_SIZE	1000000	


class CKLineDataMgr
{
public:
	CKLineDataMgr();
	void Destory();
	BOOL Init();
	BOOL Save();
	//BOOL UpdateDayKLine(const RCV_REPORT_STRUCTExV3 & NewRRS);
	INT  SaveF5KLine(VKL & VklF5_New,LPCTSTR szLabel);
	INT  SaveF1KLine(VKL & VklF1_New,LPCTSTR szLabel);
	INT  SaveDayKLine(VKL & VklDay_New,LPCTSTR szLabel);
private:
	BOOL LoadFile(VKL & vkl , LPCTSTR szLabel,LPCTSTR lpszFileName);
	BOOL SaveFile(VKL & vkl , LPCTSTR szLabel,LPCTSTR lpszFileName);

	INT  MergeKLine(VKL & VklNew,VKL & VklOld);
private:
	LPKLINEDATA			m_LoadKLineBuffer;
	CDirectoryMgr		m_DirMgr;
	map<string,VKL >	m_MData;
};


#endif // KLineDataMgr_h__