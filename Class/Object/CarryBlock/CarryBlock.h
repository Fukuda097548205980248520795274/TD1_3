#pragma once
#include "./Class/Object/Object.h"
#include "./Class/Object/Particle/Landing/Landing.h"
#include "./Class/Object/Particle/Cold/Cold.h"

class CarryBlock : public Object
{
public:

	/// <summary>
	/// 初期化する
	/// </summary>
	void InitialValue();

	/// <summary>
	/// 配置する
	/// </summary>
	/// <param name="column">列</param>
	/// <param name="row">行</param>
	virtual void Putting(int column, int row);

	/// <summary>
	/// 動かす
	/// </summary>
	virtual void Move();

	/// <summary>
	/// ブロックの上に着地する
	/// </summary>
	/// <param name="block">ブロック</param>
	virtual void BlockLanding(CarryBlock* block);

	/// <summary>
	/// 描画する
	/// </summary>
	virtual void Draw();

	// IDをカウントする
	static int countID;

	// ID
	int id_;

	// ブロックの種類
	BLOCK type_;

	// 配置しているかどうか（配置フラグ）
	int isPut_;

	// 上に乗っかっているかどうか（乗っかかりフラグ）
	int isUnderRide_;

	// 爆発しそうかどうか（爆発フラグ）
	int isExplosion_;

	// フレーム
	struct Frame frame_;

	// 着地したときの粒子
	Landing* landing_[kParticleLanding];

	// 冷気
	Cold* cold_[kParticleCold];
};