#include "SnowCarry.h"

// クールタイム
int SnowCarry::coolTime = 0;

/// <summary>
/// コンストラクタ
/// </summary>
SnowCarry::SnowCarry()
{
	// 放出しているかどうか（放出フラグ）
	isEmission_ = false;

	// 図形
	shape_.scale = { 4.0f , 4.0f };
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
}

/// <summary>
/// 放出する
/// </summary>
/// <param name="pos">位置</param>
void SnowCarry::Emission(struct Vec2 pos)
{
	// 放出されている（放出フラグがtrueでえある）ときは、放出しない
	if (isEmission_)
	{
		return;
	}


	// 放出する（放出フラグがtrueになる）
	isEmission_ = true;

	// 大きさ
	shape_.scale = { 0.0f , 0.0f };

	// 位置
	shape_.translate = pos;
	LocalToScreen();

	// フレーム
	frame_ = { 0 , 60 , 0.0f };
}

/// <summary>
/// 動かす
/// </summary>
void SnowCarry::Move()
{
	// 放出していない（放出フラグがfalseである）ときは、処理しない
	if (isEmission_ == false)
	{
		return;
	}


	// 0 ~ 10フレームで大きくする
	if (frame_.current >= 0 && frame_.current < 10)
	{
		frame_.rate = static_cast<float>(frame_.current) / 10.0f;

   		shape_.scale.x = (1.0f - frame_.rate) * 0.0f + frame_.rate * 8.0f;
		shape_.scale.y = (1.0f - frame_.rate) * 0.0f + frame_.rate * 8.0f;
	}

	// 10 ~ 60フレームで小さくする
	if (frame_.current >= 10 && frame_.current < 60)
	{
		frame_.rate = static_cast<float>(frame_.current - 10) / 50.0f;

		shape_.scale.x = (1.0f - frame_.rate) * 8.0f + frame_.rate * 0.0f;
		shape_.scale.y = (1.0f - frame_.rate) * 8.0f + frame_.rate * 0.0f;
	}

	
	LocalToScreen();


	// フレームを進める
	frame_.current++;

	// フレームが終了したら、消える（放出フラグがfalseになる）
	if (frame_.current > frame_.end)
	{
		isEmission_ = false;
	}
}

/// <summary>
/// 描画する
/// </summary>
void SnowCarry::Draw()
{
	// 放出してない（放出フラグがfalseである）ときは、描画しない
	if (isEmission_ == false)
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
		0, 0, 48, 48, ghWhite, 0xFFFFFFAA
	);
}