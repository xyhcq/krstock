#include "StdAfx.h"
#include "DirectoryMgr.h"
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include <strsafe.h>

CFileMgr::CFileMgr()
{
	ZeroMemory(m_szRootPath,sizeof(m_szRootPath));
}

CFileMgr::~CFileMgr()
{
	
}

void CFileMgr::MakePath( LPCTSTR lpszFileName,LPTSTR lpszOutPath,INT nSize )
{
	StringCbCopy(lpszOutPath,nSize,m_szRootPath);
	StringCbCat(lpszOutPath,nSize,lpszFileName);
}

BOOL CFileMgr::Init( LPCTSTR lpszDirName )
{
	TCHAR szModuleFileName[MAX_PATH]={0};
	GetModuleFileName(NULL,szModuleFileName,MAX_PATH);
	PathRemoveFileSpec(szModuleFileName);
	
	StringCbCopy(m_szRootPath,MAX_PATH,szModuleFileName);
	StringCbCat(m_szRootPath,MAX_PATH,"\\");
	
	StringCbCat(m_szRootPath,MAX_PATH,lpszDirName);
	CreateDirectory(m_szRootPath,NULL);
	StringCbCat(m_szRootPath,MAX_PATH,"\\");
	
	TCHAR szFindPath[MAX_PATH]={0};
	MakePath("*.*",szFindPath,MAX_PATH);
	
	
	WIN32_FIND_DATA FindData;  
	HANDLE hFind = FindFirstFile(szFindPath,&FindData);  
	if (hFind == INVALID_HANDLE_VALUE)  
		return TRUE;  
	
	while(FindNextFile(hFind,&FindData))  
	{  
		if (_tcscmp(FindData.cFileName,_T(".") ) == 0 || _tcscmp(FindData.cFileName,_T("..") ) == 0)   
			continue;  
		
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;
		
		m_SDir.insert(FindData.cFileName);
	}
	FindClose(hFind);
	return TRUE;
}

BOOL CFileMgr::SaveFile( LPCTSTR lpszFileName,void* pBuffer,DWORD & dwInOutBufferSize )
{
	char szFilePath[MAX_PATH]={0};
	MakePath(lpszFileName,szFilePath,MAX_PATH);
	
	HANDLE hFile = CreateFile(szFilePath,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	
	DWORD nBytesWrites = 0;
	BOOL bOk = WriteFile(hFile,pBuffer,dwInOutBufferSize,&nBytesWrites,NULL);
	
	SetEndOfFile(hFile);
	CloseHandle(hFile);
	dwInOutBufferSize = nBytesWrites;
	return bOk && dwInOutBufferSize;
}

BOOL CFileMgr::LoadFile(LPCTSTR lpszFileName,void* pBuffer,DWORD & dwInBufferSize )
{
	char szFilePath[MAX_PATH]={0};
	MakePath(lpszFileName,szFilePath,MAX_PATH);
	
	HANDLE hFile = CreateFile(szFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	
	DWORD dwFileSizeHigh;
	DWORD dwFileSize = GetFileSize(hFile,&dwFileSizeHigh);
	if(dwFileSize == 0 || dwFileSize > dwInBufferSize)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	
	DWORD dwNumberOfBytesRead;
	BOOL bOk = ReadFile(hFile,pBuffer,dwFileSize,&dwNumberOfBytesRead,NULL);
	CloseHandle(hFile);
	dwInBufferSize = dwNumberOfBytesRead;
	return bOk && dwInBufferSize;
}

CDirectoryMgr::CDirectoryMgr()
{
	ZeroMemory(m_szRootPath,sizeof(m_szRootPath));
}

CDirectoryMgr::~CDirectoryMgr()
{
	
}

BOOL CDirectoryMgr::Init( LPCTSTR lpszDirName )
{
	TCHAR szModuleFileName[MAX_PATH]={0};
	GetModuleFileName(NULL,szModuleFileName,MAX_PATH);
	PathRemoveFileSpec(szModuleFileName);
	
	StringCbCopy(m_szRootPath,MAX_PATH,szModuleFileName);
	StringCbCat(m_szRootPath,MAX_PATH,"\\");
	
	StringCbCat(m_szRootPath,MAX_PATH,lpszDirName);
	CreateDirectory(m_szRootPath,NULL);
	StringCbCat(m_szRootPath,MAX_PATH,"\\");
	
	TCHAR szFindPath[MAX_PATH]={0};
	MakePath(NULL,"*.*",szFindPath,MAX_PATH);
	
	WIN32_FIND_DATA FindData;  
	HANDLE hFind = FindFirstFile(szFindPath,&FindData);  
	if (hFind == INVALID_HANDLE_VALUE)  
		return TRUE;  
	
	while(FindNextFile(hFind,&FindData))  
	{  
		if (_tcscmp(FindData.cFileName,_T(".") ) == 0 || _tcscmp(FindData.cFileName,_T("..") ) == 0)   
			continue;  
		
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			m_SDir.insert(FindData.cFileName);
	}
	FindClose(hFind);
	return TRUE;
}

void CDirectoryMgr::MakePath( LPCTSTR lpszDirName,LPCTSTR lpszFileName,LPTSTR lpszOutPath,INT nSize )
{
	StringCbCopy(lpszOutPath,nSize,m_szRootPath);
	if(lpszDirName)
	{
		StringCbCat(lpszOutPath,nSize,lpszDirName);
		StringCbCat(lpszOutPath,nSize,"\\");
	}
	
	if(lpszFileName){
		StringCbCat(lpszOutPath,nSize,lpszFileName);
	}
}

BOOL CDirectoryMgr::MakeDirectory( LPCTSTR lpszDirName )
{
	TCHAR szDirPath[MAX_PATH]={0};
	MakePath(lpszDirName,NULL,szDirPath,MAX_PATH);
	
	m_SDir.insert(lpszDirName);
	
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(szDirPath))
		return CreateDirectory(szDirPath,NULL);
	return TRUE;
}

BOOL CDirectoryMgr::AddDataToFile( LPCTSTR lpszDirName,LPCTSTR lpszFileName,void* pBuffer,DWORD & dwInOutBufferSize )
{
	char szFilePath[MAX_PATH]={0};
	MakePath(lpszDirName,lpszFileName,szFilePath,MAX_PATH);
	MakeDirectory(lpszDirName);
	
	HANDLE hFile = CreateFile(szFilePath,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	
	
	SetFilePointer(hFile, 0, NULL, FILE_END); 
	
	DWORD nBytesWrites = 0;
	BOOL bOk = WriteFile(hFile,pBuffer,dwInOutBufferSize,&nBytesWrites,NULL);
	
	SetEndOfFile(hFile);
	CloseHandle(hFile);
	dwInOutBufferSize = nBytesWrites;
	return bOk;
}

BOOL CDirectoryMgr::SaveFile( LPCTSTR lpszDirName,LPCTSTR lpszFileName,void* pBuffer,DWORD & dwInOutBufferSize )
{
	char szFilePath[MAX_PATH]={0};
	MakePath(lpszDirName,lpszFileName,szFilePath,MAX_PATH);
	MakeDirectory(lpszDirName);
	
	HANDLE hFile = CreateFile(szFilePath,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	
	DWORD nBytesWrites = 0;
	BOOL bOk = WriteFile(hFile,pBuffer,dwInOutBufferSize,&nBytesWrites,NULL);
	
	SetEndOfFile(hFile);
	CloseHandle(hFile);
	dwInOutBufferSize = nBytesWrites;
	return bOk && dwInOutBufferSize;
}

BOOL CDirectoryMgr::LoadFile( LPCTSTR lpszDirName,LPCTSTR lpszFileName,void* pBuffer,DWORD & dwInBufferSize )
{
	char szFilePath[MAX_PATH]={0};
	MakePath(lpszDirName,lpszFileName,szFilePath,MAX_PATH);
	MakeDirectory(lpszDirName);
	
	HANDLE hFile = CreateFile(szFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	
	DWORD dwFileSizeHigh;
	DWORD dwFileSize = GetFileSize(hFile,&dwFileSizeHigh);
	if(dwFileSize == 0 || dwFileSize > dwInBufferSize)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	
	DWORD dwNumberOfBytesRead;
	BOOL bOk = ReadFile(hFile,pBuffer,dwFileSize,&dwNumberOfBytesRead,NULL);
	CloseHandle(hFile);
	dwInBufferSize = dwNumberOfBytesRead;
	return bOk && dwInBufferSize;
}

BOOL CDirectoryMgr::DeleteFile( LPCTSTR lpszDirName,LPCTSTR lpszFileName )
{
	char szFilePath[MAX_PATH]={0};
	MakePath(lpszDirName,lpszFileName,szFilePath,MAX_PATH);

	return ::DeleteFile(szFilePath);
}

BOOL CDirectoryMgr::DeleteDir( LPCTSTR lpszDirName )
{
	char szDirPath[MAX_PATH]={0};
	StringCbCopy(szDirPath,MAX_PATH,m_szRootPath);
	StringCbCat(szDirPath,MAX_PATH,lpszDirName);
	return ::RemoveDirectory(szDirPath);
}


