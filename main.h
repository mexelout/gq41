//=============================================================================
//
// 概要      :全体に共通する項目に関するコード
// ファイル名:[main.h]
// 作成者    :AT13J295 25 ths00212 杉山元和
//
//=============================================================================
#pragma once

#include <windows.h>

// NULLを入れるためのマクロ
#define NULL_VECTOR3(v) {v.x=v.y=v.z=0;}
#define NULL_COLORVALUE(c) {c.r=c.g=0c.b=c.a=0;}
#define SET_COLORVALUE(c, red, grn, ble, alp) {c.r=red;c.g=grn;c.b=ble;c.a=alp;}
#define NULL_LIGHT(l) {l.Type=D3DLIGHTTYPE(0);NULL_COLORVALUE(l.Diffuse);NULL_COLORVALUE(l.Specular);NULL_COLORVALUE(l.Ambient);NULL_VECTOR3(l.Position);NULL_VECTOR3(l.Direction);l.Range=0;l.Falloff=0;l.Attenuation0=0;l.Attenuation1=0;l.Attenuation2=0;l.Theta=0;l.Phi=0;}

// 落下速度(1/60の速度)
#define GRAVITY(t) (0.16f * t)

// エラー用
#define MB_ERROR (MB_OK | MB_ICONWARNING)

// 色(今後追加)
enum {
	COLOR_WHITE = 0xffffffff,
	COLOR_RED = 0xffff0000,
	COLOR_GREEN = 0xff00ff00,
	COLOR_BLUE = 0xff0000ff,
	COLOR_BLACK = 0xff000000
};

// 色の最大値(byte)
#define COLOR_MAX_BYTE (255)

// 色の最大値(float)
#define COLOR_MAX_FLOAT (1.0f)

//=====================================
// プロトタイプ宣言

// このアプリのインスタンスを取得
HINSTANCE GetInst();
// このアプリのウィンドウハンドルを取得
HWND GetWnd();

// end file