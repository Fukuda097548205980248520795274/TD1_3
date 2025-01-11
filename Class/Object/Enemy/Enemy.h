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
	/// 出現させる
	/// </summary>
	/// <param name="column">列</param>
	/// <param name="row">行</param>
	void Arrival(int column, int row);

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

	/// <summary>
	/// 描画する
	/// </summary>
	virtual void Draw();

	// IDを数える
	static int countID;

	// ID
	int id_;

	// 出現しているかどうか（出現フラグ）
	int isArrival_;
};