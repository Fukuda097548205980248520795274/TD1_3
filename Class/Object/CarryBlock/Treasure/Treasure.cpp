#include "Treasure.h"

/// <summary>
/// コンストラクタ
/// </summary>
Treasure::Treasure()
{
	// ID
	id_ = 0;

	// 配置しているかどうか（配置フラグ）
	isPut_ = false;

	// 図形
	shape_.scale = { 24.0f , 24.0f };
	shape_.theta = 0.0f;
	shape_.translate = { 0.0f , 0.0f };

	// 位置
	pos_.local = { {-1.0f , 1.0f} , {1.0f , 1.0f} , {-1.0f , -1.0f} , {1.0f , -1.0f} };
	LocalToScreen();

	// 移動速度
	vel_ = { 0.0f , 0.0f };

	// ジャンプ
	jump_.isJump = false;
	jump_.fallingVel = 0.0f;
}

/// <summary>
/// 配置する
/// </summary>
/// <param name="column">列</param>
/// <param name="row">行</param>
void Treasure::Putting(int column, int row)
{
	if (isPut_ == false)
	{
		// ID
		id_ = ++countID;

		// 配置する（配置フラグをtrueにする）
		isPut_ = true;

		// 位置
		shape_.translate = { static_cast<float>(column * kTileSize) + shape_.scale.x ,
			static_cast<float>((kMapRow - 1 - row) * kTileSize) + shape_.scale.y };
		LocalToScreen();

		// ジャンプ
		jump_.isJump = false;
		jump_.fallingVel = 0.0f;

		// 移動速度
		vel_ = { 0.0f , 0.0f };
	}
}

/// <summary>
/// 描画する
/// </summary>
/// <param name="gh">画像</param>
void Treasure::Draw(int gh)
{
	Novice::DrawQuad
	(
		static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
		static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
		static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
		static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
		0, 0, 1, 1, gh, 0x333333FF
	);
}