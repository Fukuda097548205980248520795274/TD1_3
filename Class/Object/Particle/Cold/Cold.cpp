#include "Cold.h"

// クールタイム
int Cold::coolTime = 0;

/// <summary>
/// コンストラクタ
/// </summary>
Cold::Cold()
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
	alpha_ = 255;
}

/// <summary>
/// 放出する
/// </summary>
/// <param name="pos">位置</param>
void Cold::Emission(struct Vec2 pos)
{
	// 放出されている（放出フラグがtrueでえある）ときは、放出しない
	if (isEmission_)
	{
		return;
	}


	// 放出する（放出フラグがtrueになる）
	isEmission_ = true;

	// 大きさ
	shape_.scale = { 1.0f , 1.0f };

	// 位置
	shape_.translate = pos;
	LocalToScreen();

	// ベクトル
	float theta = static_cast<float>(rand() % 120) + 30.0f;
	vec_ = { cosf((theta / 180.0f) * float(M_PI)) , sinf((theta / 180.0f) * float(M_PI)) };

	// 移動速度
	vel_ = { 0.0f , 0.0f };

	// フレーム
	frame_ = { 0 , 60 + rand() % 30 , 0.0f};

	// 不透明度
	alpha_ = 128;
}

/// <summary>
/// 動かす
/// </summary>
void Cold::Move()
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

	// 徐々に落下する
	if (vel_.y > -static_cast<float>(kTileSize))
	{
		vel_.y -= 0.01f;
	}


	// 透明度を下げる
	frame_.rate = static_cast<float>(frame_.current) / static_cast<float>(frame_.end);
	alpha_ = static_cast<int>((1.0f - frame_.rate) * 128.0f + frame_.rate * 0.0f);


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
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] < 0 || Map::map_[map_.rightBottom.row][map_.rightBottom.column] < 0)
		{
			isEmission_ = false;
		}
	}

	// フレームが終了したら、消える（放出フラグがfalseになる）
	if (frame_.current > frame_.end)
	{
		isEmission_ = false;
	}
}

/// <summary>
/// 描画する
/// </summary>
void Cold::Draw()
{
	// 放出してない（放出フラグがfalseである）ときは、描画しない
	if (isEmission_ == false)
	{
		return;
	}

	// 白い画像
	int ghWhite = Novice::LoadTexture("./NoviceResources/white1x1.png");

	Novice::SetBlendMode(kBlendModeAdd);

	Novice::DrawQuad
	(
		static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
		static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
		static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
		static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
		0, 0, 48, 48, ghWhite, 0xFFFFFF00 + alpha_
	);

	Novice::SetBlendMode(kBlendModeNormal);
}