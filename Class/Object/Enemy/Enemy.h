#pragma once
#include "./Class/Object/Object.h"
#include "./Class/Object/CarryBlock/CarryBlock.h"
#include "./Class/Object/Particle/Hinoko/Hinoko.h"
#include "./Class/Object/Particle/Vapor/Vapor.h"

class Enemy : public Object
{
public:

	/// <summary>
	/// 初期化する
	/// </summary>
	void InitialValue();

	/// <summary>
	/// 出現させる
	/// </summary>
	/// <param name="column">列</param>
	/// <param name="row">行</param>
	void Arrival(int column, int row);

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

	/// <summary>
	/// 描画する
	/// </summary>
	virtual void Draw();

	// 蒸気
	Vapor* vapor[kParticleVapor];

	// IDを数える
	static int countID;

	// ID
	int id_;

	// 出現しているかどうか（出現フラグ）
	int isArrival_;

	// 火の粉
	Hinoko* hinoko_[kParticleHinoko];

	// 火の粉のクールタイム
	int hinokoCoolTime_;

	// 向き
	DIRECTION directionNo;
};