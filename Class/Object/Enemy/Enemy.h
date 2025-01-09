#pragma once
#include "./Class/Object/Object.h"
#include "./Class/Object/CarryBlock/CarryBlock.h"

class Enemy : public Object
{
public:

	/// <summary>
	/// 動かす
	/// </summary>
	void Move();

	/// <summary>
	/// ブロックに着地する
	/// </summary>
	/// <param name="block">ブロック</param>
	void BlockLanding(CarryBlock* block);

	/// <summary>
	/// 敵を挟みつぶす
	/// </summary>
	/// <param name="block1">ブロック1</param>
	/// <param name="block2">ブロック2</param>
	void Death(CarryBlock* block1 , CarryBlock* block2);

	// ID
	int id_;

	// 出現しているかどうか（出現フラグ）
	int isArrival_;
};