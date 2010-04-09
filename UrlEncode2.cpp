
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "UrlEncode2.h"

void UrlEncode2(const unsigned char	*csource,
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
