#include "Cushion.h"


/// <summary>
/// コンストラクタ
/// </summary>
Cushion::Cushion()
{
	// ID
	id_ = 0;

	// 配置しているかどうか（配置フラグ）
	isPut_ = false;

	// 種類
	type_ = BLOCK_CUSHION;

	// 図形
	shape_.scale = { 23.0f , 24.0f };
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

	// 爆発しそうか（爆発フラグ）
	isExplosion_ = false;

	// 凍っている時間
	frame_ = { 0 , 0 , 0.0f };

	// 着地したときの粒子
	for (int i = 0; i < kParticleLanding; i++)
	{
		landing_[i] = new Landing();
	}

	// 冷気
	for (int i = 0; i < kParticleCold; i++)
	{
		cold_[i] = new Cold();
	}
}

/// <summary>
/// デストラクタ
/// </summary>
Cushion::~Cushion()
{
	// 着地したときの粒子
	for (int i = 0; i < kParticleLanding; i++)
	{
		delete landing_[i];
	}

	// 冷気
	for (int i = 0; i < kParticleCold; i++)
	{
		delete cold_[i];
	}
}

/// <summary>
/// 描画する
/// </summary>
void Cushion::Draw()
{
	// 配置されていない（配置フラグがfalseである）ときは、描画しない
	if (isPut_ == false)
	{
		return;
	}

	// 白い画像
	/*int ghWhite = Novice::LoadTexture("./NoviceResources/white1x1.png");*/

	int ghCushionBlock = Novice::LoadTexture("./Resources/Images/Map/cushion.png");

	Novice::DrawQuad
	(
		static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
		static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
		static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
		static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
		0, 0, 48, 48, ghCushionBlock, 0xFFFFFFFF
	);
}