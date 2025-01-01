#pragma once
#include "./Class/Object/Object.h"

class CarryBlock : public Object
{
public:

	/// <summary>
	/// コンスタンス
	/// </summary>
	CarryBlock(float posX , float posY);

	/// <summary>
	/// 配置する
	/// </summary>
	/// <param name="posX">X軸の位置</param>
	/// <param name="posY">Y軸の位置</param>
	void Putting(float posX, float posY);

	/// <summary>
	/// 動かす
	/// </summary>
	void Move();

	/// <summary>
	/// ブロックの上に着地する
	/// </summary>
	/// <param name="block">ブロック</param>
	void BlockLanding(CarryBlock* block);

	/// <summary>
	/// 描画する
	/// </summary>
	/// <param name="gh">画像</param>
	void Draw(int gh);

	// 配置しているかどうか（配置フラグ）
	int isPut_;

	// ID
	int id_;
};