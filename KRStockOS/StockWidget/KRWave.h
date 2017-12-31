#ifndef KRWave_h__
#define KRWave_h__

#include <vector>
using namespace std;


enum{WAVEMARK_UP,WAVEMARK_DOWN};

enum{WAVE_EXTENDINFO_DisposeSubWave=100,WAVE_EXTENDINFO_NormalWave,WAVE_EXTENDINFO_Virtual};


typedef struct _WAVE
{
	float fPole;
	BOOL  bMark;
	INT   nOffset;
	INT  bExtendInfo;
}WAVEDATA,*LPWAVEDATA;


BOOL InitWave(LPKLINEDATA lpKLineData,int nSize, vector<WAVEDATA> & VWaveData);

#endif // KRWave_h__