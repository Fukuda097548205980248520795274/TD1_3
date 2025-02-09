#pragma once
#include "./Class/Object/Particle/Particle.h"

class SnowCarry : public Particle
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SnowCarry();

	/// <summary>
	/// 放出する
	/// </summary>
	/// <param name="pos">位置</param>
	void Emission(struct Vec2 pos);

	/// <summary>
	/// 描画する
	/// </summary>
	void Move() override;

	/// <summary>
	/// 描画する
	/// </summary>
	void Draw() override;

	// クールタイム
	static int coolTime;
};

