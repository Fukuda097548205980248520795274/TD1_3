#pragma once
#include "./Class/Object/CarryBlock/CarryBlock.h"

class Plastic : public CarryBlock
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Plastic();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Plastic();

	/// <summary>
	/// 描画する
	/// </summary>
	void Draw() override;
};