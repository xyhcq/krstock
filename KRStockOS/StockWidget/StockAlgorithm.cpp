#include "StdAfx.h"
#include "StockAlgorithm.h"


vector<float> MakeEMA(vector< KLINEDATA > &X, int N)
{
    std::vector<float> vec;
    int nLen = X.size();
    if(nLen >= 1)
    {
        if(N > nLen) N = nLen;
        vec.resize(nLen);
        vec[0] = X[0].fClose;
        for(int i = 1; i < nLen; i++)
            vec[i] = (2 * X[i].fClose + (N - 1) * vec[i - 1]) / (N + 1);
    }
    return vec;
}

vector<float> MakeEMA(vector< float > &X, int N)
{
    std::vector<float> vec;
    int nLen = X.size();
    if(nLen >= 1)
    {
        if(N > nLen) N = nLen;
        vec.resize(nLen);
        vec[0] = X[0];
        for(int i = 1; i < nLen; i++)
            vec[i] = (2 * X[i] + (N - 1) * vec[i - 1]) / (N + 1);
    }
    return vec;
}


BOOL Price_HH_LL(KLINEDATA * pBegin,  KLINEDATA * pEnd , LPKLINEDATA & pHH,  LPKLINEDATA & pLL)
{
	
	if(pBegin==NULL || pEnd == NULL)
		return FALSE;
	
	pHH  = pBegin;
	pLL  = pBegin;

	for (KLINEDATA * pKLineData =pBegin ; pKLineData < pEnd ; pKLineData++ )
	{
		if(pKLineData->fHigh > pHH->fHigh)
		{
			pHH = pKLineData;
		}
		if(pKLineData->fLow < pLL->fLow)
		{
			pLL = pKLineData;
		}
	}
	return TRUE;
}




BOOL Price_HH_LL(const KLINEDATA * pBegin, const KLINEDATA * pEnd , float & HH, float & LL){

	if(pBegin==NULL || pEnd == NULL)
		return FALSE;

	HH  =pBegin->fHigh;
	LL  =pBegin->fLow;
	for (const KLINEDATA * pKLineData =pBegin ; pKLineData < pEnd ; pKLineData++ )
	{
		if(pKLineData->fHigh > HH)
			HH = pKLineData->fHigh;
		
		if(pKLineData->fLow < LL)
			LL = pKLineData->fLow;
	}
	return TRUE;
}



BOOL Vol_HH_LL(const KLINEDATA * pBegin, const KLINEDATA * pEnd , double & HH, double & LL){
	
	if(pBegin==NULL || pEnd == NULL)
		return FALSE;
	
	HH  =pBegin->fVolume;
	LL  =pBegin->fVolume;
	for (const KLINEDATA * pKLineData =pBegin ; pKLineData < pEnd ; pKLineData++ )
	{
		if(pKLineData->fVolume > HH)
			HH = pKLineData->fVolume;

		if(pKLineData->fVolume < LL)
			LL = pKLineData->fVolume;
	}
	return TRUE;
}

BOOL MakeMA(LPKLINEDATA lpKLineData,int nSize,INT MA , vector<float> & VMA)
{
	VMA.clear();

	float * f = MakeMA(lpKLineData,nSize,MA);
	if(f == NULL)
		return  FALSE;
	
	VMA.assign(&f[0],&f[nSize]);

	delete [] f;
	return VMA.size()?TRUE:FALSE;
}



float * MakeMA(LPKLINEDATA lpKLineData,int nSize,INT MA)
{
	if(nSize  < MA ) 
		return NULL;
	
	float * lpfMAData = new float[nSize];
	if(lpfMAData == NULL)
		return FALSE;
	
	ZeroMemory(lpfMAData,sizeof(float) * nSize);

	vector<float>  MAData;
	INT nBegin = 0;
	for (int i=nBegin;i < nSize ; i++)
	{
		MAData.push_back(lpKLineData[i].fClose);
		if(MAData.size() == MA)
		{
			//计算平均值
			float fMA = 0;
			for(int n = 0; n< MAData.size(); n++)
				fMA += MAData[n];
			
			fMA = fMA / MA;
			lpfMAData[i] = fMA;
			MAData.erase(&MAData[0]);
		}
	}
	return lpfMAData;
}




float * MakeVolumeMA(LPKLINEDATA lpKLineData,int nSize,INT MA)
{
	if(nSize  < MA ) 
		return NULL;
	
	float * lpfMAData = new float[nSize];
	if(lpfMAData == NULL)
		return FALSE;
	
	ZeroMemory(lpfMAData,sizeof(float) * nSize);
	
	vector<float>  MAData;
	INT nBegin = 0;
	for (int i=nBegin;i < nSize ; i++)
	{
		MAData.push_back(lpKLineData[i].fVolume);
		if(MAData.size() == MA)
		{
			//计算平均值
			float fMA = 0;
			for(int n = 0; n< MAData.size(); n++)
				fMA += MAData[n];
			
			fMA = fMA / MA;
			lpfMAData[i] = fMA;
			MAData.erase(&MAData[0]);
		}
	}
	return lpfMAData;
}

BOOL MakeVolumeMA(LPKLINEDATA lpKLineData,int nSize,INT MA , vector<float> & VMA)
{
	float * f = MakeVolumeMA(lpKLineData,nSize,MA);
	if(f == NULL)
		return  FALSE;
	
	VMA.assign(&f[0],&f[nSize]);

	delete [] f;
	return VMA.size()?TRUE:FALSE;
}

void FHHIndex( const float * fBegin, const float * fEnd ,INT & nIndex , float & fSum)
{
	const float * pfHH = fBegin;
	for (const float * pOffset = fBegin; pOffset < fEnd; pOffset++)
	{
		if(*pOffset >= *pfHH)
			pfHH = pOffset;

		fSum+= *pOffset;
	}
	nIndex =  pfHH - fBegin;
}

void FLLIndex( const float * fBegin, const float * fEnd ,INT & nIndex , float & fSum)
{
	const float * pfHH = fBegin;
	for (const float * pOffset = fBegin; pOffset < fEnd; pOffset++)
	{
		if(*pOffset <= *pfHH)
			pfHH = pOffset;
		
		fSum+= *pOffset;
	}
	nIndex =  pfHH - fBegin;
}