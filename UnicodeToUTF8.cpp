#include <windows.h>

void UnicodeToUTF8( const BYTE* pIn, BYTE** ppOut )
{
	const wchar_t*		pUniBuf = (wchar_t*)pIn;
//	char*			pBuf = (char*)m_pData;
	int				nBufLen = lstrlenW( (const wchar_t*)pIn );
	int				i;
//	int				j;
//	int				nCharChars;
	unsigned char*	pWork;
	unsigned char*	pDes;
	int				k;
//	unsigned short	sCode;
//	int				nUTF8Bytes;
//	int				nUNICODEBytes;
//	unsigned char	pWork[100];
//	unsigned char*	pBufUTF8;
//	wchar_t*		pUniBuf;


	k = 0;
//	for( i = 0; i < (int)(nBufLen / sizeof( wchar_t )); ++i ){
	for( i = 0; i < nBufLen ; ++i ){
		if( 0x0000 <= pUniBuf[i] && 0x007f >= pUniBuf[i] ){
			k += 1;
		}
		else if( 0x0080 <= pUniBuf[i] && 0x07ff >= pUniBuf[i] ){
			k += 2;
		}
		else if( 0x0800 <= pUniBuf[i] && 0xffff >= pUniBuf[i] ){
			k += 3;
		}
	}
	pDes = new unsigned char[k + 1];
	memset( pDes, 0, k + 1 );
	k = 0;
//	for( i = 0; i < (int)(nBufLen / sizeof( wchar_t )); ++i ){
	for( i = 0; i < nBufLen ; ++i ){
		pWork = (unsigned char*)&((pUniBuf[i]));
//		pWork = (unsigned char*)pUniBuf;
//		pWork += sizeof( wchar_t ) * i;

		if( 0x0000 <= pUniBuf[i] && 0x007f >= pUniBuf[i] ){
			pDes[k] = pWork[0];
			k += 1;
		}else
		if( 0x0080 <= pUniBuf[i] && 0x07ff >= pUniBuf[i] ){
			pDes[k] = (( pWork[0] & 0xc0 ) >> 6 )
				    | ( ( pWork[1] & 0x07 ) << 2 )
					| 0xc0;
			pDes[k + 1] = ( pWork[0] & 0x3f ) | 0x80;
			k += 2;
		}else
		if( 0x0800 <= pUniBuf[i] && 0xffff >= pUniBuf[i] ){
			pDes[k] = (( pWork[1] & 0xf0 ) >> 4 )
						| 0xe0;
			pDes[k + 1] = 0x80 
				        | (( pWork[1] & 0x0f ) << 2 )
						| (( pWork[0] & 0xc0 ) >> 6 );
			pDes[k + 2] = 0x80 
				        | ( pWork[0] & 0x3f );

			k += 3;
		}else{
		}
	}
//	delete [] pDes;
	*ppOut = pDes;
	return;
}
