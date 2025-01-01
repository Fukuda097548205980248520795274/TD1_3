#include "CarryBlock.h"

/// <summary>
/// コンストラクタ
/// </summary>
CarryBlock::CarryBlock(float posX , float posY)
{
	// 図形
	shape_.scale = { static_cast<float>(kTileSize / 2) , static_cast<float>(kTileSize / 2) };
	shape_.theta = 0.0f;
	shape_.translate = { posX , posY };

	// 位置
	pos_.local = { {-1.0f , 1.0f} , {1.0f , 1.0f} , {-1.0f , -1.0f} , {1.0f , -1.0f} };
	LocalToScreen();

	// 移動速度
	vel_ = { 0.0f , 0.0f };

	// ジャンプ
	jump_.isJump = false;
	jump_.fallingVel = 0.0f;

	isPut_ = true;

	id_ = 0;
}

/// <summary>
/// 配置する
/// </summary>
/// <param name="posX">X軸の位置</param>
/// <param name="posY">Y軸の位置</param>
void CarryBlock::Putting(float posX, float posY)
{
	if (isPut_ == false)
	{
		shape_.translate = { posX , posY };
	}
}

/// <summary>
/// 動かす
/// </summary>
void CarryBlock::Move()
{
	if (isPut_)
	{
		/*   ジャンプ   */

		// 空中にいると、自動でジャンプ状態になる（ジャンプフラグがtrueになる）
		if (jump_.isJump == false)
		{
			// 一時的にマップを更新する
			MapUpdate(0.0f, -1.0f);

			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_NOTHING &&
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_NOTHING)
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
				if (Map::map_[map_.leftTop.row][map_.leftTop.column] != TILE_NOTHING ||
					Map::map_[map_.rightTop.row][map_.rightTop.column] != TILE_NOTHING)
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
				if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] != TILE_NOTHING ||
					Map::map_[map_.rightBottom.row][map_.rightBottom.column] != TILE_NOTHING)
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
	}
}

/// <summary>
/// ブロックの上に着地する
/// </summary>
/// <param name="block">ブロック</param>
void CarryBlock::BlockLanding(CarryBlock* block)
{
	// null を探す
	if (block == nullptr)
	{
		return;
	}


	// ジャンプしている（ジャンプフラグがtrueである）ときに、ブロックの上に着地できる
	if (jump_.isJump)
	{
		// 落下中のみ
		if (vel_.y < 0.0f)
		{
			if (block->isPut_)
			{
				if (shape_.translate.y - shape_.scale.y < block->shape_.translate.y + block->shape_.scale.y &&
					shape_.translate.y - shape_.scale.y > block->shape_.translate.y - block->shape_.scale.y)
				{
					if (shape_.translate.x + shape_.scale.x > block->shape_.translate.x - block->shape_.scale.x &&
						shape_.translate.x - shape_.scale.x < block->shape_.translate.x + block->shape_.scale.x)
					{
						// 押し込み処理
						shape_.translate.y = block->shape_.translate.y + block->shape_.scale.y + shape_.scale.y;
						LocalToScreen();

						jump_.isJump = false;
						jump_.fallingVel = 0.0f;

						vel_.y = 0.0f;
					}
				}
			}
		}
	}
}

/// <summary>
/// 描画する
/// </summary>
/// <param name="gh">画像</param>
void CarryBlock::Draw(int gh)
{
	Novice::DrawQuad
	(
		static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
		static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
		static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
		static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
		0, 0, 1, 1, gh, 0x000000FF
	);
}