#ifndef deelxtool_h__
#define deelxtool_h__

#include <string>
#include <VECTOR>
using namespace std;
BOOL MatchText(LPCTSTR lpszText, LPCTSTR lpszRegular , string & szOut);
BOOL MatchText(LPCTSTR lpszText, LPCTSTR lpszRegular , vector<string> &VOut);

#endif // deelxtool_h__