#pragma once
#include "./Class/Object/CarryBlock/CarryBlock.h"

class Cushion : public CarryBlock
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Cushion();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Cushion();

	/// <summary>
	/// 描画する
	/// </summary>
	void Draw() override;
};