#pragma once
#include "./Class/Object/Object.h"

class Particle : public Object
{
public:

	Particle();

	/// <summary>
	/// 動かす
	/// </summary>
	virtual void Move();

	// 放出しているかどうか（放出フラグ）
	int isEmission_;

	// ベクトル
	struct Vec2 vec_;

	// フレーム
	struct Frame frame_;
};