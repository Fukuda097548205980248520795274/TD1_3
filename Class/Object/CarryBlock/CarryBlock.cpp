#include "CarryBlock.h"

/// <summary>
/// 動かす
/// </summary>
void CarryBlock::Move()
{
	if (isPut_)
	{
		// ジャンプしていない（ジャンプフラグがfalseである）ときは、坂を上ることができる
		if (jump_.isJump == false)
		{
			if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM)
			{
				shape_.translate.y = (static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) + shape_.scale.y) +
					((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2);
			}

			if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
			{
				shape_.translate.y = (static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) + shape_.scale.y) +
					static_cast<float>(kTileSize / 2) +
					((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2);
			}

			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM)
			{
				shape_.translate.y = (static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) + shape_.scale.y) +
					(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2);
			}

			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP)
			{
				shape_.translate.y = (static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) + shape_.scale.y) +
					static_cast<float>(kTileSize / 2) +
					(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2);
			}

			LocalToScreen();
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
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
			{
				// 坂にいる場合
				if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
					Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP ||
					Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
					Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
				{
					// 右上がり上で落下する（ジャンプフラグがtrueになる）
					if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM)
					{
						if (shape_.translate.y + vel_.y - shape_.scale.y >
							static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) +
							((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2))
						{
							jump_.isJump = true;
							jump_.fallingVel = -0.5f;

							// 速度に変化なし
							vel_.y = 0.0f;
						}
					}
					else if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
					{
						// 右上がり上で落下（ジャンプフラグがtrueになる）

						if (shape_.translate.y + vel_.y - shape_.scale.y >
							static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) +
							static_cast<float>(kTileSize / 2) +
							((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2))
						{
							jump_.isJump = true;
							jump_.fallingVel = -0.5f;

							// 速度に変化なし
							vel_.y = 0.0f;
						}
					} 
					else if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM)
					{
						// 左上がり下で落下する（ジャンプフラグがtrueになる）

						if (shape_.translate.y + vel_.y - shape_.scale.y >
							static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) +
							(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2))
						{
							jump_.isJump = true;
							jump_.fallingVel = -0.5f;

							// 速度に変化なし
							vel_.y = 0.0f;
						}
					}
					else if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP)
					{
						// 左上がり上で落下する（ジャンプフラグがtrueになる）

						if (shape_.translate.y + vel_.y - shape_.scale.y >
							static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) +
							static_cast<float>(kTileSize / 2) +
							(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2))
						{
							jump_.isJump = true;
							jump_.fallingVel = -0.5f;

							// 速度に変化なし
							vel_.y = 0.0f;
						}
					}
				} 
				else
				{
					jump_.isJump = true;
					jump_.fallingVel = -0.5f;

					// 速度に変化なし
					vel_.y = 0.0f;
				}
			}

			// マップを戻す
			MapUpdate(0.0f, 0.0f);
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
					Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_SLOPE_LEFT_BOTTOM ||
					Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_SLOPE_LEFT_BOTTOM ||
					Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_SLOPE_LEFT_TOP ||
					Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_SLOPE_LEFT_TOP ||
					Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_SLOPE_RIGHT_BOTTOM ||
					Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_SLOPE_RIGHT_BOTTOM ||
					Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_SLOPE_RIGHT_TOP ||
					Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_SLOPE_RIGHT_TOP ||
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


				/*   坂に着地する   */

				if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM)
				{
					if (shape_.translate.y + vel_.y - shape_.scale.y <
						static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) +
						((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2))
					{
						// 押し込み処理
						shape_.translate.y =
							(static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) + shape_.scale.y) +
							((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2);
						LocalToScreen();

						jump_.isJump = false;
						jump_.fallingVel = 0.0f;

						vel_.y = 0.0f;
					}
				}

				if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
				{
					if (shape_.translate.y + vel_.y - shape_.scale.y <
						static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) +
						static_cast<float>(kTileSize / 2) +
						((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2))
					{
						// 押し込み処理
						shape_.translate.y =
							(static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) + shape_.scale.y) +
							static_cast<float>(kTileSize / 2) +
							((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2);
						LocalToScreen();

						jump_.isJump = false;
						jump_.fallingVel = 0.0f;

						vel_.y = 0.0f;
					}
				}

				if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM)
				{
					if (shape_.translate.y + vel_.y - shape_.scale.y <
						static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) +
						(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2))
					{
						// 押し込み処理
						shape_.translate.y =
							(static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) + shape_.scale.y) +
							(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2);
						LocalToScreen();

						jump_.isJump = false;
						jump_.fallingVel = 0.0f;

						vel_.y = 0.0f;
					}
				}

				if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP)
				{
					if (shape_.translate.y + vel_.y - shape_.scale.y <
						static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) +
						static_cast<float>(kTileSize / 2) +
						(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2))
					{
						// 押し込み処理
						shape_.translate.y =
							(static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) + shape_.scale.y) +
							static_cast<float>(kTileSize / 2) +
							(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2);
						LocalToScreen();

						jump_.isJump = false;
						jump_.fallingVel = 0.0f;

						vel_.y = 0.0f;
					}
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

	// ブロックが配置されている（配置フラグがtrueであるとき）
	if (isPut_ && block->isPut_)
	{
		/*   着地   */

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