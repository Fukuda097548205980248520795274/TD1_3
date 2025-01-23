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
	/// 放出する8
	/// </summary>
	void Emission(float posX, float posY, int time);

	/// <summary>
	/// 動かす
	/// </summary>
	void Move();

	/// <summary>
	/// 描画する
	/// </summary>
	void Draw();

	// IDを数える値
	static int countId;

	// クールタイム
	static int coolTime;

	// 不透明度
	int alpha_;

	float theta_;

	int id_;
};