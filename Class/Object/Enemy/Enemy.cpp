#include "Enemy.h"

/// <summary>
/// 動かす
/// </summary>
void Enemy::Move()
{
	if (isArrival_)
	{
		// 右に進むとき
		if (vel_.x > 0.0f)
		{
			// 一時的にマップを更新する
			MapUpdate(vel_.x, 0.0f);

			if (Map::map_[map_.rightTop.row][map_.rightTop.column] != TILE_NOTHING ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] != TILE_NOTHING)
			{
				// 移動方向を反転する
				vel_.x *= -1.0f;

				// 押し込み処理
				shape_.translate.x = static_cast<float>(map_.rightTop.column * kTileSize) - shape_.scale.x;
				LocalToScreen();
			}
		} 
		else if (vel_.x < 0.0f)
		{
			// 左に進むとき

			// 一時的にマップを更新する
			MapUpdate(vel_.x, 0.0f);

			if (Map::map_[map_.leftTop.row][map_.leftBottom.column] != TILE_NOTHING ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] != TILE_NOTHING)
			{
				// 移動方向を反転する
				vel_.x *= -1.0f;

				// 押し込み処理
				shape_.translate.x = static_cast<float>(map_.leftTop.column * kTileSize + kTileSize) + shape_.scale.x;
				LocalToScreen();
			}
		}

		// 動かす
		shape_.translate.x += vel_.x;
		LocalToScreen();


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


		/*   腐らせ   */

		// 一時的にマップを更新する
		MapUpdate(shape_.scale.x, -1.0f);

		// 下にブロックがあたら、腐り始める
		if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_BLOCK)
		{
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] = -600;
		}
		else if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_ROTTED)
		{
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] = -60;
		}

		// マップを戻す
		MapUpdate(0.0f, 0.0f);
	}
}

/// <summary>
/// ブロックに着地する
/// </summary>
/// <param name="block">ブロック</param>
void Enemy::BlockLanding(CarryBlock* block)
{
	// null を探す
	if (block == nullptr)
	{
		return;
	}


	if (isArrival_)
	{
		/*   着地   */

		// ジャンプしている（ジャンプフラグがtrueである）ときに、ブロックの上に着地できる
		if (jump_.isJump)
		{
			if (block->isPut_)
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

								jump_.isJump = false;
								jump_.fallingVel = 0.0f;

								vel_.y = 0.0f;
							}
						}
					}
				}
			}
		}


		/*   当たり判定   */

		if (block->isPut_)
		{
			// 右に進むとき
			if (vel_.x > 0.0f)
			{
				if (shape_.translate.x + vel_.x + shape_.scale.x > block->shape_.translate.x - block->shape_.scale.x &&
					shape_.translate.x + vel_.x + shape_.scale.x < block->shape_.translate.x + block->shape_.scale.x)
				{
					if (shape_.translate.y + shape_.scale.y > block->shape_.translate.y - block->shape_.scale.y &&
						shape_.translate.y - shape_.scale.y < block->shape_.translate.y + block->shape_.scale.y)
					{
						if (shape_.translate.x - 1.0f + shape_.scale.x < block->shape_.translate.x - block->shape_.scale.x ||
							shape_.translate.x - 1.0f + shape_.scale.x > block->shape_.translate.x + block->shape_.scale.x)
						{
							// 移動方向を反転する
							vel_.x *= -1.0f;

							// 押し込み処理
							shape_.translate.x = block->shape_.translate.x - block->shape_.scale.x - shape_.scale.x;
							LocalToScreen();
						}
					}
				}
			} 
			else if (vel_.x < 0.0f)
			{
				// 左に進むとき

				if (shape_.translate.x + vel_.x - shape_.scale.x < block->shape_.translate.x + block->shape_.scale.x &&
					shape_.translate.x + vel_.x - shape_.scale.x > block->shape_.translate.x - block->shape_.scale.x)
				{
					if (shape_.translate.y + shape_.scale.y > block->shape_.translate.y - block->shape_.scale.y &&
						shape_.translate.y - shape_.scale.y < block->shape_.translate.y + block->shape_.scale.y)
					{
						if (shape_.translate.x + 1.0f - shape_.scale.x > block->shape_.translate.x + block->shape_.scale.x ||
							shape_.translate.x + 1.0f - shape_.scale.x < block->shape_.translate.x - block->shape_.scale.x)
						{
							// 移動方向を反転する
							vel_.x *= -1.0f;

							// 押し込み処理
							shape_.translate.x = block->shape_.translate.x + block->shape_.scale.x + shape_.scale.x;
							LocalToScreen();
						}
					}
				}
			}
		}
	}
}