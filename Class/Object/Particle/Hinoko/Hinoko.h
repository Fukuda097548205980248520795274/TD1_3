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

	// 不透明度
	int alpha_;

	// 角度
	float theta_;
};