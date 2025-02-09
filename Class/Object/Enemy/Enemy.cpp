#include "Enemy.h"

int Enemy::countID;

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

	// 火の粉のクールタイム
	hinokoCoolTime_ = 0;


	// パーティクルを消す

	for (int i = 0; i < kParticleHinoko; i++)
	{
		hinoko_[i]->isEmission_ = false;
	}

	for (int i = 0; i < kParticleVapor; i++)
	{
		vapor[i]->isEmission_ = false;
	}
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


		// パーティクルを消す

		for (int i = 0; i < kParticleHinoko; i++)
		{
			hinoko_[i]->isEmission_ = false;
		}

		for (int i = 0; i < kParticleVapor; i++)
		{
			vapor[i]->isEmission_ = false;
		}
	}
}

/// <summary>
/// 動かす
/// </summary>
void Enemy::Move()
{
	// 出現していない（出現フラグがfalseである）敵は、動かさない
	if (isArrival_ == false)
	{
		return;
	}


	/*   移動   */


	if (vel_.x > 0.0f)
	{
		// 右に進むとき
		float slopeVelY = 0.0f;

		if (directionNo == DIRECTION_RIGHT)
		{
			directionNo = DIRECTION_LEFT;
		}

		// ジャンプしていない（ジャンプフラグがfalseである）
		if (jump_.isJump == false)
		{
			// ブロックの上にいない（乗っかりフラグがfalseである）
			if (isRide_ == false)
			{
				if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
					Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP)
				{
					// 左上がりの坂を下りるベクトル
					struct Vec2 slopeVec = { static_cast<float>(kTileSize * 2) , static_cast<float>(-kTileSize) };
					slopeVec = Normalize(slopeVec);

					vel_.x = 2.0f * slopeVec.x;
					slopeVelY = 2.0f * slopeVec.y;
				}
				else if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
					Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
				{
					// 右上がりの坂を上がるベクトル
					struct Vec2 slopeVec = { static_cast<float>(kTileSize * 2) , static_cast<float>(kTileSize) };
					slopeVec = Normalize(slopeVec);

					vel_.x = 2.0f * slopeVec.x;
					slopeVelY = 2.0f * slopeVec.y;
				}
				else
				{
					vel_.x = 2.0f;
				}
			}
			else
			{
				vel_.x = 2.0f;
			}
		}
		else
		{
			// ジャンプ中（ジャンプフラグがtrueであるとき）は、通常移動
			vel_.x = 2.0f;
		}


		/// 天井の当たり判定

		// 一時的にマップを更新する
		MapUpdate(vel_.x, slopeVelY);

		// 壁にぶつかると、歩かない（歩きフラグがfalseになる）
		if (Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_GROUND ||
			Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_BLOCK ||
			Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_ROTTED ||
			Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_WATER ||
			Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_CONCRETE ||
			Map::map_[map_.rightTop.row][map_.rightTop.column] < 0)
		{
			// 水に触れたら、消滅する（出現フラグがfalseになる）
			if (Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_WATER)
			{
				isArrival_ = false;
			}

			// 移動方向を反転する
			vel_.x *= -1.0f;

			// 動かす
			shape_.translate.x += vel_.x;
			LocalToScreen();
		}
		else
		{
			/// 壁の当たり判定

			// 一時的にマップを更新する
			MapUpdate(vel_.x, 0.0f);

			if (Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_GROUND ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GROUND ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_BLOCK ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_BLOCK ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_ROTTED ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_ROTTED ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_WATER ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_CONCRETE ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_CONCRETE ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] < 0 || Map::map_[map_.rightBottom.row][map_.rightBottom.column] < 0)
			{
				// 水に触れたら、消滅する（出現フラグがfalseになる）
				if (Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_WATER ||
					Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER)
				{
					isArrival_ = false;
				}

				// 移動方向を反転する
				vel_.x *= -1.0f;

				// 押し込み処理
				shape_.translate.x = static_cast<float>(map_.rightTop.column * kTileSize) - shape_.scale.x;
				LocalToScreen();
			}
		}


		// 画面端で、反転する
		if (shape_.translate.x + shape_.scale.x > static_cast<float>(kScreenWidth))
		{
			// 移動方向を反転する
			vel_.x *= -1.0f;

		}
	}
	else if (vel_.x < 0.0f)
	{
		// 左に進むとき
		float slopeVelY = 0.0f;

		if (directionNo == DIRECTION_LEFT)
		{
			directionNo = DIRECTION_RIGHT;
		}

		// ジャンプしていない（ジャンプフラグがfalseである）
		if (jump_.isJump == false)
		{
			// ブロックの上にいない（乗っかりフラグがfalseである）
			if (isRide_ == false)
			{
				if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
					Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_TOP)
				{
					// 右上がりの坂を下りるベクトル
					struct Vec2 slopeVec = { static_cast<float>(-kTileSize * 2) , static_cast<float>(-kTileSize) };
					slopeVec = Normalize(slopeVec);

					vel_.x = 2.0f * slopeVec.x;
					slopeVelY = 2.0f * slopeVec.y;
				}
				else if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
					Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM)
				{
					// 左上がりの坂を下りるベクトル
					struct Vec2 slopeVec = { static_cast<float>(-kTileSize * 2) , static_cast<float>(kTileSize) };
					slopeVec = Normalize(slopeVec);

					vel_.x = 2.0f * slopeVec.x;
					slopeVelY = 2.0f * slopeVec.y;
				}
				else
				{
					vel_.x = -2.0f;
				}
			}
			else
			{
				vel_.x = -2.0f;
			}
		}
		else
		{
			// ジャンプ中（ジャンプフラグがtrueであるとき）は、通常移動
			vel_.x = -2.0f;
		}


		/// 天井の当たり判定

		// 一時的にマップを更新する
		MapUpdate(vel_.x, slopeVelY);

		// 壁にぶつかると、歩かない（歩きフラグがfalseになる）
		if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_GROUND ||
			Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_BLOCK ||
			Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_ROTTED ||
			Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_WATER ||
			Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_CONCRETE ||
			Map::map_[map_.leftTop.row][map_.leftTop.column] < 0)
		{
			// 水に触れたら、消滅する（出現フラグがfalseになる）
			if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_WATER)
			{
				isArrival_ = false;
			}

			// 移動方向を反転する
			vel_.x *= -1.0f;

			// 動かす
			shape_.translate.x += vel_.x;
			LocalToScreen();
		}
		else
		{
			/// 壁の当たり判定

			// 一時的にマップを更新する
			MapUpdate(vel_.x, 0.0f);

			if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_GROUND ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GROUND ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_BLOCK ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_BLOCK ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_ROTTED ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_ROTTED ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_WATER ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_CONCRETE ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_CONCRETE ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] < 0 || Map::map_[map_.leftBottom.row][map_.leftBottom.column] < 0)
			{
				// 水に触れたら、消滅する（出現フラグがfalseになる）
				if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_WATER ||
					Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER)
				{
					isArrival_ = false;
				}


				// 移動方向を反転する
				vel_.x *= -1.0f;

				if (directionNo == DIRECTION_LEFT)
				{
					directionNo = DIRECTION_RIGHT;
				}

				// 押し込み処理
				shape_.translate.x = static_cast<float>(map_.leftTop.column * kTileSize + kTileSize) + shape_.scale.x;
				LocalToScreen();
			}
		}


		// 画面端で反転する
		if (shape_.translate.x - shape_.scale.x < 0.0f)
		{
			// 移動方向を反転する
			vel_.x *= -1.0f;
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
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_NOTHING &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_NOTHING &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GOAL ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_NOTHING &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_NOTHING &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_TOP ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_NOTHING &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_NOTHING &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP ||

			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_LADDER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_LADDER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_NOTHING ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_LADDER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GOAL ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_LADDER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_LADDER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_TOP ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_LADDER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_LADDER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP ||

			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GOAL &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GOAL ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GOAL &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_NOTHING ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GOAL &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GOAL &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GOAL &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_TOP ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GOAL &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GOAL &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP ||

			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_NOTHING ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GOAL ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_TOP ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP ||

			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_TOP ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_NOTHING ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GOAL ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP ||

			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_BOTTOM &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_BOTTOM &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_NOTHING ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_BOTTOM &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_BOTTOM &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GOAL ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_BOTTOM &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_BOTTOM &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_TOP ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_BOTTOM &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP ||

			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_TOP &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_TOP &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_NOTHING ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_TOP &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_TOP &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GOAL ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_TOP &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_TOP &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_TOP ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_TOP &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM)
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
				} else if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
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
				} else if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM)
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
				} else if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP)
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
			} else
			{
				jump_.isJump = true;
				jump_.fallingVel = -0.5f;

				// 速度に変化なし
				vel_.y = 0.0f;
			}
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
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_WATER ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_WATER ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_CONCRETE ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_CONCRETE ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] < 0 || Map::map_[map_.rightTop.row][map_.rightTop.column] < 0)
			{
				// 水に触れると、消火される（出現フラグがfalseになる）
				if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_WATER ||
					Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_WATER)
				{
					isArrival_ = false;

					// IDを消す
					id_ = 0;
					--countID;

					// 消火した音
					int shFireExtinguishing = Novice::LoadAudio("./Resources/Sounds/Se/fireExtinguishing.mp3");
					Novice::PlayAudio(shFireExtinguishing, 0, 0.3f);
				}

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
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_CONCRETE ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_CONCRETE ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] < 0 || Map::map_[map_.rightBottom.row][map_.rightBottom.column] < 0)
			{
				// 水に触れたら、消火される（出現フラグがfalseになる）
				if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER ||
					Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER)
				{
					isArrival_ = false;

					// IDを消す
					id_ = 0;
					--countID;

					// 消火した音
					int shFireExtinguishing = Novice::LoadAudio("./Resources/Sounds/Se/fireExtinguishing.mp3");
					Novice::PlayAudio(shFireExtinguishing, 0, 0.3f);
				}

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
				if (shape_.translate.y + vel_.y - shape_.scale.y - 1.0f <
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
				if (shape_.translate.y + vel_.y - shape_.scale.y - 1.0f <
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
				if (shape_.translate.y + vel_.y - shape_.scale.y - 1.0f <
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
				if (shape_.translate.y + vel_.y - shape_.scale.y - 1.0f <
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
		Map::map_[map_.leftBottom.row][map_.leftBottom.column] = kStartRotten;

		// 消火した音
		int shFireExtinguishing = Novice::LoadAudio("./Resources/Sounds/Se/fireExtinguishing.mp3");
		Novice::PlayAudio(shFireExtinguishing, 0, 0.3f);

		for (int i = 0; i < 24; i++)
		{
			for (int j = 0; j < kParticleVapor; j++)
			{
				if (vapor[j]->isEmission_ == false)
				{
					vapor[j]->Emission({ static_cast<float>(map_.leftBottom.column * kTileSize + rand() % kTileSize) ,
						static_cast<float>(kScreenHeight - (map_.leftBottom.row * kTileSize + rand() % kTileSize)) });

					break;
				}
			}
		}
	}
	else if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_ROTTED)
	{
		Map::map_[map_.leftBottom.row][map_.leftBottom.column] = kStartRotten + 300;

		// 消火した音
		int shFireExtinguishing = Novice::LoadAudio("./Resources/Sounds/Se/fireExtinguishing.mp3");
		Novice::PlayAudio(shFireExtinguishing, 0, 0.3f);

		for (int i = 0; i < 24; i++)
		{
			for (int j = 0; j < kParticleVapor; j++)
			{
				if (vapor[j]->isEmission_ == false)
				{
					vapor[j]->Emission({ static_cast<float>(map_.leftBottom.column * kTileSize + rand() % kTileSize) ,
						static_cast<float>(kScreenHeight - (map_.leftBottom.row * kTileSize + rand() % kTileSize)) });

					break;
				}
			}
		}
	}


	/*   弱点の水   */

	// 地面が水だったら、蒸発する（出現フラグがfalseになる）
	if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER)
	{
		// 敵も消滅する（出現フラグがfalseになる）
		isArrival_ = false;

		// IDを消す
		id_ = 0;
		--countID;
	}



	/*   パーティクル   */

	// クールタイムを進める
	if (hinokoCoolTime_ > 0)
	{
		hinokoCoolTime_--;
	}

	// 放出する
	if (hinokoCoolTime_ <= 0)
	{
		for (int i = 0; i < kParticleHinoko; i++)
		{
			if (hinoko_[i]->isEmission_ == false)
			{
				// クールタイムを開始する
				hinokoCoolTime_ = 5;

				hinoko_[i]->Emission({ shape_.translate.x - shape_.scale.x + static_cast<float>(rand() % static_cast<int>(shape_.scale.x * 2)) ,
					shape_.translate.y - shape_.scale.y + static_cast<float>(rand() % static_cast<int>(shape_.scale.y * 2)) });

				break;
			}
		}
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


	/*   つぶす   */

	// 敵の上に、ブロックが落ちてきたら、敵がやられる
	if (block->jump_.isJump)
	{
		if (block->vel_.y < 0.0f)
		{
			if (block->shape_.translate.y + block->vel_.y - block->shape_.scale.y < shape_.translate.y + shape_.scale.y &&
				block->shape_.translate.y + block->vel_.y - block->shape_.scale.y > shape_.translate.y - shape_.scale.y)
			{
				if (block->shape_.translate.x + block->shape_.scale.x > shape_.translate.x - shape_.scale.x &&
					block->shape_.translate.x - block->shape_.scale.x < shape_.translate.x + shape_.scale.x)
				{
					if (block->shape_.translate.y - block->shape_.scale.y > shape_.translate.y + shape_.scale.y ||
						block->shape_.translate.y - block->shape_.scale.y < shape_.translate.y - shape_.scale.y)
					{
						// 敵も消滅する（出現フラグがfalseになる）
						isArrival_ = false;

						// IDを消す
						id_ = 0;
						--countID;
					}
				}
			}
		}
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

						// 触れたブロックが爆弾だったら、時間が起動する（爆発フラグがtrueになる）
						if (block->type_ == BLOCK_BOMB)
						{
							block->isExplosion_ = true;

							// 敵も消滅する（出現フラグがfalseになる）
							isArrival_ = false;

							// IDを消す
							id_ = 0;
							--countID;

							// 着火した音
							int shIgnition = Novice::LoadAudio("./Resources/Sounds/Se/ignition.mp3");
							Novice::PlayAudio(shIgnition, 0, 0.3f);
						}
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

						// 触れたブロックが爆弾だったら、時間が起動する（爆発フラグがtrueになる）
						if (block->type_ == BLOCK_BOMB)
						{
							block->isExplosion_ = true;

							// 敵も消滅する（出現フラグがfalseになる）
							isArrival_ = false;

							// IDを消す
							id_ = 0;
							--countID;

							// 着火した音
							int shIgnition = Novice::LoadAudio("./Resources/Sounds/Se/ignition.mp3");
							Novice::PlayAudio(shIgnition, 0, 0.3f);
						}
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
				if (shape_.translate.y + shape_.scale.y - 1.0f > block->shape_.translate.y - block->shape_.scale.y &&
					shape_.translate.y - shape_.scale.y + 1.0f < block->shape_.translate.y + block->shape_.scale.y)
				{
					// 移動方向を反転する
					vel_.x *= -1.0f;

					// 押し込み処理
					shape_.translate.x = block->shape_.translate.x - block->shape_.scale.x - shape_.scale.x;
					LocalToScreen();

					// 触れたブロックが爆弾だったら、時間が起動する（爆発フラグがtrueになる）
					if (block->type_ == BLOCK_BOMB)
					{
						block->isExplosion_ = true;

						// 敵も消滅する（出現フラグがfalseになる）
						isArrival_ = false;

						// IDを消す
						id_ = 0;
						--countID;

						// 着火した音
						int shIgnition = Novice::LoadAudio("./Resources/Sounds/Se/ignition.mp3");
						Novice::PlayAudio(shIgnition, 0, 0.3f);
					}
				}
			}
		}
		else if (vel_.x < 0.0f)
		{
			// 左に進むとき

			if (shape_.translate.x - shape_.scale.x < block->shape_.translate.x + block->shape_.scale.x &&
				shape_.translate.x - shape_.scale.x > block->shape_.translate.x - block->shape_.scale.x)
			{
				if (shape_.translate.y + shape_.scale.y - 1.0f > block->shape_.translate.y - block->shape_.scale.y &&
					shape_.translate.y - shape_.scale.y + 1.0f < block->shape_.translate.y + block->shape_.scale.y)
				{
					// 移動方向を反転する
					vel_.x *= -1.0f;

					// 押し込み処理
					shape_.translate.x = block->shape_.translate.x + block->shape_.scale.x + shape_.scale.x;
					LocalToScreen();

					// 触れたブロックが爆弾だったら、時間が起動する（爆発フラグがtrueになる）
					if (block->type_ == BLOCK_BOMB)
					{
						block->isExplosion_ = true;

						// 敵も消滅する（出現フラグがfalseになる）
						isArrival_ = false;

						// IDを消す
						id_ = 0;
						--countID;

						// 着火した音
						int shIgnition = Novice::LoadAudio("./Resources/Sounds/Se/ignition.mp3");
						Novice::PlayAudio(shIgnition, 0, 0.3f);
					}
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