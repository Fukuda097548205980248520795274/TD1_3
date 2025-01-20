#pragma once
#include "./Class/Object/Particle/Particle.h"

class Dust : public Particle
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Dust();

	/// <summary>
	/// 放出する
	/// </summary>
	/// <param name="posX">X軸の位置</param>
	/// <param name="posY">Y軸の位置</param>
	/// <param name="theta">角度</param>
	void Emission(float posX, float posY, float theta);

	/// <summary>
	/// 動かす
	/// </summary>
	void Move();

	/// <summary>
	/// 描画する
	/// </summary>
	/// <param name="gh">画像</param>
	void Draw(int gh) ;

	// IDに入れる値
	static int countId;

	// 透明度
	int alpha_;
};

