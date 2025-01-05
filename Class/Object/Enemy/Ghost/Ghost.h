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
	/// 出現させる
	/// </summary>
	/// <param name="column">列</param>
	/// <param name="row">行</param>
	void Arrival(int column, int row);

	/// <summary>
	/// 描画する
	/// </summary>
	/// <param name="gh">画像</param>
	void Draw(int gh);

	// IDを数える値
	static int countID;
};