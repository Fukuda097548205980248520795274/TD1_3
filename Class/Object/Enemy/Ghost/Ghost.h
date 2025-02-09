#pragma once
#include "./Class/Object/Enemy/Enemy.h"

class Ghost : public Enemy
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Ghost();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Ghost();

	/// <summary>
	/// 描画する
	/// </summary>
	void Draw() override;
};