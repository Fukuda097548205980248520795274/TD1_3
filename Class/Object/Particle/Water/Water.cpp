#include "Water.h"

// クールタイム
int Water::coolTime = 0;

/// <summary>
/// コンストラクタ
/// </summary>
Water::Water()
{
	// 放出しているかどうか（放出フラグ）
	isEmission_ = false;

	// 図形
	shape_.scale = { 1.0f , 1.0f };
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

	// 不透明度
	alpha_ = 0;
}

/// <summary>
/// 放出する
/// </summary>
/// <param name="pos">位置</param>
void Water::Emission(struct Vec2 pos)
{
	// 放出されている（放出フラグがtrueでえある）ときは、放出しない
	if (isEmission_)
	{
		return;
	}


	// 放出する（放出フラグがtrueになる）
	isEmission_ = true;

	// 大きさ
	shape_.scale = { 2.0f , 2.0f };

	// 位置
	shape_.translate = pos;
	LocalToScreen();

	// ベクトル
	float theta = static_cast<float>(rand() % 120) + 30.0f;
	vec_ = { cosf((theta / 180.0f) * float(M_PI)) , sinf((theta / 180.0f) * float(M_PI)) };

	// 移動速度
	vel_ = { 0.0f , 0.0f };

	// フレーム
	frame_ = { 0 , 0 , 0.0f };

	// 不透明度
	alpha_ = 0;
}

/// <summary>
/// 動かす
/// </summary>
void Water::Move()
{
	// 放出していない（放出フラグがfalseである）ときは、処理しない
	if (isEmission_ == false)
	{
		return;
	}


	// 動かす
	shape_.translate.x += vel_.x * vec_.x;
	shape_.translate.y += vel_.y * vec_.y;
	LocalToScreen();


	// 透明度を下げる
	if (frame_.current >= 0 && frame_.current < 60)
	{
		frame_.rate = static_cast<float>(frame_.current) / 60.0f;
		alpha_ = static_cast<int>((1.0f - frame_.rate) * 0.0f + frame_.rate * 192.0f);
	}

	// 徐々に落下する
	if (frame_.current >= 60)
	{
		if (vel_.y > -static_cast<float>(kTileSize))
		{
			vel_.y -= 0.2f;
		}
	}

	// フレームを進める
	frame_.current++;

	// 地面についたら、消滅する（放出フラグがfalseになる）
	if (vel_.y < 0.0f)
	{
		if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GROUND ||
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GROUND ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_BLOCK ||
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_BLOCK ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_ROTTED ||
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_ROTTED ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER ||
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER)
		{
			isEmission_ = false;

			// 水滴音
			int shWaterDrop = Novice::LoadAudio("./Resources/Sounds/Se/waterDrop.mp3");
			Novice::PlayAudio(shWaterDrop , 0 , 0.05f);
		}
	}

	// 画面下を越えたら、消滅する
	if (shape_.translate.y + shape_.scale.y < 0.0f)
	{
		isEmission_ = false;
	}
}

/// <summary>
/// 描画する
/// </summary>
void Water::Draw()
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
		0, 0, 48, 48, ghWhite, 0x0000FF00 + alpha_
	);
}