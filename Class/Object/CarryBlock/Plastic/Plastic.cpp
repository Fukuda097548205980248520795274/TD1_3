#include "Plastic.h"

/// <summary>
/// コンストラクタ
/// </summary>
Plastic::Plastic()
{
	// ID
	id_ = 0;

	// 配置しているかどうか（配置フラグ）
	isPut_ = false;

	// 種類
	type_ = BLOCK_PLASTIC;

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

	// 乗っかりフラグ
	isRide_ = false;

	// 乗っかかりフラグ
	isUnderRide_ = false;

	// 凍っている時間
	freezeTimer_ = 0;
}

/// <summary>
/// 描画する
/// </summary>
void Plastic::Draw()
{
	// 配置されていない（配置フラグがfalseである）ときは、描画しない
	if (isPut_ == false)
	{
		return;
	}

	// 白い画像
	int ghWhite = Novice::LoadTexture("./NoviceResources/white1x1.png");

	Novice::DrawQuad
	(
		static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
		static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
		static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
		static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
		0, 0, 1, 1, ghWhite, 0xFFFFFFAA
	);
}