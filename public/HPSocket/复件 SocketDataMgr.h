#ifndef SocketDataMgr_h__
#define SocketDataMgr_h__

#include "ZLIB/zlib.h"
#pragma comment(lib,"zlib.lib")



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
		TOKEN_HQBASE_ERROR,
		TOKEN_HQBASE_OK
};

enum{ 
		TOKEN_ERROR,
		TOKEN_KLINE, 
		TOKEN_TICKERSYMBOL,
		TOKEN_FINANCIAL,
		TOKEN_CHUQUAN,
		TOKEN_RTQH,
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

		m_lpPacketData = new BYTE[dwSize + sizeof(DWORD) ];
		if(m_lpPacketData == NULL)
			return FALSE;

		m_dwPacketDataSize = dwSize;
		if(dwSize < 1024)
		{
			*(( DWORD * )m_lpPacketData) = m_dwPacketDataSize;
			CopyMemory(m_lpPacketData+sizeof(DWORD),pData,dwSize);
			m_dwPacketDataSize+=sizeof(DWORD);
			return TRUE;
		}

		//前4个字节保存压缩前的大小
		if(Z_OK == compress(m_lpPacketData + sizeof(DWORD),&m_dwPacketDataSize,pData,dwSize))
		{
			* (( DWORD * )m_lpPacketData) = dwSize;
			m_dwPacketDataSize+=sizeof(DWORD);
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

		int nResult = uncompress(m_lpUnpackData,&m_dwUnpackDataSize, pData + sizeof(DWORD), dwSize - sizeof(DWORD));
		if(Z_OK == nResult)
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