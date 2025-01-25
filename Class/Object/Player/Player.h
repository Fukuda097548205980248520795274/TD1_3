#pragma once
#include "./Class/Object/Object.h"
#include "./Class/Object/CarryBlock/CarryBlock.h"
#include "./Class/Object/Enemy/Enemy.h"
#include "./Class/Object/CarryBlock/IceGhost/IceGhost.h"
#include "./Class/Scene/Scene.h"
#include "./Class/Object/Particle/SnowCarry/SnowCarry.h"

class Player : public Object
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 配置する
	/// </summary>
	/// <param name="column">列</param>
	/// <param name="row">行</param>
	void Puttting(int column, int row);

	/// <summary>
	/// 操作する
	/// </summary>
	/// <param name="keys">キー</param>
	/// <param name="preKeys">前のキー</param>
	void Operation(const char* keys, const char* preKeys);

	/// <summary>
	/// ブロックの上に着地する
	/// </summary>
	/// <param name="block">ブロック</param>
	void BlockLanding(CarryBlock* block);

	/// <summary>
	/// ブロックを運ぶ
	/// </summary>
	/// <param name="keys">キー</param>
	/// <param name="preKeys">前のキー</param>
	/// <param name="block1">運ぶブロック</param>
	/// <param name="block2">他のブロック</param>
	void Carry(const char* keys, const char* preKeys, CarryBlock* block1, CarryBlock* block2);

	/// <summary>
	/// 描画する
	/// </summary>
	void Draw();


	// 向き
	DIRECTION directionNo;

	// 復活
	struct Respawn respawn_;

	// 歩き
	struct Walk walk_;

	// 運び
	struct Carry carry_;

	// 梯子を使っているかどうか（梯子フラグ）
	int isLadderClimd_;

	// 梯子の上る速度
	float ladderVel_;

	// 左スティック
	int stickLeftX_;
	int stickLeftY_;


	// 運ぶときの雪
	SnowCarry* snowCarry[kParticleSnowCarry];
};