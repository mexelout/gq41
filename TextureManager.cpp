#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include "TextureManager.h"
#include "safe.h"
#include "main.h"

/* コンストラクタ*/
TextureManager::TextureManager() {
	idx_apply = 0;
	TEXTURE_DATA d;
	d.filename = "";
	memset(&d.info, 0, sizeof(D3DXIMAGE_INFO));
	d.texture = NULL;
	data.push_back(d);
}
/* デストラクタ*/
TextureManager::~TextureManager() {
	releaseAll();
}
/** テクスチャ読み込み\n
* スタックに入りきらなかったら0が返ってくる\m
* 読み込み失敗でも0が返ってくる
* @param pFilename ファイル名
* @param pDevice 描画デバイス
* @return 整理番号
*/
UINT TextureManager::loadTexture(std::string filename, LPDIRECT3DDEVICE9 device) {
	TextureManager& t = TextureManager::inst();

	// 同じもの読み込み禁止
	for(UINT i = 0, len = t.data.size(); i < len; ++i) {
		if(filename == t.data[i].filename) {
			return i;
		}
	}

	HRESULT hr;
	TEXTURE_DATA d;
	hr = D3DXGetImageInfoFromFile(filename.c_str(), &d.info);

	// ファイルの有無
	if(SUCCEEDED(hr)) {
		// 有るとわかったら色々保存
		d.filename = filename;

		// テクスチャバッファ読み込み
		D3DXCreateTextureFromFileEx(device, filename.c_str(), d.info.Width, d.info.Height, 1, 0, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0x00000000, NULL, NULL, &d.texture);
		t.data.push_back(d);

		return t.data.size() - 1;
	} else {
		// 開けねぇ！？
		MessageBox(NULL, "画像が読み込めねぇよ！", "エラー", MB_ERROR);
		return 0;
	}
}
/** テクスチャ適用
* @param nIdxNum 整理番号
* @param dwStage ステージ番号
* @param pDevice 描画デバイス
*/
void TextureManager::applyTexture(UINT idx, DWORD stage, LPDIRECT3DDEVICE9 device) {
	TextureManager& t = TextureManager::inst();

	// 最大以上ならさっさと抜ける
	if(idx >= t.data.size()) return;

	// 既に適用済みならさっさと抜ける
	if(t.idx_apply == idx) return;

	// テクスチャ適用
	device->SetTexture(stage, t.data[idx].texture);

	// 今回設定した番号を保存(次回適用しない為)
	t.idx_apply = idx;
}
/** テクスチャの情報を取得\n
* 変な数値を引数にするとNULLが返ってくる
* @param nIdxNum 整理番号
* @return テクスチャの情報が返ってくる(変更不可)
*/
const D3DXIMAGE_INFO* TextureManager::getImageInfo(UINT idx) {
	TextureManager& t = TextureManager::inst();
	if(idx >= t.data.size()) return NULL;
	return &t.data[idx].info;
}
/** テクスチャを全開放&スタック消去
*/
void TextureManager::releaseAll() {
	TextureManager& t = TextureManager::inst();
	for each(TEXTURE_DATA d in t.data) {
		SAFE_RELEASE(d.texture);
	}
	t.data.clear();
	t.idx_apply = 0;
}
/** テクスチャを再読み込み(デフォルト指定にする為)
*/
void TextureManager::resetLoad(LPDIRECT3DDEVICE9 device) {
	TextureManager& t = TextureManager::inst();
}

// end file