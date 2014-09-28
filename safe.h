/** SAFE～のマクロ郡
* @author HAL東京ゲーム学科3年 25 杉山元和
*/
#ifndef _SAFE_H_
#define _SAFE_H_

#define SAFE_NEW(p, type) {if(p==NULL){p=new type;}}		// メモリ確保
#define SAFE_RELEASE(p) {if(p){(p)->Release();(p)=NULL;}}	// インスタンス開放
#define SAFE_DELETE(p) {if(p){delete p;p=NULL;}}			// メモリ開放
#define SAFE_DELETE_ARRAY(p) {if(p){delete[]p;p=NULL;}}		// 配列メモリ領域開放
#define SAFE_UNINIT(p) {if(p){p->Uninit();SAFE_DELETE(p);}}	// 後処理後メモリ開放
#define SAFE_UNINIT_RELEASE(p) {if(p){p->Uninit();p->Release();p = NULL;}}

#endif // _SAFE_H_