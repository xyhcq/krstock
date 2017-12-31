#include "stdafx.h"
#include "KRWave.h"
#include "StockAlgorithm.h"


#include <SHLWAPI.H>
#pragma comment(lib,"Shlwapi.lib")
void Debug(LPCTSTR lpszFileName,LPVOID lpData,DWORD dwSize)
{
	TCHAR szDebugInfoPathDir[MAX_PATH];
	GetModuleFileName(NULL,szDebugInfoPathDir,sizeof(szDebugInfoPathDir)/sizeof(TCHAR));
	PathRemoveFileSpec(szDebugInfoPathDir);
	
	TCHAR szDebugInfoPath[MAX_PATH];
	sprintf(szDebugInfoPath,"%s\\Debug\\%s.txt",szDebugInfoPathDir,lpszFileName);
	HANDLE hFile = CreateFile(szDebugInfoPath,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	DWORD dwWriteByte;
	TCHAR szInfo[1000]={0};
	if( hFile != INVALID_HANDLE_VALUE)
	{
		WriteFile(hFile,lpData,dwSize,&dwWriteByte,NULL);
		SetEndOfFile(hFile);
		CloseHandle(hFile);
	}
}


void OutputDebugText(LPCSTR szFormat,...)
{
	va_list vagrc;
	va_start(vagrc,szFormat);
	CHAR  szFormatBuffer[1024]={0};
	vsprintf(szFormatBuffer,szFormat,vagrc);
	va_end(vagrc);
	OutputDebugString(szFormatBuffer);
}


INT HHighOffset(LPKLINEDATA  lpData, INT nBegin , INT nEnd)
{
	INT nOffset = nBegin;
	float fHight = lpData[nBegin].fHigh;
	for (INT i = nBegin + 1; i< nEnd; i++)
	{
		if(lpData[i].fHigh > fHight)
		{
			fHight = lpData[i].fHigh;
			nOffset = i;
		}
	}
	return nOffset;
}


INT LLowOffset(LPKLINEDATA  lpData, INT nBegin , INT nEnd)
{
	INT nOffset = nBegin;
	float fLow = lpData[nBegin].fLow;
	for (INT i = nBegin + 1; i< nEnd; i++)
	{
		if(lpData[i].fLow < fLow)
		{
			fLow = lpData[i].fLow;
			nOffset = i;
		}
	}
	return nOffset;
}



BOOL IsLocalityLow(LPKLINEDATA  lpData ,INT nSize ,INT nIndex , INT nNum)
{
	INT nBegin = nIndex < nNum ? 0 : nIndex - nNum;
	INT nEnd = nIndex + nNum > nSize ? nSize : nIndex+nNum;
	
	if(nEnd - nBegin < nNum)
		return FALSE;
	
	//找出最小值
	float LL = 999999.0f;
	INT nOffset = -1;
	for (INT i = nBegin; i< nEnd; i++)
	{
		if(lpData[i].fLow < LL)
		{
			LL = lpData[i].fLow;
			nOffset = i;
		}
	}
	if(nOffset == nIndex)
		return TRUE;
	
	return FALSE;
}


BOOL IsLocalityHigh(LPKLINEDATA  lpData ,INT nSize ,INT nIndex , INT nNum)
{
	INT nBegin = nIndex < nNum ? 0 : nIndex - nNum;
	INT nEnd = nIndex + nNum > nSize ? nSize : nIndex+nNum;
	if(nEnd - nBegin < nNum)
		return FALSE;
	
	//找出最大值
	float HH = 0.00f;
	INT nOffset = -1;
	for (INT i = nBegin; i< nEnd; i++)
	{
		if(lpData[i].fHigh > HH)
		{
			HH = lpData[i].fHigh;
			nOffset = i;
		}
	}
	if(nOffset == nIndex)
		return TRUE;
	
	return FALSE;
}



BOOL GetSubWave(vector<WAVEDATA> & VSubWaveData,LPKLINEDATA lpKLineData,INT nSize,INT nBegin,INT nEnd)
{
	VSubWaveData.clear();
	if(nEnd - nBegin < 5) 
		return FALSE;

	WAVEDATA WaveData;
	for(INT i=nBegin;i<=nEnd;i++)
	{
		BOOL bIsHighWave = IsLocalityHigh(lpKLineData,nSize,i,4);
		BOOL bIsLowWave  = IsLocalityLow(lpKLineData,nSize,i,4);
		
		//又是顶又是底的 无视之
		if(bIsLowWave && bIsHighWave )
			continue;
		
		WaveData.nOffset=i;
		WaveData.bExtendInfo = WAVE_EXTENDINFO_DisposeSubWave;
		if(bIsHighWave)
		{
			WaveData.fPole = lpKLineData[i].fHigh;
			WaveData.bMark = WAVEMARK_UP;
			VSubWaveData.push_back(WaveData);
		}else if(bIsLowWave)
		{
			WaveData.fPole = lpKLineData[i].fLow;
			WaveData.bMark = WAVEMARK_DOWN;
			VSubWaveData.push_back(WaveData);
		}
	}
	

	if(!VSubWaveData.size()) 
		return FALSE;
	
	return VSubWaveData.size() ? TRUE : FALSE;
}






INT GetNext_OppositeWave(vector<WAVEDATA> & VWaveData, INT nIndex)
{
	if(nIndex >= VWaveData.size())
		return -1;

	for (INT i=nIndex; i< VWaveData.size(); i++)
	{
		if(VWaveData[i].bMark != VWaveData[nIndex].bMark)
			return i;
	}
	return -1;
}









void OptimizeRepetition(vector<WAVEDATA> & VWaveData)
{
	for (INT i=0;i < VWaveData.size() -1 ; i++)
	{
		if(VWaveData[i].bMark ==  VWaveData[i+1].bMark)
		{
			if(VWaveData[i].bMark == WAVEMARK_UP)
			{
				if(VWaveData[i].fPole < VWaveData[i+1].fPole)
					VWaveData.erase(&VWaveData[i]);
				else 
					VWaveData.erase(&VWaveData[i+1]);
				
			}else
			{
				if(VWaveData[i].fPole > VWaveData[i+1].fPole)
					VWaveData.erase(&VWaveData[i]);
				else
					VWaveData.erase(&VWaveData[i+1]);
				
			}
			i--;
		}
	}
}

void OptimizeWave(LPKLINEDATA lpKLineData,INT nSize,vector<WAVEDATA> & VWaveData)
{
	vector<WAVEDATA> Temp;
	INT nIndex = 0;
	Temp.push_back(VWaveData[nIndex]);
	while(TRUE)
	{
		INT nNext = GetNext_OppositeWave(VWaveData,nIndex);
		if(nNext == -1)
			break;

		Temp.push_back(VWaveData[nNext]);
		nIndex = nNext;
	}

	VWaveData = Temp;

	for(INT i=0; i< VWaveData.size() - 1; i++)
	{
		//下跌浪连接上升浪，下降浪起点为最低点
		if(VWaveData[i].bMark == WAVEMARK_DOWN)
		{
			INT nLLowOffset = LLowOffset(lpKLineData,VWaveData[i].nOffset,VWaveData[i+1].nOffset);
			VWaveData[i].nOffset = nLLowOffset;
			VWaveData[i].fPole   = lpKLineData[nLLowOffset].fLow;

		}else //上升浪连接下跌浪，上升浪的起点为最高点
		{
			INT nHHighOffset = HHighOffset(lpKLineData,VWaveData[i].nOffset,VWaveData[i+1].nOffset);
			VWaveData[i].nOffset = nHHighOffset;
			VWaveData[i].fPole   = lpKLineData[nHHighOffset].fHigh;
		}
	}

}








//有效突破MA10  
//1 当前K线不碰MA10
//2 左边K线不碰MA10
//3 右边K线不碰MA10
BOOL MakeMA10_Wave(float  * MA10Data, LPKLINEDATA lpData, INT nIndex,WAVEDATA &  WaveData)
{

	//检查MA10Data是否已经有数据
	if(MA10Data[nIndex-1] <=0)
		return FALSE;
	

	//向上突破
	if( lpData[nIndex-1].fLow >= MA10Data[nIndex-1]  && lpData[nIndex].fLow >= MA10Data[nIndex] && lpData[nIndex+1].fLow >= MA10Data[nIndex+1])
	{
		ZeroMemory(&WaveData,sizeof(WAVEDATA));
		WaveData.bMark = WAVEMARK_UP;
		
		if(lpData[nIndex-1].fHigh > lpData[nIndex].fHigh)
			nIndex -=1;
		if(lpData[nIndex+1].fHigh > lpData[nIndex].fHigh)
			nIndex +=1;
		
		WaveData.nOffset = nIndex;
		WaveData.fPole = lpData[nIndex].fHigh;
		return TRUE;
	}
	

	//向下突破
	if( lpData[nIndex-1].fHigh <= MA10Data[nIndex-1]  && lpData[nIndex].fHigh <= MA10Data[nIndex] && lpData[nIndex+1].fHigh <= MA10Data[nIndex+1])
	{
		ZeroMemory(&WaveData,sizeof(WAVEDATA));
		WaveData.bMark = WAVEMARK_DOWN;
		
		if(lpData[nIndex-1].fLow < lpData[nIndex].fLow)
			nIndex -=1;
		if(lpData[nIndex+1].fLow < lpData[nIndex].fLow)
			nIndex +=1;
		
		WaveData.nOffset = nIndex;
		WaveData.fPole = lpData[nIndex].fLow;
		return TRUE;
	}
	return FALSE;
}

//删除无效的子浪中的底分型
BOOL DeleteSubWave_SpecialLL(vector<WAVEDATA> & VWaveData,LPKLINEDATA lpKLineData)
{
	BOOL bResult = FALSE;
	
	for (INT i=1;i < VWaveData.size() -1 ; i++)
	{
		if(VWaveData[i].bMark ==  WAVEMARK_UP && VWaveData[i].bExtendInfo == WAVE_EXTENDINFO_DisposeSubWave)
		{
			if(VWaveData[i].nOffset - VWaveData[i-1].nOffset < 4)
			{
				//VWaveData[i-1].nOffset 是特殊的底分型  
				//即高点在左边5根K线范围内是最高的 将该点位删除之
				INT nOffset = VWaveData[i-1].nOffset;
				float fLHigh = lpKLineData[nOffset].fHigh;
				INT nLHigtOffset = nOffset;
				for (INT n=1; n < 5; n++)
				{
					if(lpKLineData[ nOffset - n ].fHigh > fLHigh)
					{
						fLHigh = lpKLineData[nOffset - n].fHigh;
						nLHigtOffset = nOffset - n;
					}
				}
				if(nLHigtOffset == nOffset)
				{
					VWaveData.erase(&VWaveData[i-1]);
					i--;
					bResult= TRUE;
				}
			}	
		}
	}
	
	return bResult;
}



BOOL DeleteSubWave_InvalidHH(vector<WAVEDATA> & VWaveData,LPKLINEDATA lpKLineData)
{
	BOOL bResult = FALSE;
	
	for (INT i=1;i < VWaveData.size() -1 ; i++)
	{
		if(VWaveData[i].bMark !=  WAVEMARK_UP || VWaveData[i].bExtendInfo != WAVE_EXTENDINFO_DisposeSubWave)
			continue;

		if(VWaveData[i].fPole <= lpKLineData [VWaveData[i-1].nOffset].fHigh || VWaveData[i].fPole <= lpKLineData [VWaveData[i+1].nOffset].fHigh )
		{
			WAVEDATA * pD1 = NULL, *pD2 = NULL;

			if(VWaveData[i - 1].fPole <  VWaveData[i + 1].fPole)
			{
				pD1 = &VWaveData[i+1];
				pD2 = &VWaveData[i];
			}else
			{
				pD1 = &VWaveData[i];
				pD2 = &VWaveData[i-1];
			}

			if(pD1) { VWaveData.erase(pD1); i--; bResult = TRUE; }
			if(pD2) { VWaveData.erase(pD2); i--; bResult = TRUE; }
		}
	}

	return bResult;
}



BOOL DeleteSubWave_InvalidOffset(vector<WAVEDATA> & VWaveData)
{
	BOOL bResult = FALSE;

	for (INT i=1;i < VWaveData.size() -1 ; i++)
	{
		if(VWaveData[i].bMark !=  WAVEMARK_UP || VWaveData[i].bExtendInfo != WAVE_EXTENDINFO_DisposeSubWave)
			continue;
	
		/*
					 i
					 * 	
				*   * *
				 * *   *
				  *     *
				 i-1     *

				 i - (i-1) < 4
		*/
		if(VWaveData[i].nOffset - VWaveData[i - 1].nOffset < 4)
		{
			WAVEDATA * pD1 = NULL, *pD2 = NULL;
			if( VWaveData[i].fPole >= VWaveData[i -2].fPole)
			{
				/*
								     i
					  i-2	        * 
					   *           * *
					  *  *        -   *     
					 *     *     *     *  
					*	     *  -       *
				   *           *         * 
				  *	          i-1         *
               
					i - (i-1) < 4
					(i-2) < i 
					
					PD1 = i-1;
					PD2 = i-2	
				*/
				pD1 = &VWaveData[i-1];
				pD2 = &VWaveData[i-2];
			}else
			{
				/*
								   
					  i-2	      
					   *          i
					  *  *        *        
					 *     *     * *  
					*	     *  *   *
				   *           *     * 
				  *	          i-1     *
									   i+1
           
					i - (i-1) < 4
					(i-2) > i 

					if( i+1  < i-1)
					 PD1 = i;
					 PD2 = i-1
					else
					 PD1 = i+1;
					 PD2 = i;
					
				*/
				 if(VWaveData[i+1].fPole < VWaveData[i-1].fPole)
				 {
					 pD1 = &VWaveData[i];
					 pD2 = &VWaveData[i-1];
				 }else
				 {
					pD1 = &VWaveData[i+1];
					pD2  = &VWaveData[i];
				 }		
			}
			if(pD1) { VWaveData.erase(pD1); i--; bResult = TRUE; }
			if(pD2) { VWaveData.erase(pD2); i--; bResult = TRUE; }

			
		}else if(VWaveData[i+1].nOffset - VWaveData[i].nOffset < 4)
		{
			/*
				   *		 i
					*		 * 	
					 *	    * *
					  *    *   *
					   *  *     *
						 *       i+1 
							 
						 (i+1) - i < 4
			*/

			WAVEDATA * pD1 = NULL, *pD2 = NULL;
			/*
                                 i+2
			   *		 i       *
				*		 * 	    *
				 *	    * *    *
				  *    *   *  *
				   *  *     * 
					 *       i+1 
						 
					 (i+1) - i < 4

					PD1 = i+1
					PD2 = i
			*/	

			if( VWaveData[i].fPole <= VWaveData[i + 2].fPole)
			{
				pD1 = &VWaveData[i+1];
				pD2 = &VWaveData[i];
			}else
			{
				/*
                                         
				               i         
				       		   *	   i+2
				   *		  * *      * 
					*		 * 	      * 
					 *	    *    *   *   
					  *    *        *     
					   *  *       *        
						 *       i+1        
						                   
							 
						 (i+1) - i < 4

                        
						PD1 = i+2
						PD2 = i+1

				*/	
				 pD1 = &VWaveData[i+2];
				 pD2 = &VWaveData[i+1];
			}

			if(pD1) { VWaveData.erase(pD1); i--; bResult = TRUE; }
			if(pD2) { VWaveData.erase(pD2); i--; bResult = TRUE; }
		}
	}
	return bResult;
}



void AdjustEndWaveOffset(LPKLINEDATA lpKLineData,INT nSize,vector<WAVEDATA> & VWaveData)
{
	//处理最后一浪
	INT nEnd = VWaveData.size() - 1;
	if(VWaveData[nEnd].bMark == WAVEMARK_DOWN)
	{
		INT nLLowOffset = LLowOffset(lpKLineData,VWaveData[nEnd].nOffset,nSize);
		VWaveData[nEnd].nOffset = nLLowOffset;
		VWaveData[nEnd].fPole   = lpKLineData[nLLowOffset].fLow;
	}else
	{
		INT nHHighOffset = HHighOffset(lpKLineData,VWaveData[nEnd].nOffset,nSize);
		VWaveData[nEnd].nOffset = nHHighOffset;
		VWaveData[nEnd].fPole   = lpKLineData[nHHighOffset].fHigh;
	}
}


BOOL InitWave(LPKLINEDATA lpKLineData,int nSize, vector<WAVEDATA> & VWaveData)
{
	VWaveData.clear();

	float * lpfMA10Data = MakeMA(lpKLineData,nSize,10);
	if(lpfMA10Data == NULL) 
		return FALSE;

	WAVEDATA WaveTemp;
	for(INT i=0;i < nSize - 1 ; i++)
	{	
		ZeroMemory(&WaveTemp,sizeof(WAVEDATA));
		if( lpfMA10Data[i] > 0.0  && MakeMA10_Wave(lpfMA10Data,lpKLineData,i,WaveTemp))
			VWaveData.push_back(WaveTemp);
	}
	
	if(VWaveData.size() == 0)
		return FALSE;


	//为美观，为插入模拟第一浪
	if(VWaveData[0].bMark == WAVEMARK_UP && lpKLineData[0].fLow < VWaveData[0].fPole)
	{
		WAVEDATA VRWave;
		VRWave.fPole = lpKLineData[0].fLow;
		VRWave.nOffset = 0;
		VRWave.bMark = WAVEMARK_DOWN;
		VRWave.bExtendInfo = WAVE_EXTENDINFO_Virtual;
		VWaveData.insert(&VWaveData[0],&VRWave,&VRWave+1);
	}else if( lpKLineData[0].fHigh > VWaveData[0].fPole ) //插入上涨浪
	{
		WAVEDATA VRWave;
		VRWave.fPole = lpKLineData[0].fHigh;
		VRWave.nOffset = 0;
		VRWave.bMark = WAVEMARK_UP;
		VRWave.bExtendInfo = WAVE_EXTENDINFO_Virtual;
		VWaveData.insert(&VWaveData[0],&VRWave,&VRWave+1);
	}

	OptimizeWave(lpKLineData,nSize,VWaveData);
	
	AdjustEndWaveOffset(lpKLineData,nSize,VWaveData);

	for (i =0; i< VWaveData.size()-1; i++)
	{
		if((VWaveData[i+1].nOffset - VWaveData[i].nOffset) <= 32)
			continue;
	
		WAVEDATA * lpDebug = &VWaveData[i];
		vector<WAVEDATA>  SubWaveData;
		if(!GetSubWave(SubWaveData,lpKLineData,nSize, VWaveData[i].nOffset + 4,VWaveData[i+1].nOffset - 4))
			continue;
		
		VWaveData.insert(&VWaveData[i+1],SubWaveData.begin(),SubWaveData.end());
		i += SubWaveData.size();
	}

	OptimizeRepetition(VWaveData);

	if(DeleteSubWave_SpecialLL(VWaveData,lpKLineData))
		OptimizeRepetition(VWaveData);

	if(DeleteSubWave_InvalidOffset(VWaveData))
		OptimizeRepetition(VWaveData);

	if(DeleteSubWave_InvalidHH(VWaveData,lpKLineData))
		OptimizeRepetition(VWaveData);


	AdjustEndWaveOffset(lpKLineData,nSize,VWaveData);

	return TRUE;
}


