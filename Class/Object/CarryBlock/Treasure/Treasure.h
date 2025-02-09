#pragma once
#include "./Class/Object/CarryBlock/CarryBlock.h"

class Treasure : public CarryBlock
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Treasure();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Treasure();

	/// <summary>
	/// 動かす
	/// </summary>
	void Move() override;

	/// <summary>
	/// ブロックの上に着地する
	/// </summary>
	/// <param name="block"></param>
	void BlockLanding(CarryBlock* block) override;

	/// <summary>
	/// 描画する
	/// </summary>
	void Draw() override;
};