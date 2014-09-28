#pragma once

template<typename T>class Singleton {
public:
	/** インスタンス取得
	* @return T& こいつ
	*/
	static T& inst() {
		static T i;
		return i;
	}
protected:
	/**
	* コンストラクタ
	*/
	Singleton() {}
	/**
	* デストラクタ
	*/
	~Singleton() {}
private:
	/** コピーコンストラクタ
	* 呼び出し禁止
	*/
	Singleton(const Singleton& s) {(*this) = s}

	/** 代入演算子
	* 呼び出し禁止
	*/
	Singleton& operator = (const Singleton &s) {(*this) = s}
};
