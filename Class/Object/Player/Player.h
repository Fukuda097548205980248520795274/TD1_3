#pragma once
#include "./Class/Object/Object.h"
#include "./Class/Object/CarryBlock/CarryBlock.h"
#include "./Class/Object/Enemy/Enemy.h"
#include "./Class/Object/CarryBlock/IceGhost/IceGhost.h"

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
	void Carry(const char* keys, const char* preKeys, CarryBlock* block1 , CarryBlock* block2);

	/// <summary>
	/// 描画する
	/// </summary>
	void Draw();
};