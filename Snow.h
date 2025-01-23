#pragma once
#include"./Class/Object/Particle/Particle.h"

class Snow : public Particle
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Snow();


	void Emission();

	void Move();

	void Draw();

	int isEmission_;

	struct Vec2 acceleration_;

	float mass_;

	// IDに入れる値
	static int countId;

	// 透明度
	int alpha_;


	// ベクトル
	struct Vec2 vec_;

	// フレーム
	struct Frame frame_;

	float SnoWGravity_;
};

