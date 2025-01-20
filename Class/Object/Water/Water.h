#pragma once
#include "./Class/Object/Object.h"

class Water : public Object
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Water();

	/// <summary>
	/// 放出する
	/// </summary>
	/// <param name="column">列</param>
	/// <param name="row">行</param>
	void Emission(int column , int row);

	/// <summary>
	/// 動かす
	/// </summary>
	void Move();

	/// <summary>
	/// 描画する
	/// </summary>
	void Draw();

	// 放出しているかどうか（放出フラグ）
	int isEmission_;

	// 加速度
	struct Vec2 acceleration_;

	// 質量
	float mass_;
};