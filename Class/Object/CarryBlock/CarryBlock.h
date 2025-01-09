#pragma once
#include "./Class/Object/Object.h"

class CarryBlock : public Object
{
public:

	/// <summary>
	/// 動かす
	/// </summary>
	virtual void Move();

	/// <summary>
	/// ブロックの上に着地する
	/// </summary>
	/// <param name="block">ブロック</param>
	virtual void BlockLanding(CarryBlock* block);

	// 配置しているかどうか（配置フラグ）
	int isPut_;

	// ID
	int id_;

	// 上に乗っかっているかどうか（乗っかかりフラグ）
	int isUnderRide_;
};