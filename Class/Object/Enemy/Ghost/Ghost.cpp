#include "Ghost.h"

/// <summary>
/// コンストラクタ
/// </summary>
Ghost::Ghost()
{
	// ID
	id_ = 0;

	// 出現しているかどうか（出現フラグ）
	isArrival_ = false;

	// 図形
	shape_.scale = { 18.0f , 18.0f };
	shape_.theta = 0.0f;
	shape_.translate = { 0.0f , 0.0f };

	// 位置
	pos_.local = { {-1.0f , 1.0f} , {1.0f , 1.0f} , {-1.0f , -1.0f} , {1.0f , -1.0f} };
	LocalToScreen();

	// 移動速度
	vel_ = { -2.0f , 0.0f };

	// ジャンプ
	jump_.isJump = false;
	jump_.fallingVel = 0.0f;
}

/// <summary>
/// 出現させる
/// </summary>
/// <param name="column">列</param>
/// <param name="row">行</param>
void Ghost::Arrival(int column, int row)
{
	if (isArrival_ == false)
	{
		// ID
		id_ = ++countID;

		// 出現させる（出現フラグをtrueにする）
		isArrival_ = true;

		// 位置
		shape_.translate = { static_cast<float>(column * kTileSize) + shape_.scale.x ,
			static_cast<float>((kMapRow - 1 - row) * kTileSize) + shape_.scale.y };
		LocalToScreen();

		// ジャンプ
		jump_.isJump = false;
		jump_.fallingVel = 0.0f;

		// 移動速度
		vel_ = { -2.0f , 0.0f };
	}
}

/// <summary>
/// 描画する
/// </summary>
/// <param name="gh">画像</param>
void Ghost::Draw(int gh)
{
	if (isArrival_)
	{
		Novice::DrawQuad
		(
			static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
			static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
			static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
			static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
			0, 0, 1, 1, gh, 0x00FF00FF
		);
	}
}