#ifndef SocketDataMgr_h__
#define SocketDataMgr_h__

#include "ZLIB/zlib.h"
#pragma comment(lib,"zlib.lib")

#include "Lzma/LzmaLib.h"
#pragma comment(lib,"LZMA.lib")

enum
{
		TOKEN_TICKERSYMBOL_ERROR,
		TOKEN_TICKERSYMBOL_NOUPDATE,
		TOKEN_TICKERSYMBOL_OK
};


enum
{ 
		TOKEN_FINANCIAL_ERROR,
		TOKEN_FINANCIAL_NOUPDATE,
		TOKEN_FINANCIAL_OK
};

enum
{ 
		TOKEN_CHUQUAN_ERROR,
		TOKEN_CHUQUAN_NOUPDATE,
		TOKEN_CHUQUAN_OK
};

enum
{ 
		TOKEN_FENBIDATA_NOTFOUND,
		TOKEN_FENBIDATA_ISEMPTY,
		TOKEN_FENBIDATA_OK
};


enum
{ 
		TOKEN_KLINE_NOTFOUND, 
		TOKEN_KLINE_PERIODERROR,
		TOKEN_KLINE_OK
};


enum{ 
	TOKEN_F10DATA_NOTFOUND, 
	TOKEN_F10DATA_SERVERERROR, 
	TOKEN_F10DATA_NOUPDATE,
	TOKEN_F10DATA_OK

};


enum
{ 
		TOKEN_HQBASE_NOUPDATE, 
		TOKEN_HQBASE_ERROR,
		TOKEN_HQBASE_OK
};

enum
{ 
		TOKEN_RTQH_INIT_NOUPDATE, 
		TOKEN_RTQH_INIT_ERROR,
		TOKEN_RTQH_INIT_OK
};


enum{ 
		TOKEN_ERROR,
		TOKEN_KLINE, 
		TOKEN_TICKERSYMBOL,
		TOKEN_FINANCIAL,
		TOKEN_CHUQUAN,
		TOKEN_RTQH_INIT,
		TOKEN_RTQH_SMALL,
		TOKEN_RTQH_FULL,
		TOKEN_FENBIDATA,
		TOKEN_F10DATA,
		TOKEN_HQBASE
};





class CSocketDataMgr
{
public:
	CSocketDataMgr(){ 
		m_lpPacketData = NULL;
		m_dwPacketDataSize = 0;
		m_lpUnpackData = NULL;
		m_dwUnpackDataSize = 0;
		Init_CRC32Table();
		m_lpSendData = NULL;
	}


	~CSocketDataMgr(){

		if(m_lpPacketData) 
		{
			delete [] m_lpPacketData;
			m_lpPacketData = NULL;
		}

		if(m_lpUnpackData) 
		{
			delete [] m_lpUnpackData;
			m_lpUnpackData = NULL;
		}
		if(m_lpSendData)
		{
			delete m_lpSendData;
			m_lpSendData = NULL;
		}
	}

	UINT CRC32( BYTE * buf, int len)
	{
		UINT ret = 0xFFFFFFFF;
		for(int   i = 0; i < len;i++)
		{
			ret = CRC32Table[((ret & 0xFF) ^ buf[i])] ^ (ret >> 8);
		}
		ret = ~ret;
		return ret;
	}


	BOOL MakeRecvData(DWORD & dwToken,DWORD & dwErrorCode, LPBYTE & pData,DWORD & dwDataSize)
	{
		dwToken = *(DWORD *)pData;
		dwErrorCode = *(DWORD *)(pData+sizeof(DWORD));
		pData+=sizeof(DWORD) + sizeof(DWORD);
		dwDataSize -= sizeof(DWORD) + sizeof(DWORD);
		return TRUE;
	}

	BOOL MakeRecvData( LPBYTE  pData,DWORD  dwDataSize ,LPBYTE & pHeader,DWORD dwHeaderDataSize,LPBYTE & pUserData,DWORD & dwUserDataSize )
	{
		pData+=sizeof(DWORD) + sizeof(DWORD);
		pHeader = pData;
		pData += dwHeaderDataSize;
		pUserData = pData;
		dwUserDataSize = dwDataSize - sizeof(DWORD) - sizeof(DWORD) - dwHeaderDataSize;
		return TRUE;
	}


	BOOL MakeSendData(DWORD dwToken,DWORD dwErrorCode,LPBYTE pData,DWORD dwDataSize)
	{
		if(m_lpSendData) 
			delete [] m_lpSendData;

		m_dwSendSize   = sizeof(DWORD) + sizeof(DWORD) + dwDataSize;
		m_lpSendData   = new BYTE[m_dwSendSize];
		if(m_lpSendData == NULL)
			return FALSE;
		
		*(DWORD *)(m_lpSendData+0)					= dwToken;
		*(DWORD *)(m_lpSendData+sizeof(DWORD))		= dwErrorCode;

		if(dwDataSize > 0)
			CopyMemory(m_lpSendData + sizeof(DWORD) + sizeof(DWORD) , pData,dwDataSize);
		
		return TRUE;
	}


	BOOL MakeSendData(DWORD dwToken,DWORD dwErrorCode,LPBYTE pHeader,DWORD dwHeaderDataSize,LPBYTE pData,DWORD dwDataSize)
	{
		if(m_lpSendData) 
			delete [] m_lpSendData;
		
		if(pHeader == NULL || dwHeaderDataSize ==0 )
			return FALSE;

		m_dwSendSize   = sizeof(DWORD) + sizeof(DWORD) + dwHeaderDataSize + dwDataSize;
		m_lpSendData   = new BYTE[m_dwSendSize];
		if(m_lpSendData == NULL)
			return FALSE;

		*(DWORD *)(m_lpSendData+0)					= dwToken;
		*(DWORD *)(m_lpSendData+sizeof(DWORD))		= dwErrorCode;
		
		CopyMemory(m_lpSendData + sizeof(DWORD) + sizeof(DWORD) , pHeader,dwHeaderDataSize);
		
		if(dwDataSize > 0)
			CopyMemory(m_lpSendData + sizeof(DWORD) + sizeof(DWORD) + dwHeaderDataSize, pData,dwDataSize);

		return TRUE;
	}


	LPBYTE GetSendData(){ return m_lpSendData; }
	DWORD  GetSendDataSize(){ return m_dwSendSize;}

	BOOL Packet(LPBYTE pData,DWORD dwSize)
	{
		if(m_lpPacketData) 
			delete [] m_lpPacketData;

		byte    prop[5]={0};
		size_t  nPropSize = sizeof(prop);
		
		m_lpPacketData = new BYTE[dwSize + sizeof(DWORD) + nPropSize/*7Z需要的数据*/ ];
		if(m_lpPacketData == NULL)
			return FALSE;


		m_dwPacketDataSize = dwSize;
		if(dwSize < 1024) //小于1KB 不压缩
		{
			*(( DWORD * )m_lpPacketData) = m_dwPacketDataSize;
			CopyMemory(m_lpPacketData+sizeof(DWORD),pData,dwSize);
			m_dwPacketDataSize+=sizeof(DWORD);
			return TRUE;
		}


//      ZLib
//		//前4个字节保存压缩前的大小
// 		if(Z_OK == compress(m_lpPacketData + sizeof(DWORD),&m_dwPacketDataSize,pData,dwSize))
// 		{
// 			* (( DWORD * )m_lpPacketData) = dwSize;
// 			m_dwPacketDataSize+=sizeof(DWORD);
// 			return TRUE;
// 		}

		int nRet = LzmaCompress( m_lpPacketData + sizeof(DWORD),		/*接收压缩后数据的缓冲区*/
			( size_t * )&m_dwPacketDataSize,			                /*接收压缩后数据的缓冲区大小*/
			pData,						                                /*需要压缩的缓冲区*/
			dwSize,					                                    /*需要压缩的缓冲区大小*/
			prop,    
			&nPropSize,
			9,					        /*压缩等级*/
			4 * 1024 * 1024,            /*字典大小*/
			7,		
			2,		
			3,	
			200,   
			2);

		if(nRet==SZ_OK)
		{
			//前4个字节保存压缩前的大小
			* (( DWORD * )m_lpPacketData) = dwSize;
			
			//结尾保存 prop
			CopyMemory(m_lpPacketData + m_dwPacketDataSize + sizeof(DWORD), prop , sizeof(prop));
			
			// m_dwPacketDataSize 需要调整
			m_dwPacketDataSize+= sizeof(DWORD);
			m_dwPacketDataSize+= sizeof(prop);
			return TRUE;
		}


		return FALSE;
	}




	BOOL Unpack(LPBYTE pData,DWORD dwSize)
	{

		//解压缩前需要取前4个字节 获得解压后大小
		m_dwUnpackDataSize =  *((DWORD *)pData);
		if(m_lpUnpackData ) 
			delete [] m_lpUnpackData;

		m_lpUnpackData = new BYTE[m_dwUnpackDataSize];
		if(m_lpUnpackData==NULL)
			return FALSE;


		if(m_dwUnpackDataSize < 1024)
		{
			CopyMemory(m_lpUnpackData,pData + sizeof(DWORD) ,m_dwUnpackDataSize);
			return TRUE;
		}


// 		int nResult = uncompress(m_lpUnpackData,&m_dwUnpackDataSize, pData + sizeof(DWORD), dwSize - sizeof(DWORD));
// 		if(Z_OK == nResult)
// 			return TRUE;

		byte    prop[5]={0}; 

		//前4个字节 是保存压缩前的大小  需要跳过
		BYTE * pSrcDataBuf      = pData +  sizeof(DWORD);
		DWORD  dwSrcDataBufSize = dwSize - sizeof(DWORD) - sizeof(prop);
		
		//这个数据在结尾处
		CopyMemory(prop , pSrcDataBuf + dwSrcDataBufSize  , sizeof(prop));

		int nRet = LzmaUncompress(
			m_lpUnpackData,					
			( size_t * )&m_dwUnpackDataSize, 
			pSrcDataBuf,
			( size_t * )&dwSrcDataBufSize, 
			prop, 
			5);

		if(nRet==SZ_OK)
			return TRUE;
		

		return FALSE;
	}



	LPBYTE GetPacketData(){ return m_lpPacketData; }
	DWORD  GetPacketSize(){ return m_dwPacketDataSize;}

	LPBYTE GetUnpackData(){ return m_lpUnpackData; }
	DWORD  GetUnpackSize(){ return m_dwUnpackDataSize;}

private:
	void Init_CRC32Table(){
		int    i,j;
		UINT   crc;
		for(i = 0;i < 256;i++)
		{
			crc = i;
			for(j = 0;j < 8;j++)
			{
				if(crc & 1)
					crc = (crc >> 1) ^ 0xEDB88320;
				else
					crc = crc >> 1;
			}
			CRC32Table[i] = crc;
		}
	}


private:
	LPBYTE m_lpPacketData;
	DWORD  m_dwPacketDataSize;

	LPBYTE m_lpUnpackData;
	DWORD  m_dwUnpackDataSize;

	UINT   CRC32Table[256];

	DWORD  m_dwSendSize;
	BYTE  *m_lpSendData;
};




#endif // SocketDataMgr_h__