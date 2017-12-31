#ifndef TopLevelExceptionFilter_h__
#define TopLevelExceptionFilter_h__

#pragma warning(disable:4786)

#include <vector>
#include <string>
using namespace std;
class CStackRecord
{
public:
	void Push(const char* lpFunctionName){
		m_VArray.push_back(lpFunctionName);
	}
	
	void Pop(){
		m_VArray.pop_back();
	}
	
	size_t size(){
		return m_VArray.size();
	}
	
	const char* Get(size_t i){
		return m_VArray[i].c_str();
	}
private:
	vector<string> m_VArray;
};

void _Record_StartC(const char* lpFuctionName , const char * szFileName, INT nLine );
void _Record_StartB(const char* lpFuctionName , const char * szFileName );
void _Record_StartA(const char* lpFuctionName);
void _Record_End();


class CAutoStackRecord
{
public:
	CAutoStackRecord(LPCTSTR lpFuctionName)
	{
		_Record_StartA(lpFuctionName);
		m_nCount = 1;
	}

	void Record( LPCTSTR lpFuctionName)
	{
		_Record_StartA(lpFuctionName);
		m_nCount++;
	}

	~CAutoStackRecord()
	{
		for(INT i =0 ; i< m_nCount; i++)
			_Record_End();
	}

private:
	INT  m_nCount;
};





#ifndef  _STACK_RECORD
	//不带文件名
	#define STACK_RECORD_STARTA(FUNC )  _Record_StartA(FUNC);
	//带文件名
	#define STACK_RECORD_STARTB(FUNC )  _Record_StartB(FUNC,__FILE__);
	//带文件名和行号
	#define STACK_RECORD_STARTC(FUNC )  _Record_StartC(FUNC,__FILE__ , __LINE__);
	
	#define STACK_RECORD_END			 _Record_End();

#else
	#define STACK_RECORD_START(FUNC)
	#define STACK_RECORD_END
#endif


class CTopLevelExceptionFilter
{
public:
	void InstallExceptionFilter();
	static LONG WINAPI ExceptionFilter(_EXCEPTION_POINTERS *ExceptionInfo);
	static BOOL m_bShowMessageBox;
};


#endif // TopLevelExceptionFilter_h__