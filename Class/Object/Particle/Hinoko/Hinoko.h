#pragma once
#include "./Class/Object/Particle/Particle.h"

class Hinoko : public Particle
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Hinoko();

	/// <summary>
	/// 放出する
	/// </summary>
	void Emission(float posX, float posY, float theta, int time);

	/// <summary>
	/// 動かす
	/// </summary>
	void Move();

	/// <summary>
	/// 描画する
	/// </summary>
	/// <param name="gh">画像</param>
	void Draw(int gh) override;

	// IDを数える値
	static int countId;

	// クールタイム
	static int coolTime;

	// 不透明度
	int alpha_;
};