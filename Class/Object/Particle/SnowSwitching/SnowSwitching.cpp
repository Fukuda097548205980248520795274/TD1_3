#include "SnowSwitching.h"

// クールタイム
int SnowSwitching::coolTime = 0;

/// <summary>
/// コンストラクタ
/// </summary>
SnowSwitching::SnowSwitching()
{
	// 放出しているかどうか（放出フラグ）
	isEmission_ = false;

	// 図形
	shape_.scale = { 0.0f , 0.0f };
	shape_.theta = 0.0f;
	shape_.translate = { 0.0f , 0.0f };

	// 位置
	pos_.local = { {-1.0f , 1.0f} , {1.0f , 1.0f} , {-1.0f , -1.0f} , {1.0f , -1.0f} };
	LocalToScreen();

	// ベクトル
	vec_ = { 0.0f , 0.0f };

	// 移動速度
	vel_ = { 0.0f , 0.0f };

	// ジャンプ
	jump_.isJump = false;
	jump_.fallingVel = 0.0f;

	// 乗っかっているかどうか（乗っかりフラグ）
	isRide_ = false;

	// フレーム
	frame_ = { 0 , 0 , 0.0f };

	// 画像
	gh_ = 0;
}

/// <summary>
/// 放出する
/// </summary>
/// <param name="pos">位置</param>
void SnowSwitching::Emission(struct Vec2 pos)
{
	// 放出されている（放出フラグがtrueでえある）ときは、放出しない
	if (isEmission_)
	{
		return;
	}


	// 放出する（放出フラグがtrueになる）
	isEmission_ = true;

	// 画像
	int gh = rand() % 4;
	if (gh == 0)
	{
		gh_ = Novice::LoadTexture("./Resources/Images/Particle/snow_crystal_1.png");
	}
	else if (gh == 1)
	{
		gh_ = Novice::LoadTexture("./Resources/Images/Particle/snow_crystal_2.png");
	}
	else if (gh == 2)
	{
		gh_ = Novice::LoadTexture("./Resources/Images/Particle/snow_crystal_3.png");
	}
	else
	{
		gh_ = Novice::LoadTexture("./Resources/Images/Particle/snow_crystal_4.png");
	}

	// 大きさ
	float scale = static_cast<float>(100 + rand() % 150);
	shape_.scale = { scale , scale };

	// 角度
	float theta = static_cast<float>(rand() % 360);
	shape_.theta = theta;

	// 位置
	shape_.translate = pos;
	LocalToScreen();

	// 移動速度
	vel_ = { -16.0f , 0.0f };
}

/// <summary>
/// 動かす
/// </summary>
void SnowSwitching::Move()
{
	// 放出していない（放出フラグがfalseである）ときは、処理しない
	if (isEmission_ == false)
	{
		return;
	}


	// 回転させる
	shape_.theta += 0.5f;

	if (shape_.theta >= 360.0f)
	{
		shape_.theta = 0.0f;
	}

	// 動かす
	shape_.translate.x += vel_.x;
	shape_.translate.y += vel_.y;
	LocalToScreen();

	// 画面の下に行ったら、消える（放出フラグがfalseになる）
	if (shape_.translate.x + shape_.scale.x < 0.0f)
	{
		isEmission_ = false;
	}
}

/// <summary>
/// 描画する
/// </summary>
void SnowSwitching::Draw()
{
	// 放出してない（放出フラグがfalseである）ときは、描画しない
	if (isEmission_ == false)
	{
		return;
	}

	Novice::DrawQuad
	(
		static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
		static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
		static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
		static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
		0, 0, 96, 96, gh_, 0xFFFFFFAA
	);
}