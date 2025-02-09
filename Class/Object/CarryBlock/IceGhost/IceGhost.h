#pragma once
#include "./Class/Object/CarryBlock/CarryBlock.h"
#include "./Class/Object/Enemy/Ghost/Ghost.h"

class IceGhost : public CarryBlock
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IceGhost();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~IceGhost();

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