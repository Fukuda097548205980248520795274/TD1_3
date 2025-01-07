#include "Treasure.h"

/// <summary>
/// コンストラクタ
/// </summary>
Treasure::Treasure()
{
	// ID
	id_ = 0;

	// 配置しているかどうか（配置フラグ）
	isPut_ = false;

	// 体力
	hp_ = 0;

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
}

/// <summary>
/// 配置する
/// </summary>
/// <param name="column">列</param>
/// <param name="row">行</param>
void Treasure::Putting(int column, int row)
{
	if (isPut_ == false)
	{
		// ID
		id_ = ++countID;

		// 配置する（配置フラグをtrueにする）
		isPut_ = true;

		// 体力
		hp_ = 5;

		// 位置
		shape_.translate = { static_cast<float>(column * kTileSize) + shape_.scale.x ,
			static_cast<float>((kMapRow - 1 - row) * kTileSize) + shape_.scale.y };
		LocalToScreen();

		// ジャンプ
		jump_.isJump = false;
		jump_.fallingVel = 0.0f;

		// 移動速度
		vel_ = { 0.0f , 0.0f };
	}
}

/// <summary>
/// 動かす
/// </summary>
void Treasure::Move()
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

					// 速度に合わせてダメージが入る
					hp_ -= static_cast<int>(-vel_.y / 4);

					jump_.isJump = false;
					jump_.fallingVel = 0.0f;

					vel_.y = 0.0f;
				}
			}
		}

		// 動かす
		shape_.translate.y += vel_.y;
		LocalToScreen();


		// 体力がなくなったら壊れる（配置フラグがfalseになる）
		if (hp_ <= 0)
		{
			isPut_ = false;

			// IDを消す
			id_ = 0;
			--countID;
		}
	}
}

/// <summary>
/// ブロックの上に着地する
/// </summary>
/// <param name="block"ブロック></param>
void Treasure::BlockLanding(CarryBlock* block)
{
	// null を探す
	if (block == nullptr)
	{
		return;
	}

	// ブロックが配置されている（配置フラグがtrueであるとき）
	if (isPut_ && block->isPut_)
	{
		// ジャンプしている（ジャンプフラグがtrueである）ときに、ブロックの上に着地できる
		if (jump_.isJump)
		{
			if (vel_.y > 0.0f)
			{
				// 先に進んでいる場合
				if (shape_.translate.y + vel_.y + shape_.scale.y > block->shape_.translate.y - block->shape_.scale.y &&
					shape_.translate.y + vel_.y + shape_.scale.y < block->shape_.translate.y + block->shape_.scale.y)
				{
					if (shape_.translate.x + shape_.scale.x > block->shape_.translate.x - block->shape_.scale.x &&
						shape_.translate.x - shape_.scale.x < block->shape_.translate.x + block->shape_.scale.x)
					{
						// 進む直前
						if (shape_.translate.y - 1.0f + shape_.scale.y > block->shape_.translate.y - block->shape_.scale.y ||
							shape_.translate.y - 1.0f + shape_.scale.y < block->shape_.translate.y + block->shape_.scale.y)
						{
							// 押し込み処理
							shape_.translate.y = block->shape_.translate.y - block->shape_.scale.y - shape_.scale.y;
							LocalToScreen();

							vel_.y = 0.0f;
						}
					}
				}
			} 
			else if (vel_.y < 0.0f)
			{
				// 先に進んでいる場合
				if (shape_.translate.y + vel_.y - shape_.scale.y < block->shape_.translate.y + block->shape_.scale.y &&
					shape_.translate.y + vel_.y - shape_.scale.y > block->shape_.translate.y - block->shape_.scale.y)
				{
					if (shape_.translate.x + shape_.scale.x > block->shape_.translate.x - block->shape_.scale.x &&
						shape_.translate.x - shape_.scale.x < block->shape_.translate.x + block->shape_.scale.x)
					{
						// 進む直前
						if (shape_.translate.y + 1.0f - shape_.scale.y > block->shape_.translate.y + block->shape_.scale.y ||
							shape_.translate.y + 1.0f - shape_.scale.y < block->shape_.translate.y - block->shape_.scale.y)
						{
							// 押し込み処理
							shape_.translate.y = block->shape_.translate.y + block->shape_.scale.y + shape_.scale.y;
							LocalToScreen();

							// 速度に合わせてダメージが入る
							hp_ -= static_cast<int>(-vel_.y / 4);

							jump_.isJump = false;
							jump_.fallingVel = 0.0f;

							vel_.y = 0.0f;
						}
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
void Treasure::Draw(int gh)
{
	if (isPut_)
	{
		Novice::DrawQuad
		(
			static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
			static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
			static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
			static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
			0, 0, 1, 1, gh, 0x333333FF
		);
	}
}