#pragma once
#include "./Class/Object/CarryBlock/CarryBlock.h"

class Bomb : public CarryBlock
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Bomb();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Bomb();

	/// <summary>
	/// 配置する
	/// </summary>
	/// <param name="column">列</param>
	/// <param name="row">行</param>
	void Putting(int column, int row) override;

	/// <summary>
	/// 動かす
	/// </summary>
	void Move() override;

	/// <summary>
	/// 描画する
	/// </summary>
	void Draw() override;
};