#ifndef DirectoryMgr_h__
#define DirectoryMgr_h__

#pragma warning (disable: 4514 4786)
#include <set>
#include <string>
using namespace std;



class CFileMgr
{
public:
	CFileMgr();
	~CFileMgr();
	void MakePath(LPCTSTR lpszFileName,LPTSTR lpszOutPath,INT nSize);
	BOOL Init(LPCTSTR lpszDirName);
	BOOL SaveFile(LPCTSTR lpszFileName,void* pBuffer,DWORD & dwInOutBufferSize);
	BOOL LoadFile(LPCTSTR lpszFileName,void* pBuffer,DWORD & dwInBufferSize);
	set<string> & Get(){ 
		return m_SDir; 
	}
	DWORD Size(){ 
		return m_SDir.size(); 
	}
private:
	TCHAR m_szRootPath[MAX_PATH];
	set<string> m_SDir;
};


class CDirectoryMgr
{
public:
	CDirectoryMgr();

	~CDirectoryMgr();
public:
	BOOL Init(LPCTSTR lpszDirName);
	void MakePath(LPCTSTR lpszDirName,LPCTSTR lpszFileName,LPTSTR lpszOutPath,INT nSize);
	BOOL MakeDirectory(LPCTSTR lpszDirName);
	BOOL AddDataToFile(LPCTSTR lpszDirName,LPCTSTR lpszFileName,void* pBuffer,DWORD & dwInOutBufferSize);
	BOOL SaveFile(LPCTSTR lpszDirName,LPCTSTR lpszFileName,void* pBuffer,DWORD & dwInOutBufferSize);
	BOOL LoadFile(LPCTSTR lpszDirName,LPCTSTR lpszFileName,void* pBuffer,DWORD & dwInBufferSize);

	BOOL DeleteFile(LPCTSTR lpszDirName,LPCTSTR lpszFileName);
	BOOL DeleteDir(LPCTSTR lpszDirName);

	set<string> & Get(){ 
		return m_SDir; 
	}
	DWORD Size(){ 
		return m_SDir.size(); 
	}
private:
	TCHAR m_szRootPath[MAX_PATH];
	set<string> m_SDir;
};


#endif // DirectoryMgr_h__