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

	// 火の粉のクールタイム
	hinokoCoolTime_ = 0;

	// 向き
	directionNo = DIRECTION_RIGHT;

	// 火の粉
	for (int i = 0; i < kParticleHinoko; i++)
	{
		hinoko_[i] = new Hinoko();
	}

	// 蒸気
	for (int i = 0; i < kParticleVapor; i++)
	{
		vapor[i] = new Vapor();
	}
}

/// <summary>
/// デストラクタ
/// </summary>
Ghost::~Ghost()
{
	// 火の粉
	for (int i = 0; i < kParticleHinoko; i++)
	{
		delete hinoko_[i];
	}

	// 蒸気
	for (int i = 0; i < kParticleVapor; i++)
	{
		delete vapor[i];
	}
}

/// <summary>
/// 描画する
/// </summary>
void Ghost::Draw()
{
	// 出現していない（出現フラグがfalseである）敵は描画しない
	if (isArrival_ == false)
	{
		return;
	}

	int ghEnemy = Novice::LoadTexture("./Resources/Images/Enemy/FireEnemy1.png");

	if (directionNo == DIRECTION_RIGHT)
	{
		Novice::DrawQuad
		(
			static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
			static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
			static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
			static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
			0, 0, 48, 48, ghEnemy, 0xFFFFFFFF
		);
	}
	else if (directionNo == DIRECTION_LEFT)
	{
		Novice::DrawQuad
		(
			static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
			static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
			static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
			static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
			0, 0, 48, 48, ghEnemy, 0xFFFFFFFF
		);
	}

}