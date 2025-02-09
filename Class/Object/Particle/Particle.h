#pragma once
#include "./Class/Object/Object.h"

class Particle : public Object
{
public:

	/// <summary>
	/// 動かす
	/// </summary>
	virtual void Move();

	/// <summary>
	/// 描画する
	/// </summary>
	virtual void Draw();

	// 放出しているかどうか（放出フラグ）
	int isEmission_;

	// ベクトル
	struct Vec2 vec_;

	// フレーム
	struct Frame frame_;
};