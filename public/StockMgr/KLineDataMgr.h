#ifndef KLineDataMgr_h__
#define KLineDataMgr_h__

#include "Common/Common.h"
#include "ComHeader/ComHeader.h"
#include "DirectoryMgr.h"
#include "StockCommon.h"


#include <set>
#include <map>
#include <vector>
#include <string>
#include <assert.h>
using namespace std;

typedef vector <KLINEDATA>  VKL;
#define  LOAD_BUFFER_SIZE	100000	



class CKLineDataMgr
{
public:
	CDirectoryMgr		m_DirMgr;
public:
	CKLineDataMgr();
	void Destory();
	BOOL Init();
	INT  GetSize();

	//  Make �ӿ� ֻ���������ݲ�ȫ����µ��� 
	//  ��Ϊ��ȫ������ ���Լ��ļ�¼������ ʱ��첻һ�� ���������
	INT  MakeF5KLine(VKL & VklF5_New,LPCTSTR szLabel);
	INT  MakeF1KLine(VKL & VklF1_New,LPCTSTR szLabel);
	INT  MakeDayKLine(VKL & VklDay_New,LPCTSTR szLabel);

	BOOL Select(VKL & vkl,LPCTSTR szLabel,LPCTSTR lpszFileName);

	const map< string , VKL > * GetMapPoint(){ return &m_DayKLine; }
private:
	BOOL LoadFile(VKL & vkl , LPCTSTR szLabel,LPCTSTR lpszFileName);
	BOOL SaveFile(VKL & vkl , LPCTSTR szLabel,LPCTSTR lpszFileName);
	INT  MergeKLine(VKL & VklNew,VKL & VklOld);

private:
	LPKLINEDATA					m_LoadKLineBuffer;
	map< string , VKL >			m_DayKLine;
};


#endif // KLineDataMgr_h__