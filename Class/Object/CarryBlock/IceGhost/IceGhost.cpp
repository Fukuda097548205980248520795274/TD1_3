#include "IceGhost.h"

/// <summary>
/// コンストラクタ
/// </summary>
IceGhost::IceGhost()
{
	// ID
	id_ = 0;

	// 配置しているかどうか（配置フラグ）
	isPut_ = false;

	// 種類
	type_ = BLOCK_ICE_GHOST;

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
IceGhost::~IceGhost()
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
/// 配置する
/// </summary>
/// <param name="column">列</param>
/// <param name="row">行</param>
void IceGhost::Putting(int column, int row)
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

		// 凍っている時間
		frame_ = { 0 , 600 , 0.0f };

		// パーティクルを消す
		for (int i = 0; i < kParticleLanding; i++)
		{
			landing_[i]->isEmission_ = false;
		}
	}
}

/// <summary>
/// 動かす
/// </summary>
void IceGhost::Move()
{
	// 配置されていない（配置フラグがfalseである）ときは、動かさない
	if (isPut_ == false)
	{
		return;
	}


	/*   ジャンプ   */

	// 空中にいると、自動でジャンプ状態になる（ジャンプフラグがtrueになる）
	if (jump_.isJump == false)
	{
		// 一時的にマップを更新する
		MapUpdate(0.0f, -1.0f);

		if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_NOTHING &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_NOTHING ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_LADDER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER)
		{
			jump_.isJump = true;
			jump_.fallingVel = -0.5f;

			// 速度に変化なし
			vel_.y = 0.0f;
		}
	}

	// ジャンプしている（ジャンプフラグがtrueである）ときの動き
	if (jump_.isJump)
	{
		// ブロックのサイズまで、落下する
		if (vel_.y < static_cast<float>(kTileSize) && vel_.y > -static_cast<float>(kTileSize))
		{
			vel_.y += jump_.fallingVel;
		}

		// マップを一時的に更新する
		MapUpdate(0.0f, vel_.y);

		// 上に飛んでいるとき
		if (vel_.y > 0.0f)
		{
			// 飛んだ先に天井があったら、天井にぶつかる
			if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_GROUND ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_GROUND ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_BLOCK ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_BLOCK ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_ROTTED ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_ROTTED ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] < 0 || Map::map_[map_.rightTop.row][map_.rightTop.column] < 0)
			{
				// 押し込み処理
				shape_.translate.y = static_cast<float>(kScreenHeight - map_.leftTop.row * kTileSize - kTileSize) - shape_.scale.y;
				LocalToScreen();

				vel_.y = 0.0f;
			}
		}
		else
		{
			// 下に落下しているとき

			// 落ちた先に地面があったら、着地する（ジャンプフラグがfalseになる）
			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GROUND ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GROUND ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_BLOCK ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_BLOCK ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_ROTTED ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_ROTTED ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] < 0 || Map::map_[map_.rightBottom.row][map_.rightBottom.column] < 0)
			{
				// 押し込み処理
				shape_.translate.y = static_cast<float>(kScreenHeight - map_.leftBottom.row * kTileSize) + shape_.scale.y;
				LocalToScreen();

				jump_.isJump = false;
				jump_.fallingVel = 0.0f;

				vel_.y = 0.0f;
			}
		}
	}

	// 動かす
	shape_.translate.y += vel_.y;
	LocalToScreen();


	/*   解凍   */

	// フレームを進める
	frame_.current++;

	// 凍っている時間が終わったら、ブロックが消える（配置フラグがfalseになる）
	if (frame_.current >= frame_.end)
	{
		isPut_ = false;

		// IDを消す
		id_ = 0;
		--countID;
	}
}

/// <summary>
/// 描画する
/// </summary>
void IceGhost::Draw()
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
		0, 0, 1, 1, ghWhite, 0x33FFFFFF
	);
}