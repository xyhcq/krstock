#ifndef GetMidString_h__
#define GetMidString_h__


#pragma warning(disable:4786)

#include <string>
#include <vector>
#include <shlwapi.h>

#pragma comment(lib,"shlwapi.lib")

using namespace std;


VOID copyClipbord(CString str);

void _dzjy_replace(std::string&s1,const std::string s2,const std::string s3);
BOOL _dzjy_GetMidString(const char * pTextBufferPoint,char* leftKeyWords,char* RightKeyWords,std::string& OutMidString,const char** lpNewOffset = NULL);
BOOL _dzjy_GetMidStringEx(const char * pTextBufferPoint,char* leftKeyWords1,  char* RightKeyWords1,char* leftKeyWords2,char* RightKeyWords2,std::string& OutMidString,const char** lpNewOffset = NULL);
BOOL _dzjy_GetValue(const char* lpszContext ,const char* lpszFirstBegin, const char* lpszBegin,const char* lpszEnd,string& OutMidString,const char** lpszNewOffset = NULL);




#endif // GetMidString_h__