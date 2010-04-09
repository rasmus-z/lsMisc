#include <windows.h>
#include <tchar.h>

inline const char* MemCharNext( const char* pData, int nDataLen, const char* pDataCurrent )
{
//#ifdef _DEBUG
//	CRunningTimer cRunningTimer( (const char*)"CMemory::MemCharNext" );
//#endif

	const char*	pNext;
	if( pDataCurrent[0] == '\0' ){
		pNext = pDataCurrent + 1;
	}else
	{
		pNext = ::CharNext( pDataCurrent );
	}

	if( pNext >= &pData[nDataLen] ){
		pNext = &pData[nDataLen];
	}
	return pNext;
}

void SJISToEUC( LPCTSTR pData, BYTE** ppTarget)
{
	if(ppTarget==NULL)
		return;
	const unsigned char*	pBuf = (const unsigned char*)pData;
	int				nBufLen = lstrlen(pData);
	int				i;
	int				nCharChars;
	unsigned char*	pDes;
	int				nDesIdx;
	unsigned short	sCode;


//	pDes = new unsigned char[nBufLen * 2];
	pDes = new unsigned char[nBufLen*2];
	nDesIdx = 0;
	for( i = 0; i < nBufLen; ++i ){
//		nCharChars = CMemory::MemCharNext( (const char *)pBuf, nBufLen, (const char *)&(pBuf[i]) ) - (const char*)&(pBuf[i]);
		nCharChars = MemCharNext( (const char *)pBuf, nBufLen, (const char *)&(pBuf[i]) ) - (const char*)&(pBuf[i]);
		if( nCharChars == 1 ){
			if( pBuf[i] >= (unsigned char)0x80 ){
				/* 半角カタカナ */
				pDes[nDesIdx    ] = (unsigned char)0x8e;
				pDes[nDesIdx + 1] = pBuf[i];
				nDesIdx += 2;
			}else{
				pDes[nDesIdx] = pBuf[i];
				nDesIdx++;
			}
		}else
		if( nCharChars == 2 ){
			/* 全角文字 */
			sCode =	(unsigned short)_mbcjmstojis(
				(unsigned int)
				(((unsigned short)pBuf[i    ] << 8) |
				 ((unsigned short)pBuf[i + 1]))
			);
			if(sCode != 0){
				pDes[nDesIdx    ] = (unsigned char)0x80 | (unsigned char)(sCode >> 8);
				pDes[nDesIdx + 1] = (unsigned char)0x80 | (unsigned char)(sCode);
				nDesIdx+=2;
				++i;
			}else{
				pDes[nDesIdx    ] = pBuf[i];
				pDes[nDesIdx + 1] = pBuf[i + 1];
				nDesIdx+=2;
				++i;
			}
		}else
		if( nCharChars > 0 ){
			i += nCharChars - 1;
		}
	}
//	SetData( (const char *)pDes, nDesIdx );
//	delete [] pDes;
	pDes[nDesIdx] = 0;
	*ppTarget	 = pDes;
	return ;
}

