
/*
----- UrlEncode 関数 for C 言語 Version 1.02 Beta -----
(C)2002 けるべ
MAIL : kelbe@geocities.co.jp
HOME : http://www.geocities.co.jp/SilkRoad/4511/

引数として与えられた文字列を URL エンコードした文字列を返します。
なお、現バージョンでは ANSI/S-JIS のみ対応です。

引数 *csource
	URL エンコードしたい文字列へのポインタを指定します。

引数 *cbuffer
	URL エンコードされた文字列が格納されるバッファへのポインタを指定します。
	なお、引数 *csource に指定した文字列サイズの 3 倍 + 1 以上のサイズが
	必要となります。

引数 lbuffersize
	cbuffer のバッファサイズを指定します。

戻り値
	関数が成功すると、cbuffer に URL エンコードされた文字列が格納され、
	cbuffer バッファに書き込まれた文字数(終端の NULL 文字を除く)が
	返ります。失敗した場合は 0 が返ります。

この URL エンコード関数は拙作の VB 用 UrlEncode 関数を、強引に C 言語用に
書き換えたものです。私は C 言語については全くの初心者ですので、
ソースもいいかげんです。正しく直して使って下さい(^^;
*/

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include "UrlEncode.h"

void UrlEncode(const unsigned char	*csource,
						size_t	nSize,
						char** pOut,
						int bUseMalloc)
{
	if ( csource==NULL || csource==0 || csource==NULL)
		return;

	if ( bUseMalloc )
		*pOut = (char*)malloc(nSize*3 + 2);
	else
		*pOut = new char[nSize*3 + 2];
	if( *pOut==NULL )
		return;

	const unsigned char* cbuffer = (const unsigned char*)*pOut;

	unsigned long lbuffersize = nSize*3 + 2;
	unsigned long	llength;										/* csource のサイズを格納 */
	unsigned long	lcount = 0;										/* csource の読み込み位置カウンタ */
	unsigned char	cbyte;											/* 抜き出された 1 バイト分のデータを格納 */
	unsigned char	ctemp[4];										/* 変換結果(1 文字分)一時格納バッファ */
	unsigned long	lresultcount = 0;								/* cbuffer の書き込み位置カウンタ */

	llength = nSize;						/* csource の文字サイズを得る */
	if(!llength) { return ; }							/* csource が 0 文字の場合、関数を抜ける */
	if(lbuffersize < (llength * 3 + 1)) { return ; }	/* バッファサイズが足りない場合、関数を抜ける */

	for(;;) {
		cbyte = *(csource + lcount);								/* 1 バイトを抜き出す */
		if( ((cbyte >= 0x81) && (cbyte <= 0x9F)) ||
			((cbyte >= 0xE0) && (cbyte <= 0xEF)) ) {				/* Shift-JIS 2 バイト文字だった場合 */
			sprintf((char*)ctemp, "%%%02X", cbyte);						/* URL エンコード(上位バイト) */
			strncpy((char*)cbuffer + lresultcount, (char*)ctemp, 4);				/* cbuffer にコピー */
			lcount++;												/* 読み込みカウンタをインクリメント */
			lresultcount += 3;										/* 書き込みカウンタを 3 増やす */
			if(lcount == llength) { break; }						/* 文字列の終端に達した場合、ループを抜ける */
			sprintf((char*)ctemp, "%%%02X", *(csource + lcount));			/* URL エンコード(下位バイト) */
			strncpy((char*)cbuffer + lresultcount, (char*)ctemp, 4);				/* cbuffer にコピー */
			lcount++;												/* 読み込みカウンタをインクリメント */
			lresultcount += 3;										/* 書き込みカウンタを 3 増やす */
		} else if(cbyte == 0x20) {									/* 1 バイト半角スペース(" ")だった場合 */
			strncpy((char*)cbuffer + lresultcount, "+", 2);				/* "+" を cbuffer にコピー */
			lcount++;												/* 読み込みカウンタをインクリメント */
			lresultcount++;											/* 書き込みカウンタをインクリメント */
		} else if( ((cbyte >= 0x40) && (cbyte <= 0x5A)) ||			/* @A-Z */
				   ((cbyte >= 0x61) && (cbyte <= 0x7A)) ||			/* a-z */
				   ((cbyte >= 0x30) && (cbyte <= 0x39)) ||			/* 0-9 */
				   (cbyte == 0x2A) ||								/* "*" */
				   (cbyte == 0x2D) ||								/* "-" */
				   (cbyte == 0x2E) ||								/* "." */
				   (cbyte == 0x5F) ) {								/* "_" */ /* 無変換文字だった場合 */
			strncpy((char*)cbuffer + lresultcount, (char*)csource + lcount, 2);	/* そのまま cbuffer にコピー */
			lcount++;												/* 読み込みカウンタをインクリメント */
			lresultcount++;											/* 書き込みカウンタをインクリメント */
		} else {													/* その他の文字の場合 */
			sprintf((char*)ctemp, "%%%02X", cbyte);						/* URL エンコード */
			strncpy((char*)cbuffer + lresultcount, (char*)ctemp, 4);				/* cbuffer にコピー */
			lcount++;												/* 読み込みカウンタをインクリメント */
			lresultcount += 3;										/* 書き込みカウンタを 3 増やす */
		}
		if(lcount == llength) { break; }							/* 文字列の終端に達した場合、ループを抜ける */
	}
	return ;
}















// http://www.emoticode.net/c/urlencode-in-plain-c.html
static char hex[] = "0123456789abcdef";

char i2a(char code) {
	return hex[code & 15];
}

char *urlencodenew(char *pstr)
{
	char
		*buf,
		*pbuf;

	pbuf = buf = (char *)malloc(strlen(pstr) * 3 + 1);

	while (*pstr) {
		if ((*pstr >0 && isalnum(*pstr)) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') {
			*pbuf++ = *pstr;
		}
		else if (*pstr == ' ') {
			*pbuf++ = '+';
		}
		else {
			*pbuf++ = '%';
			*pbuf++ = i2a(*pstr >> 4);
			*pbuf++ = i2a(*pstr & 15);
		}
		pstr++;
	}
	*pbuf = '\0';

	return buf;
}

char *urlencodenew2(char *pstr, size_t size)
{
	char
		*buf,
		*pbuf;

	pbuf = buf = (char *)malloc(size * 3 + 1);

	while (size--) {
		if ((*pstr >0 && isalnum(*pstr)) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') {
			*pbuf++ = *pstr;
		}
		else if (*pstr == ' ') {
			*pbuf++ = '+';
		}
		else {
			*pbuf++ = '%';
			*pbuf++ = i2a(*pstr >> 4);
			*pbuf++ = i2a(*pstr & 15);
		}
		pstr++;
	}
	*pbuf = '\0';

	return buf;
}


static char a2ibyte(char c)
{
	switch (c) {
	case '0': return 0x0;
	case '1': return 0x1;
	case '2': return 0x2;
	case '3': return 0x3;
	case '4': return 0x4;
	case '5': return 0x5;
	case '6': return 0x6;
	case '7': return 0x7;
	case '8': return 0x8;
	case '9': return 0x9;
	case 'a':case 'A': return 0xa;
	case 'b':case 'B': return 0xb;
	case 'c':case 'C': return 0xc;
	case 'd':case 'D': return 0xd;
	case 'e':case 'E': return 0xe;
	case 'f':case 'F': return 0xf;
	}
	return 0;
}
static char a2i(char c1, char c2)
{
	return (a2ibyte(c1) << 4) | (a2ibyte(c2));
}
unsigned char* urldecode(const char* penc, unsigned int* pSize)
{
	unsigned int size = 0;
	unsigned char* pOrig = (unsigned char*)malloc(strlen(penc) + 1);
	unsigned char* p = pOrig;
	for (; *penc; ++penc, ++p, ++size)
	{
		char c = *penc;
		if (c == '%')
		{
			penc++;
			if (!*penc)
				break;
			char c1, c2;
			c1 = *penc;
			penc++;
			if (!*penc)
				break;

			c2 = *penc;
			c = a2i(c1, c2);
		}

		*p = c;
	}
	*p = 0;
	*pSize = size;
	return pOrig;
}
