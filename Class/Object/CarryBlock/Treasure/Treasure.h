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
	/// 配置する
	/// </summary>
	/// <param name="column">列</param>
	/// <param name="row">行</param>
	void Putting(int column, int row);

	/// <summary>
	/// 描画する
	/// </summary>
	/// <param name="gh">画像</param>
	void Draw(int gh);

	// IDをカウントする
	static int countID;
};