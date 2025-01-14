#include "Enemy.h"

/// <summary>
/// 初期化する
/// </summary>
void Enemy::InitialValue()
{
	// 出現していない（出現フラグがfalseである）敵は、初期化しない
	if (isArrival_ == false)
	{
		return;
	}


	// ID
	id_ = 0;
	--countID;

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
}


/// <summary>
/// 出現させる
/// </summary>
/// <param name="column">列</param>
/// <param name="row">行</param>
void Enemy::Arrival(int column, int row)
{
	if (isArrival_ == false)
	{
		// ID
		id_ = ++countID;

		// 出現させる（出現フラグをtrueにする）
		isArrival_ = true;

		// 位置
		shape_.translate = { static_cast<float>(column * kTileSize) + shape_.scale.x ,
			static_cast<float>((kMapRow - 1 - row) * kTileSize) + shape_.scale.y };
		LocalToScreen();

		// ジャンプ
		jump_.isJump = false;
		jump_.fallingVel = 0.0f;

		// 移動速度
		vel_ = { -2.0f , 0.0f };
	}
}

/// <summary>
/// 動かす
/// </summary>
void Enemy::Move()
{
	if (isArrival_)
	{
		/*   移動   */

		// 右に進むとき
		if (vel_.x > 0.0f)
		{
			// 一時的にマップを更新する
			MapUpdate(vel_.x, 0.0f);

			if (Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_GROUND ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GROUND ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_BLOCK ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_BLOCK ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_ROTTED ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_ROTTED ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] < 0 || Map::map_[map_.rightBottom.row][map_.rightBottom.column] < 0)
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

			if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_GROUND ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GROUND ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_BLOCK ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_BLOCK ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_ROTTED ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_ROTTED ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] < 0 || Map::map_[map_.leftBottom.row][map_.leftBottom.column] < 0)
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

		/*   坂   */

		// ジャンプしていない（ジャンプフラグがfalseである）ときは、坂を上ることができる
		if (jump_.isJump == false)
		{
			// 右上がり下
			if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM)
			{
				shape_.translate.y = 1.0f + (static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) + shape_.scale.y) +
					((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2);
			}

			// 右上がり上
			if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
			{
				shape_.translate.y = 1.0f + (static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) + shape_.scale.y) +
					static_cast<float>(kTileSize / 2) +
					((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2);
			}

			// 左上がり下
			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM)
			{
				shape_.translate.y = 1.0f + (static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) + shape_.scale.y) +
					(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2);
			}

			// 左下がり上
			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP)
			{
				shape_.translate.y = 1.0f + (static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) + shape_.scale.y) +
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

				// 右上がり下で着地する（ジャンプフラグがfalseになる）
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

				// 右上がり上で着地する（ジャンプフラグがfalseになる）
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

				// 左上がり下で着地する（ジャンプフラグがfalseになる）
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

				// 左上がり上で着地する（ジャンプフラグがfalseになる）
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

	// 敵とブロックが出現していなかったら（出現、配置フラグがfalseだったら）、処理を修了する
	if (isArrival_ == false || block->isPut_ == false)
	{
		return;
	}



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
	else
	{
		/*   当たり判定   */

		// 右に進むとき
		if (vel_.x > 0.0f)
		{
			if (shape_.translate.x + shape_.scale.x > block->shape_.translate.x - block->shape_.scale.x &&
				shape_.translate.x + shape_.scale.x < block->shape_.translate.x + block->shape_.scale.x)
			{
				if (shape_.translate.y + shape_.scale.y > block->shape_.translate.y - block->shape_.scale.y &&
					shape_.translate.y - shape_.scale.y < block->shape_.translate.y + block->shape_.scale.y)
				{
					// 移動方向を反転する
					vel_.x *= -1.0f;

					// 押し込み処理
					shape_.translate.x = block->shape_.translate.x - block->shape_.scale.x - shape_.scale.x;
					LocalToScreen();
				}
			}
		}
		else if (vel_.x < 0.0f)
		{
			// 左に進むとき

			if (shape_.translate.x - shape_.scale.x < block->shape_.translate.x + block->shape_.scale.x &&
				shape_.translate.x - shape_.scale.x > block->shape_.translate.x - block->shape_.scale.x)
			{
				if (shape_.translate.y + shape_.scale.y > block->shape_.translate.y - block->shape_.scale.y &&
					shape_.translate.y - shape_.scale.y < block->shape_.translate.y + block->shape_.scale.y)
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

/// <summary>
/// 敵を挟みつぶす
/// </summary>
/// <param name="block1">ブロック1</param>
/// <param name="block2">ブロック2</param>
void Enemy::Death(CarryBlock* block1, CarryBlock* block2)
{
	// null を探す
	if (block1 == nullptr || block2 == nullptr)
	{
		return;
	}

	// 一つでも配置、出現していなかった処理を修了する
	if (isArrival_ == false || block1->isPut_ == false || block2->isPut_ == false)
	{
		return;
	}
}

/// <summary>
/// 描画する
/// </summary>
void Enemy::Draw()
{

}