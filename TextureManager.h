#pragma once

#include <string>
#include <vector>
#include <d3dx9.h>
#include "Singleton.h"

class TextureManager : public Singleton<TextureManager> {
private:
	friend Singleton; // お友達～お友達～ぷぎゃらぁ！！！！！！
public:
	/** テクスチャ読み込み
	* 読み込み失敗でも0が返ってくる
	* @param filename ファイル名
	* @param device 描画デバイス
	* @return 整理番号
	*/
	UINT loadTexture(std::string filename, LPDIRECT3DDEVICE9 device);
	/** テクスチャ適用
	* @param idx 整理番号
	* @param stage ステージ番号
	* @param device 描画デバイス
	*/
	void applyTexture(UINT idx, DWORD stage, LPDIRECT3DDEVICE9 device);
	/** テクスチャの情報を取得
	* @param idx 整理番号
	* @return テクスチャの情報が返ってくる(変更不可)
	*/
	const D3DXIMAGE_INFO* getImageInfo(UINT idx);
	/** テクスチャを全開放&スタック消去
	*/
	void releaseAll();
	/** テクスチャを再読み込み(デフォルト指定にする為)
	*/
	void resetLoad(LPDIRECT3DDEVICE9 device);
protected:
	/**
	* コンストラクタ
	*/
	TextureManager();
	/**
	* デストラクタ
	*/
	~TextureManager();

	/** 内部構造体
	*/
	typedef struct TEXTURE_DATA{
		std::string filename;
		D3DXIMAGE_INFO info;
		LPDIRECT3DTEXTURE9 texture;
	}TEXTURE_DATA, *LPTEXTURE_DATA;



	/** datas */
	std::vector<TEXTURE_DATA> data;
	/** 適用中画像インデックス*/
	UINT idx_apply;
};
