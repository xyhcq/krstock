#ifndef StockAlgorithm_h__
#define StockAlgorithm_h__
#include <VECTOR>
using namespace std;


vector<float> MakeEMA(vector< KLINEDATA > &X, int N);
vector<float> MakeEMA(vector< float > &X, int N);
float * MakeMA(LPKLINEDATA lpKLineData,int nSize,INT MA);
BOOL    MakeMA(LPKLINEDATA lpKLineData,int nSize,INT MA , vector<float> & VMA);

float * MakeVolumeMA(LPKLINEDATA lpKLineData,int nSize,INT MA);
BOOL    MakeVolumeMA(LPKLINEDATA lpKLineData,int nSize,INT MA , vector<float> & VMA);


//INT      GetOpenTimeCount(time_t t);
//float    GetLiangBi(const REALTIMEQUOTES & RTQ);


BOOL Price_HH_LL(KLINEDATA * pBegin,  KLINEDATA * pEnd , LPKLINEDATA & pHH,  LPKLINEDATA & pLL);
BOOL Price_HH_LL(const KLINEDATA * pBegin, const KLINEDATA * pEnd , float & HH, float & LL);
BOOL Vol_HH_LL(const KLINEDATA * pBegin, const KLINEDATA * pEnd , double & HH, double & LL);

void FHHIndex( const float * fBegin, const float * fEnd ,INT & nIndex , float & fSum);
void FLLIndex( const float * fBegin, const float * fEnd ,INT & nIndex , float & fSum);


#define  VS_HH_LL(V,S,H,L)		{H = V[0].S; L = V[0].S;			\
								for(INT _i=0; _i< V.size(); _i++){	\
								if(V[_i].S > H )					\
								H = V[_i].S;						\
								if(V[_i].S < L)						\
								L = V[_i].S;						\
								}}


#define  V_HH_LL(V,H,L)			{H = V[0]; L = V[0];				\
								for(INT _i=0; _i< V.size(); _i++){	\
								if(V[_i] > H )						\
								H = V[_i];							\
								if(V[_i] < L)						\
								L = V[_i];							\
								}}


#endif // StockAlgorithm_h__