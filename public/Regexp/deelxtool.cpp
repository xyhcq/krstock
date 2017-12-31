#include "StdAfx.h"
#include "deelxtool.h"
#include "deelx.h"




BOOL MatchText(LPCTSTR lpszText, LPCTSTR lpszRegular , string & szOut)
{
	CRegexpT <TCHAR> regexp(lpszRegular);
	MatchResult result = regexp.Match(lpszText);
	if(!result.IsMatched())
		return FALSE;
	
	szOut.assign( &lpszText[result.GetStart()] , &lpszText[result.GetEnd()]  );
	return TRUE;
}



BOOL MatchText(LPCTSTR lpszText, LPCTSTR lpszRegular , vector<string> &VOut)
{
	CRegexpT <TCHAR> regexp(lpszRegular);
	CContext * pContext = regexp.PrepareMatch(lpszText);
	
	string s; s.reserve(1024);
    MatchResult result = regexp.Match(pContext);
    while( result.IsMatched() )
    {
		s.assign(&lpszText[result.GetStart()] , &lpszText[result.GetEnd()]);

		VOut.push_back(s);
        result = regexp.Match(pContext);
    }
    regexp.ReleaseContext(pContext);

	return VOut.size() ? TRUE : FALSE;
}



