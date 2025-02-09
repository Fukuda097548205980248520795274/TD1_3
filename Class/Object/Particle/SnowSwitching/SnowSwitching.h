#pragma once
#include "./Class/Object/Particle/Particle.h"

class SnowSwitching : public Particle
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SnowSwitching();

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

	// 画像
	int gh_;

	// クールタイム
	static int coolTime;
};

