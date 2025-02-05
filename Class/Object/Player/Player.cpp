#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
{
	// 図形
	shape_.scale = { 20.0f , 20.0f };
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

	// 乗っかっているかどうか（乗っかりフラグ）
	isRide_ = false;

	// 坂フラグ
	isLeftSlopeRide_ = false;
	isRightSlopeRide_ = false;


	// 向き
	directionNo = DIRECTION_RIGHT;

	// 復活
	respawn_.isRespawn = false;
	respawn_.frame = 120;

	// 歩き
	walk_.isWalk = false;
	walk_.frame = 0;

	// 運び
	carry_.isCarry = false;
	carry_.frame = 0;

	// 梯子を使っているかどうか（梯子フラグ）
	isLadderClimd_ = false;

	// 梯子を上る速度
	ladderVel_ = 0.0f;

	// 運ぶときの雪
	for (int i = 0; i < kParticleSnowCarry; i++)
	{
		snowCarry[i] = new SnowCarry();
	}

	// 着地したときの粒子
	for (int i = 0; i < kParticleLanding; i++)
	{
		landing[i] = new Landing();
	}

	// 輝く雪
	for (int i = 0; i < kParticleSnowShining; i++)
	{
		snowShining[i] = new SnowShining();
	}
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	// 運ぶときの雪
	for (int i = 0; i < kParticleSnowCarry; i++)
	{
		delete snowCarry[i];
	}

	// 着地したときの粒子
	for (int i = 0; i < kParticleLanding; i++)
	{
		delete landing[i];
	}

	// 輝く雪
	for (int i = 0; i < kParticleSnowShining; i++)
	{
		delete snowShining[i];
	}
}

/// <summary>
/// 配置する
/// </summary>
/// <param name="column">列</param>
/// <param name="row">行</param>
void Player::Puttting(int column, int row)
{
	// 復活
	respawn_.isRespawn = true;
	respawn_.frame = 60;

	// 位置
	shape_.translate = { static_cast<float>(column * kTileSize) + shape_.scale.x ,
		static_cast<float>((kMapRow - 1 - row) * kTileSize) + shape_.scale.y };
	LocalToScreen();

	// ジャンプ
	jump_.isJump = false;
	jump_.fallingVel = 0.0f;

	// 移動速度
	vel_ = { 0.0f , 0.0f };

	// 向き
	directionNo = DIRECTION_RIGHT;

	// 歩き
	walk_.isWalk = false;
	walk_.frame = 0;

	// 運び
	carry_.isCarry = false;
	carry_.frame = 0;


	// パーティクルを消す

	for (int i = 0; i < kParticleSnowCarry; i++)
	{
		snowCarry[i]->isEmission_ = false;
	}

	for (int i = 0; i < kParticleLanding; i++)
	{
		landing[i]->isEmission_ = false;
	}

	for (int i = 0; i < kParticleSnowShining; i++)
	{
		snowShining[i]->isEmission_ = false;
	}
}

/// <summary>
/// 操作する
/// </summary>
/// <param name="keys">キー</param>
/// <param name="preKeys">前のキー</param>
void Player::Operation(const char* keys, const char* preKeys , int stickX , int stickY)
{
	// null を探す
	if (keys == nullptr || preKeys == nullptr)
	{
		return;
	}

	// やられている（復活フラグがfalseである）ときは、操作できない
	if (respawn_.isRespawn == false)
	{
		return;
	}


	/*   横移動   */

	// 横移動の速度を初期化する
	vel_.x = 0.0f;

	// Aキーで、左に歩く（歩きフラグがtrueになる）
	if (keys[DIK_A] || stickX < -16000)
	{
		if (shape_.translate.x - shape_.scale.x > 0.0f)
		{
			walk_.isWalk = true;
			directionNo = DIRECTION_LEFT;

			float slopeVelY = 0.0f;

			// ジャンプしていない（ジャンプフラグがfalseである）
			if (jump_.isJump == false)
			{
				// ブロックの上にいない（乗っかりフラグがfalseである）
				if (isRide_ == false)
				{
					if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
						Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_TOP)
					{
						// 左上がりの坂を下りるベクトル
						struct Vec2 slopeVec = { static_cast<float>(-kTileSize * 2) , static_cast<float>(-kTileSize) };
						slopeVec = Normalize(slopeVec);

						vel_.x = 4.0f * slopeVec.x;
						slopeVelY = 4.0f * slopeVec.y;
					}
					else if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
						Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM)
					{
						// 左上がりの坂を下りるベクトル
						struct Vec2 slopeVec = { static_cast<float>(-kTileSize * 2) , static_cast<float>(kTileSize) };
						slopeVec = Normalize(slopeVec);

						vel_.x = 4.0f * slopeVec.x;
						slopeVelY = 4.0f * slopeVec.y;
					}
					else
					{
						vel_.x = -4.0f;
					}
				}
				else
				{
					vel_.x = -4.0f;
				}
			}
			else
			{
				// ジャンプ中（ジャンプフラグがtrueであるとき）は、通常移動
				vel_.x = -4.0f;
			}


			// 天井の当たり判定

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
				// 水があったら凍る
				if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_WATER)
				{
					Map::map_[map_.leftTop.row][map_.leftTop.column] = TILE_ROTTED;
				}

				walk_.isWalk = false;

				vel_.x = 0.0f;
			}
			else
			{
				// 一時的にマップを更新する
				MapUpdate(vel_.x, 0.0f);

				// 壁にぶつかると、歩かない（歩きフラグがfalseになる）
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
					// 水があったら、凍る
					if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_WATER)
					{
						Map::map_[map_.leftTop.row][map_.leftTop.column] = TILE_ROTTED;
					}

					if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER)
					{
						Map::map_[map_.leftBottom.row][map_.leftBottom.column] = TILE_ROTTED;
					}

					vel_.x = 0.0f;

					// 押し込み処理
					shape_.translate.x = static_cast<float>(map_.leftTop.column * kTileSize + kTileSize) + shape_.scale.x;
					LocalToScreen();
				}
			}
		}
	}
	else if (keys[DIK_D] || stickX > 16000)
	{
		// Dキーで、右に歩く（歩きフラグがtrueになる）

		if (shape_.translate.x + shape_.scale.x < static_cast<float>(kScreenWidth))
		{
			walk_.isWalk = true;
			directionNo = DIRECTION_RIGHT;

			float slopeVelY = 0.0f;

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

						vel_.x = 4.0f * slopeVec.x;
						slopeVelY = 4.0f * slopeVec.y;
					}
					else if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
						Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
					{
						// 右上がりの坂を上がるベクトル
						struct Vec2 slopeVec = { static_cast<float>(kTileSize * 2) , static_cast<float>(kTileSize) };
						slopeVec = Normalize(slopeVec);

						vel_.x = 4.0f * slopeVec.x;
						slopeVelY = 4.0f * slopeVec.y;
					}
					else
					{
						vel_.x = 4.0f;
					}
				}
				else
				{
					vel_.x = 4.0f;
				}
			}
			else
			{
				// ジャンプ中（ジャンプフラグがtrueであるとき）は、通常移動
				vel_.x = 4.0f;
			}


			// 壁の当たり判定

			// 一時的にマップを更新する
			MapUpdate(vel_.x, 0.0f);

			// 壁にぶつかると、歩かない（歩きフラグがfalseになる）
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
				// 水があったら凍る
				if (Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_WATER)
				{
					Map::map_[map_.rightTop.row][map_.rightTop.column] = TILE_ROTTED;
				}

				if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER)
				{
					Map::map_[map_.rightBottom.row][map_.rightBottom.column] = TILE_ROTTED;
				}

				walk_.isWalk = false;

				vel_.x = 0.0f;

				// 押し込み処理
				shape_.translate.x = static_cast<float>(map_.rightTop.column * kTileSize) - shape_.scale.x;
				LocalToScreen();
			}


			// 天井の当たり判定

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
				// 水があったら凍る
				if (Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_WATER)
				{
					Map::map_[map_.rightTop.row][map_.rightTop.column] = TILE_ROTTED;
				}

				walk_.isWalk = false;

				vel_.x = 0.0f;
			}
		}
	}
	else
	{
		// キーを押していないときは、歩かない（歩きフラグがfalseになる）

		walk_.isWalk = false;
	}

	// 歩いている（歩きフラグがtrueである）ときに、輝く雪を散らす
	if (walk_.isWalk)
	{
		if (SnowShining::coolTime <= 0)
		{
			for (int i = 0; i < kParticleSnowShining; i++)
			{
				if (snowShining[i]->isEmission_ == false)
				{
					SnowShining::coolTime = 2;

					snowShining[i]->Emission({ shape_.translate.x - shape_.scale.x + static_cast<float>(rand() % (static_cast<int>(shape_.scale.x) * 2)) ,
						shape_.translate.y - shape_.scale.y + static_cast<float>(rand() % (static_cast<int>(shape_.scale.y) * 2)) });

					break;
				}
			}
		}
	}

	// 動かす
	shape_.translate.x += vel_.x;
	LocalToScreen();


	/*   坂   */

	// ジャンプしていない（ジャンプフラグがfalseである）ときは、坂を上ることができる
	if (jump_.isJump == false)
	{
		if (isRide_ == false)
		{
			// 右上がり下
			if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM)
			{
				shape_.translate.y = (static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) + shape_.scale.y) +
					((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2);

				isRightSlopeRide_ = true;
				isLeftSlopeRide_ = false;
			}

			// 右上がり上
			if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
			{
				shape_.translate.y = (static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) + shape_.scale.y) +
					static_cast<float>(kTileSize / 2) +
					((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2);

				isRightSlopeRide_ = true;
				isLeftSlopeRide_ = false;
			}

			// 左上がり下
			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM)
			{
				shape_.translate.y = (static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) + shape_.scale.y) +
					(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2);

				isRightSlopeRide_ = false;
				isLeftSlopeRide_ = true;
			}

			// 左下がり上
			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP)
			{
				shape_.translate.y = (static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) + shape_.scale.y) +
					static_cast<float>(kTileSize / 2) +
					(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2);

				isRightSlopeRide_ = false;
				isLeftSlopeRide_ = true;
			}

			LocalToScreen();
		}
		else
		{
			isRightSlopeRide_ = false;
			isLeftSlopeRide_ = false;
		}
	}
	else
	{
		isRightSlopeRide_ = false;
		isLeftSlopeRide_ = false;
	}

	// 乗っかりフラグをfalseにする
	isRide_ = false;



	/*   梯子   */

	// 梯子を上る速度を初期化する
	ladderVel_ = 0.0f;

	// Wキーで、梯子を上る
	if (keys[DIK_W] || stickY < -16000)
	{
		if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_LADDER ||
			Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_LADDER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_LADDER ||
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER)
		{
			ladderVel_ = 4.0f;

			// 梯子を使う（梯子フラグがtrueになる）
			isLadderClimd_ = true;

			// ジャンプできなくなる（ジャンプフラグがfalseになる）
			jump_.isJump = false;
			jump_.fallingVel = 0.0f;
			vel_.y = 0.0f;

			// マップを一時的に更新する
			MapUpdate(0.0f, ladderVel_);

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
				// 一時的にマップを更新する
				MapUpdate(shape_.scale.x, vel_.y);

				// 水に触れたら凍る
				if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_WATER)
				{
					Map::map_[map_.leftTop.row][map_.leftTop.column] = TILE_ROTTED;
				}

				// 押し込み処理
				shape_.translate.y = static_cast<float>(kScreenHeight - map_.leftTop.row * kTileSize) - shape_.scale.y;
				LocalToScreen();

				// 梯子を上る速度を消す
				ladderVel_ = 0.0f;
			}

			shape_.translate.y += ladderVel_;
			LocalToScreen();
		}
		else
		{
			// 梯子を上らない（梯子フラグがfalseになる）
			isLadderClimd_ = false;
		}
	}
	else
	{
		// 梯子を上らない（梯子フラグがfalseになる）
		isLadderClimd_ = false;
	}


	/*   ジャンプ   */

	// Jキーで、ジャンプする（ジャンプフラグがtrueになる）
	if (!preKeys[DIK_J] && keys[DIK_J] || Novice::IsTriggerButton(0, kPadButton10))
	{
		if (jump_.isJump == false)
		{
			if (carry_.isCarry == false)
			{
				jump_.isJump = true;
				jump_.fallingVel = -0.5f;

				// 着地したときの効果音
				int shLanding = Novice::LoadAudio("./Resources/Sounds/Se/playerLanding.mp3");
				Novice::PlayAudio(shLanding, 0, 0.3f);

				// 上方向に上昇する
				vel_.y = 12.0f;
			}
		}
	}

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


		// 一時的にマップを更新する
		MapUpdate(shape_.scale.x, -1.0f);

		// 水の上にいたら、凍る
		if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER)
		{
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] = TILE_ROTTED;
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

		// 上に飛んでいるとき
		if (vel_.y > 0.0f)
		{
			// マップを一時的に更新する
			MapUpdate(0.0f, vel_.y);

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
				// 一時的にマップを更新する
				MapUpdate(shape_.scale.x, vel_.y);

				// 水に触れたら凍る
				if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_WATER)
				{
					Map::map_[map_.leftTop.row][map_.leftTop.column] = TILE_ROTTED;
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

			// マップを一時的に更新する
			MapUpdate(0.0f, vel_.y);

			// 着地したときの効果音
			int shLanding = Novice::LoadAudio("./Resources/Sounds/Se/playerLanding.mp3");

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
				// 一時的にマップを更新する
				MapUpdate(shape_.scale.x, vel_.y);

				// 水に着地したら、凍る
				if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER)
				{
					Map::map_[map_.leftBottom.row][map_.leftBottom.column] = TILE_ROTTED;
				}

				// 押し込み処理
				shape_.translate.y = static_cast<float>(kScreenHeight - map_.leftBottom.row * kTileSize) + shape_.scale.y;
				LocalToScreen();

				// 早く落下したら、効果音と粒子をだす
				if (vel_.y < -4.0f)
				{
					Novice::PlayAudio(shLanding, 0, 0.3f);

					// 粒子が出る
					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < kParticleLanding; j++)
						{
							if (landing[j]->isEmission_ == false)
							{
								landing[j]->Emission({ shape_.translate.x , shape_.translate.y - shape_.scale.y });

								break;
							}
						}
					}
				}

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

					// 早く落下したら、効果音と粒子をだす
					if (vel_.y < -4.0f)
					{
						Novice::PlayAudio(shLanding, 0, 0.3f);

						// 粒子が出る
						for (int i = 0; i < 4; i++)
						{
							for (int j = 0; j < kParticleLanding; j++)
							{
								if (landing[j]->isEmission_ == false)
								{
									landing[j]->Emission({ shape_.translate.x , shape_.translate.y - shape_.scale.y });

									break;
								}
							}
						}
					}

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

					// 早く落下したら、効果音と粒子をだす
					if (vel_.y < -4.0f)
					{
						Novice::PlayAudio(shLanding, 0, 0.3f);

						// 粒子が出る
						for (int i = 0; i < 4; i++)
						{
							for (int j = 0; j < kParticleLanding; j++)
							{
								if (landing[j]->isEmission_ == false)
								{
									landing[j]->Emission({ shape_.translate.x , shape_.translate.y - shape_.scale.y });

									break;
								}
							}
						}
					}

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

					// 早く落下したら、効果音と粒子をだす
					if (vel_.y < -4.0f)
					{
						Novice::PlayAudio(shLanding, 0, 0.3f);

						// 粒子が出る
						for (int i = 0; i < 4; i++)
						{
							for (int j = 0; j < kParticleLanding; j++)
							{
								if (landing[j]->isEmission_ == false)
								{
									landing[j]->Emission({ shape_.translate.x , shape_.translate.y - shape_.scale.y });

									break;
								}
							}
						}
					}

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

					// 早く落下したら、効果音と粒子をだす
					if (vel_.y < -4.0f)
					{
						Novice::PlayAudio(shLanding, 0, 0.3f);

						// 粒子が出る
						for (int i = 0; i < 4; i++)
						{
							for (int j = 0; j < kParticleLanding; j++)
							{
								if (landing[j]->isEmission_ == false)
								{
									landing[j]->Emission({ shape_.translate.x , shape_.translate.y - shape_.scale.y });

									break;
								}
							}
						}
					}

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


	/*  奈落に落ちたらゲームオーバーになる   */

	if (shape_.translate.y <= -40)
	{
		//奈落に落ちたら、フラグをfalseにする
		respawn_.isRespawn = false;
	}

	/*   フレーム   */

	// 歩き（90フレームまで）

	walk_.frame++;

	// 歩く時の音
	if (walk_.isWalk)
	{
		if (jump_.isJump == false)
		{
			if (walk_.frame % 20 == 0)
			{
				int shWalk = Novice::LoadAudio("./Resources/Sounds/Se/walk.mp3");
				Novice::PlayAudio(shWalk, 0, 0.2f);
			}
		}
	}

	if (walk_.frame >= 60)
	{
		walk_.frame = 0;
	}

	// 運び

	if (carry_.isCarry)
	{
		carry_.frame++;

		if (carry_.frame == 5)
		{
			int shCarry = Novice::LoadAudio("./Resources/Sounds/Se/carry.mp3");
			Novice::PlayAudio(shCarry, 0, 0.2f);
		}

		if (carry_.frame >= 60)
		{
			carry_.frame = 0;
		}
	}
	else
	{
		carry_.frame = 0;
	}
}

/// <summary>
/// ブロックの上に着地する
/// </summary>
/// <param name="block">ブロック</param>
void Player::BlockLanding(CarryBlock* block)
{
	// null を探す
	if (block == nullptr)
	{
		return;
	}

	// やられている（復活フラグがfalseである）ときは、処理しない
	if (respawn_.isRespawn == false)
	{
		return;
	}

	// ブロックが配置されていない（配置フラグがfalseである）ときは、処理をしない
	if (block->isPut_ == false)
	{
		return;
	}

	// ジャンプしていない（ジャンプフラグがfalseである）ときは、処理しない
	if (jump_.isJump == false)
	{
		return;
	}


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
				if (shape_.translate.y - 1.0f + shape_.scale.y < block->shape_.translate.y - block->shape_.scale.y ||
					shape_.translate.y - 1.0f + shape_.scale.y > block->shape_.translate.y + block->shape_.scale.y)
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
					// 着地したときの効果音
					int shLanding = Novice::LoadAudio("./Resources/Sounds/Se/playerLanding.mp3");

					// 押し込み処理
					shape_.translate.y = block->shape_.translate.y + block->shape_.scale.y + shape_.scale.y;
					LocalToScreen();

					// 早く落下したら、効果音と粒子をだす
					if (vel_.y < -4.0f)
					{
						Novice::PlayAudio(shLanding, 0, 0.3f);

						// 粒子が出る
						for (int i = 0; i < 4; i++)
						{
							for (int j = 0; j < kParticleLanding; j++)
							{
								if (landing[j]->isEmission_ == false)
								{
									landing[i]->Emission({ shape_.translate.x , shape_.translate.y - shape_.scale.y });

									break;
								}
							}
						}
					}

					jump_.isJump = false;
					jump_.fallingVel = 0.0f;

					vel_.y = 0.0f;

					// 乗っかりフラグがtrueになる
					isRide_ = true;
				}
			}
		}
	}
}

/// <summary>
/// ブロックを運ぶ
/// </summary>
/// <param name="keys">キー</param>
/// <param name="preKeys">前のキー</param>
/// <param name="block1">運ぶブロック</param>
/// <param name="block2">他のブロック</param>
void Player::Carry(const char* keys, const char* preKeys, CarryBlock* block1, CarryBlock* block2)
{
	// null を探す
	if (keys == nullptr || preKeys == nullptr || block1 == nullptr || block2 == nullptr)
	{
		return;
	}

	// やられている（復活フラグがfalseである）ときは、操作できない
	if (respawn_.isRespawn == false)
	{
		return;
	}

	// 運ぶブロックが配置されていない（配置フラグがfalseである）ときは、処理をしない
	if (block1->isPut_ == false)
	{
		return;
	}


	/*   運ぶ   */

	// スペースキーで、運ぶことができる
	if (keys[DIK_SPACE] || Novice::IsPressButton(0, PadButton::kPadButton11))
	{
		// ジャンプしていない（ジャンプフラグがfalseである）とき
		if (jump_.isJump == false && block1->jump_.isJump == false)
		{
			// 運びたいブロックに、別のブロックが乗っかていない（のかかりフラグがfalseである）ときは、運べる（運びフラグがtrueになる）
			if (block1->isUnderRide_ == false)
			{
				// 右
				if (shape_.translate.x + shape_.scale.x > block1->shape_.translate.x - block1->shape_.scale.x &&
					shape_.translate.x + shape_.scale.x < block1->shape_.translate.x + block1->shape_.scale.x)
				{
					if (shape_.translate.y + shape_.scale.y / 2 > block1->shape_.translate.y - block1->shape_.scale.y &&
						shape_.translate.y - shape_.scale.y / 2 < block1->shape_.translate.y + block1->shape_.scale.y)
					{
						// 右から押すとき
						if (vel_.x > 0.0f)
						{
							block1->vel_.x = vel_.x / 2.0f;

							carry_.isCarry = true;

							// 一時的にマップを更新する
							block1->MapUpdate(block1->vel_.x, 0.0f);

							// 進んだ先で壁にぶつかるとき
							if (Map::map_[block1->map_.rightTop.row][block1->map_.rightTop.column] == TILE_GROUND ||
								Map::map_[block1->map_.rightBottom.row][block1->map_.rightBottom.column] == TILE_GROUND ||
								Map::map_[block1->map_.rightTop.row][block1->map_.rightTop.column] == TILE_BLOCK ||
								Map::map_[block1->map_.rightBottom.row][block1->map_.rightBottom.column] == TILE_BLOCK ||
								Map::map_[block1->map_.rightTop.row][block1->map_.rightTop.column] == TILE_ROTTED ||
								Map::map_[block1->map_.rightBottom.row][block1->map_.rightBottom.column] == TILE_ROTTED ||
								Map::map_[block1->map_.rightTop.row][block1->map_.rightTop.column] == TILE_WATER ||
								Map::map_[block1->map_.rightBottom.row][block1->map_.rightBottom.column] == TILE_WATER ||
								Map::map_[block1->map_.rightTop.row][block1->map_.rightTop.column] == TILE_CONCRETE ||
								Map::map_[block1->map_.rightBottom.row][block1->map_.rightBottom.column] == TILE_CONCRETE ||
								Map::map_[block1->map_.rightTop.row][block1->map_.rightTop.column] < 0 || Map::map_[block1->map_.rightBottom.row][block1->map_.rightBottom.column] < 0)
							{
								block1->vel_.x = 0.0f;

								// 押し込み処理
								block1->shape_.translate.x = static_cast<float>(block1->map_.rightTop.column * kTileSize) - block1->shape_.scale.x;
							}

							// 画面端にぶつかった場合
							if (block1->shape_.translate.x + block1->vel_.x + block1->shape_.scale.x > static_cast<float>(kScreenWidth))
							{
								block1->vel_.x = 0.0f;

								// 押し込み処理
								block1->shape_.translate.x = static_cast<float>(kScreenWidth) - block1->shape_.scale.x;
							}

							// 動かす
							block1->shape_.translate.x += block1->vel_.x;
							block1->LocalToScreen();

							vel_.x = 0.0f;
							shape_.translate.x = block1->shape_.translate.x - block1->shape_.scale.x - shape_.scale.x;
							LocalToScreen();
						}
					}
				}

				// 左
				if (shape_.translate.x - shape_.scale.x < block1->shape_.translate.x + block1->shape_.scale.x &&
					shape_.translate.x - shape_.scale.x > block1->shape_.translate.x - block1->shape_.scale.x)
				{
					if (shape_.translate.y + shape_.scale.y > block1->shape_.translate.y - block1->shape_.scale.y &&
						shape_.translate.y - shape_.scale.y < block1->shape_.translate.y + block1->shape_.scale.y)
					{
						// 左から押すとき
						if (vel_.x < 0.0f)
						{
							block1->vel_.x = vel_.x / 2.0f;

							carry_.isCarry = true;

							// 一時的にマップを更新する
							block1->MapUpdate(block1->vel_.x, 0.0f);

							// 進んだ先で壁にぶつかるとき
							if (Map::map_[block1->map_.leftTop.row][block1->map_.leftTop.column] == TILE_GROUND ||
								Map::map_[block1->map_.leftBottom.row][block1->map_.leftBottom.column] == TILE_GROUND ||
								Map::map_[block1->map_.leftTop.row][block1->map_.leftTop.column] == TILE_BLOCK ||
								Map::map_[block1->map_.leftBottom.row][block1->map_.leftBottom.column] == TILE_BLOCK ||
								Map::map_[block1->map_.leftTop.row][block1->map_.leftTop.column] == TILE_ROTTED ||
								Map::map_[block1->map_.leftBottom.row][block1->map_.leftBottom.column] == TILE_ROTTED ||
								Map::map_[block1->map_.leftTop.row][block1->map_.leftTop.column] == TILE_WATER ||
								Map::map_[block1->map_.leftBottom.row][block1->map_.leftBottom.column] == TILE_WATER ||
								Map::map_[block1->map_.leftTop.row][block1->map_.leftTop.column] == TILE_CONCRETE ||
								Map::map_[block1->map_.leftBottom.row][block1->map_.leftBottom.column] == TILE_CONCRETE ||
								Map::map_[block1->map_.leftTop.row][block1->map_.leftTop.column] < 0 || Map::map_[block1->map_.leftBottom.row][block1->map_.leftBottom.column] < 0)
							{
								block1->vel_.x = 0.0f;

								// 押し込み処理
								block1->shape_.translate.x = static_cast<float>(block1->map_.leftTop.column * kTileSize + kTileSize) + block1->shape_.scale.x;
							}

							// 画面端にぶつかった場合
							if (block1->shape_.translate.x + block1->vel_.x - block1->shape_.scale.x < 0.0f)
							{
								block1->vel_.x = 0.0f;

								// 押し込み処理
								block1->shape_.translate.x = block1->shape_.scale.x;
							}

							// ブロックを動かす
							block1->shape_.translate.x += block1->vel_.x;
							block1->LocalToScreen();

							// プレイヤーの押し込み処理
							vel_.x = 0.0f;
							shape_.translate.x = block1->shape_.translate.x + block1->shape_.scale.x + shape_.scale.x;
							LocalToScreen();
						}
					}
				}

				// もう一つのブロックが配置されていた（配置フラグがtrueである）とき
				if (block2->isPut_)
				{
					// 右から
					if (block1->shape_.translate.x + block1->shape_.scale.x > block2->shape_.translate.x - block2->shape_.scale.x &&
						block1->shape_.translate.x + block1->shape_.scale.x < block2->shape_.translate.x + block2->shape_.scale.x)
					{
						if (block1->shape_.translate.y + block1->shape_.scale.y - 1.0f > block2->shape_.translate.y - block2->shape_.scale.y + 1.0f &&
							block1->shape_.translate.y - block1->shape_.scale.y + 1.0f < block2->shape_.translate.y + block2->shape_.scale.y - 1.0f)
						{
							block1->vel_.x = 0.0f;

							// 押し込み処理
							block1->shape_.translate.x = block2->shape_.translate.x - block2->shape_.scale.x - block1->shape_.scale.x;
							block1->LocalToScreen();

							// プレイヤーの押し込み処理
							vel_.x = 0.0f;
							shape_.translate.x = block1->shape_.translate.x - block1->shape_.scale.x - shape_.scale.x;
							LocalToScreen();
						}
					}

					// 左から
					if (block1->shape_.translate.x - block1->shape_.scale.x < block2->shape_.translate.x + block2->shape_.scale.x &&
						block1->shape_.translate.x - block1->shape_.scale.x > block2->shape_.translate.x - block2->shape_.scale.x)
					{
						if (block1->shape_.translate.y + block1->shape_.scale.y - 1.0f > block2->shape_.translate.y - block2->shape_.scale.y + 1.0f &&
							block1->shape_.translate.y - block1->shape_.scale.y + 1.0f < block2->shape_.translate.y + block2->shape_.scale.y - 1.0f)
						{
							block1->vel_.x = 0.0f;

							// 押し込み処理
							block1->shape_.translate.x = block2->shape_.translate.x + block2->shape_.scale.x + block1->shape_.scale.x;
							block1->LocalToScreen();

							// プレイヤーの押し込み処理
							vel_.x = 0.0f;
							shape_.translate.x = block1->shape_.translate.x + block1->shape_.scale.x + shape_.scale.x;
							LocalToScreen();
						}
					}
				}

				// うまく動かせた場合は、パーティクルを放出させる
				if (block1->vel_.x != 0.0f)
				{
					if (SnowCarry::coolTime <= 0)
					{
						for (int i = 0; i < kParticleSnowCarry; i++)
						{
							if (snowCarry[i]->isEmission_ == false)
							{
								// クールタイムを開始する
								SnowCarry::coolTime = 3;

								snowCarry[i]->Emission({ block1->shape_.translate.x - block1->shape_.scale.x +
									static_cast<float>(rand() % static_cast<int>(block1->shape_.scale.x * 2)) ,block1->shape_.translate.y - block1->shape_.scale.y });

								break;
							}
						}
					}
				}

				// 移動速度を初期化する
				block1->vel_.x = 0.0f;
			} 
			else
			{
				// 運びたいブロックに、別のブロックが乗っかているときは、運べない

				if (vel_.y >= -1.0f)
				{
					// 右から押すとき
					if (vel_.x > 0.0f)
					{
						if (shape_.translate.x + shape_.scale.x > block1->shape_.translate.x - block1->shape_.scale.x &&
							shape_.translate.x + shape_.scale.x < block1->shape_.translate.x + block1->shape_.scale.x)
						{
							if (shape_.translate.y + shape_.scale.y - 1.0f > block1->shape_.translate.y - block1->shape_.scale.y &&
								shape_.translate.y - shape_.scale.y + 1.0f < block1->shape_.translate.y + block1->shape_.scale.y)
							{
								shape_.translate.x = block1->shape_.translate.x - block1->shape_.scale.x - shape_.scale.x;
								LocalToScreen();

								vel_.x = 0.0f;
							}
						}
					} else if (vel_.x < 0.0f)
					{
						// 左から押すとき

						if (shape_.translate.x - shape_.scale.x - 1.0f < block1->shape_.translate.x + block1->shape_.scale.x &&
							shape_.translate.x - shape_.scale.x + 1.0f > block1->shape_.translate.x - block1->shape_.scale.x)
						{
							if (shape_.translate.y + shape_.scale.y - 1.0f > block1->shape_.translate.y - block1->shape_.scale.y &&
								shape_.translate.y - shape_.scale.y + 1.0f < block1->shape_.translate.y + block1->shape_.scale.y)
							{
								shape_.translate.x = block1->shape_.translate.x + block1->shape_.scale.x + shape_.scale.x;
								LocalToScreen();

								vel_.x = 0.0f;
							}
						}
					}
				}
				else
				{
					// 右から押すとき
					if (vel_.x > 0.0f)
					{
						if (shape_.translate.x + shape_.scale.x + 1.0f + vel_.x > block1->shape_.translate.x - block1->shape_.scale.x &&
							shape_.translate.x + shape_.scale.x + 1.0f + vel_.x < block1->shape_.translate.x + block1->shape_.scale.x)
						{
							if (shape_.translate.y + shape_.scale.y > block1->shape_.translate.y - block1->shape_.scale.y &&
								shape_.translate.y - shape_.scale.y < block1->shape_.translate.y + block1->shape_.scale.y)
							{
								walk_.isWalk = false;

								shape_.translate.x = block1->shape_.translate.x - block1->shape_.scale.x - shape_.scale.x - 1.0f - vel_.x;
								LocalToScreen();

								// 壁にぶつかる
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
									// 押し込み処理
									shape_.translate.x = static_cast<float>(map_.leftTop.column * kTileSize + kTileSize) + shape_.scale.x;
									LocalToScreen();
								}

								vel_.x = 0.0f;
							}
						}
					} else if (vel_.x < 0.0f)
					{
						// 左から押すとき

						if (shape_.translate.x - shape_.scale.x - 1.0f + vel_.x < block1->shape_.translate.x + block1->shape_.scale.x &&
							shape_.translate.x - shape_.scale.x - 1.0f + vel_.x > block1->shape_.translate.x - block1->shape_.scale.x)
						{
							if (shape_.translate.y + shape_.scale.y > block1->shape_.translate.y - block1->shape_.scale.y &&
								shape_.translate.y - shape_.scale.y < block1->shape_.translate.y + block1->shape_.scale.y)
							{
								walk_.isWalk = false;

								shape_.translate.x = block1->shape_.translate.x + block1->shape_.scale.x + shape_.scale.x + 1.0f - vel_.x;
								LocalToScreen();

								// 壁にぶつかる
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
									// 押し込み処理
									shape_.translate.x = static_cast<float>(map_.rightTop.column * kTileSize) - shape_.scale.x;
									LocalToScreen();
								}

								vel_.x = 0.0f;
							}
						}
					}
				}
			}
		} 
		else if (jump_.isJump || block1->jump_.isJump)
		{
			// ジャンプしている（ジャンプフラグがtrueである）ときは、押すことができない（運びフラグがfalseになる）

			carry_.isCarry = false;

			if (vel_.y >= -1.0f)
			{
				// 右から押すとき
				if (vel_.x > 0.0f)
				{
					if (shape_.translate.x + shape_.scale.x > block1->shape_.translate.x - block1->shape_.scale.x &&
						shape_.translate.x + shape_.scale.x < block1->shape_.translate.x + block1->shape_.scale.x)
					{
						if (shape_.translate.y + shape_.scale.y - 1.0f > block1->shape_.translate.y - block1->shape_.scale.y &&
							shape_.translate.y - shape_.scale.y + 1.0f < block1->shape_.translate.y + block1->shape_.scale.y)
						{
							shape_.translate.x = block1->shape_.translate.x - block1->shape_.scale.x - shape_.scale.x;
							LocalToScreen();

							vel_.x = 0.0f;
						}
					}
				} 
				else if (vel_.x < 0.0f)
				{
					// 左から押すとき

					if (shape_.translate.x - shape_.scale.x - 1.0f < block1->shape_.translate.x + block1->shape_.scale.x &&
						shape_.translate.x - shape_.scale.x + 1.0f > block1->shape_.translate.x - block1->shape_.scale.x)
					{
						if (shape_.translate.y + shape_.scale.y - 1.0f > block1->shape_.translate.y - block1->shape_.scale.y &&
							shape_.translate.y - shape_.scale.y + 1.0f < block1->shape_.translate.y + block1->shape_.scale.y)
						{
							shape_.translate.x = block1->shape_.translate.x + block1->shape_.scale.x + shape_.scale.x;
							LocalToScreen();

							vel_.x = 0.0f;
						}
					}
				}
			} 
			else
			{
				// 右から押すとき
				if (vel_.x > 0.0f)
				{
					if (shape_.translate.x + shape_.scale.x + 1.0f + vel_.x > block1->shape_.translate.x - block1->shape_.scale.x &&
						shape_.translate.x + shape_.scale.x + 1.0f + vel_.x < block1->shape_.translate.x + block1->shape_.scale.x)
					{
						if (shape_.translate.y + shape_.scale.y > block1->shape_.translate.y - block1->shape_.scale.y &&
							shape_.translate.y - shape_.scale.y < block1->shape_.translate.y + block1->shape_.scale.y)
						{
							walk_.isWalk = false;

							shape_.translate.x = block1->shape_.translate.x - block1->shape_.scale.x - shape_.scale.x - 1.0f - vel_.x;
							LocalToScreen();

							// 壁にぶつかる
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
								// 押し込み処理
								shape_.translate.x = static_cast<float>(map_.leftTop.column * kTileSize + kTileSize) + shape_.scale.x;
								LocalToScreen();
							}

							vel_.x = 0.0f;
						}
					}
				} 
				else if (vel_.x < 0.0f)
				{
					// 左から押すとき

					if (shape_.translate.x - shape_.scale.x - 1.0f + vel_.x < block1->shape_.translate.x + block1->shape_.scale.x &&
						shape_.translate.x - shape_.scale.x - 1.0f + vel_.x > block1->shape_.translate.x - block1->shape_.scale.x)
					{
						if (shape_.translate.y + shape_.scale.y > block1->shape_.translate.y - block1->shape_.scale.y &&
							shape_.translate.y - shape_.scale.y < block1->shape_.translate.y + block1->shape_.scale.y)
						{
							walk_.isWalk = false;

							shape_.translate.x = block1->shape_.translate.x + block1->shape_.scale.x + shape_.scale.x + 1.0f - vel_.x;
							LocalToScreen();

							// 壁にぶつかる
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
								// 押し込み処理
								shape_.translate.x = static_cast<float>(map_.rightTop.column * kTileSize) - shape_.scale.x;
								LocalToScreen();
							}

							vel_.x = 0.0f;
						}
					}
				}
			}
		}
	} 
	else
	{
		// スペースキーを押してないときは、ブロックにぶつかる（運びフラグがfalseになる）

		carry_.isCarry = false;

		if (vel_.y >= -1.0f)
		{
			// 右から押すとき
			if (vel_.x > 0.0f)
			{
				if (shape_.translate.x + shape_.scale.x > block1->shape_.translate.x - block1->shape_.scale.x &&
					shape_.translate.x + shape_.scale.x < block1->shape_.translate.x + block1->shape_.scale.x)
				{
					if (shape_.translate.y + shape_.scale.y - 1.0f > block1->shape_.translate.y - block1->shape_.scale.y &&
						shape_.translate.y - shape_.scale.y + 1.0f < block1->shape_.translate.y + block1->shape_.scale.y)
					{
						walk_.isWalk = false;

						shape_.translate.x = block1->shape_.translate.x - block1->shape_.scale.x - shape_.scale.x;
						LocalToScreen();

						vel_.x = 0.0f;
					}
				}
			}
			else if (vel_.x < 0.0f)
			{
				// 左から押すとき

				if (shape_.translate.x - shape_.scale.x < block1->shape_.translate.x + block1->shape_.scale.x &&
					shape_.translate.x - shape_.scale.x > block1->shape_.translate.x - block1->shape_.scale.x)
				{
					if (shape_.translate.y + shape_.scale.y - 1.0f > block1->shape_.translate.y - block1->shape_.scale.y &&
						shape_.translate.y - shape_.scale.y + 1.0f < block1->shape_.translate.y + block1->shape_.scale.y)
					{
						walk_.isWalk = false;

						shape_.translate.x = block1->shape_.translate.x + block1->shape_.scale.x + shape_.scale.x;
						LocalToScreen();

						vel_.x = 0.0f;
					}
				}
			}
		} 
		else
		{
			// 右から押すとき
			if (vel_.x > 0.0f)
			{
				if (shape_.translate.x + shape_.scale.x + 1.0f + vel_.x > block1->shape_.translate.x - block1->shape_.scale.x &&
					shape_.translate.x + shape_.scale.x + 1.0f + vel_.x < block1->shape_.translate.x + block1->shape_.scale.x)
				{
					if (shape_.translate.y + shape_.scale.y > block1->shape_.translate.y - block1->shape_.scale.y &&
						shape_.translate.y - shape_.scale.y < block1->shape_.translate.y + block1->shape_.scale.y)
					{
						walk_.isWalk = false;

						shape_.translate.x = block1->shape_.translate.x - block1->shape_.scale.x - shape_.scale.x - 1.0f - vel_.x;
						LocalToScreen();

						// 壁にぶつかる
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
							// 押し込み処理
							shape_.translate.x = static_cast<float>(map_.leftTop.column * kTileSize + kTileSize) + shape_.scale.x;
							LocalToScreen();
						}

						vel_.x = 0.0f;
					}
				}
			} 
			else if (vel_.x < 0.0f)
			{
				// 左から押すとき

				if (shape_.translate.x - shape_.scale.x - 1.0f + vel_.x < block1->shape_.translate.x + block1->shape_.scale.x &&
					shape_.translate.x - shape_.scale.x - 1.0f + vel_.x > block1->shape_.translate.x - block1->shape_.scale.x)
				{
					if (shape_.translate.y + shape_.scale.y > block1->shape_.translate.y - block1->shape_.scale.y &&
						shape_.translate.y - shape_.scale.y < block1->shape_.translate.y + block1->shape_.scale.y)
					{
						walk_.isWalk = false;

						shape_.translate.x = block1->shape_.translate.x + block1->shape_.scale.x + shape_.scale.x + 1.0f - vel_.x;
						LocalToScreen();

						// 壁にぶつかる）
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
							// 押し込み処理
							shape_.translate.x = static_cast<float>(map_.rightTop.column * kTileSize) - shape_.scale.x;
							LocalToScreen();
						}

						vel_.x = 0.0f;
					}
				}
			}
		}
	}
}

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="enemy">敵</param>
void Player::Hit(Enemy* enemy)
{
	// null を探す
	if (enemy == nullptr)
	{
		return;
	}

	// やられている（復活フラグがfalseである）ときは、当たらない
	if (respawn_.isRespawn == false)
	{
		return;
	}

	// 出現していない（出現フラグがfalseである）ときは、処理しない
	if (enemy->isArrival_ == false)
	{
		return;
	}

	// 敵に当たったら、やられる（復活フラグがfalseになる）
	if (shape_.translate.x + shape_.scale.x > enemy->shape_.translate.x - enemy->shape_.scale.x &&
		shape_.translate.x - shape_.scale.x < enemy->shape_.translate.x + enemy->shape_.scale.x)
	{
		if (shape_.translate.y + shape_.scale.y > enemy->shape_.translate.y - enemy->shape_.scale.y &&
			shape_.translate.y - shape_.scale.y < enemy->shape_.translate.y + enemy->shape_.scale.y)
		{
			respawn_.isRespawn = false;
		}
	}
}

/// <summary>
/// 描画する
/// </summary>
void Player::Draw()
{
	// 復活していない（復活フラグがfalseである）ときは、描画しない
	if (respawn_.isRespawn == false)
	{
		return;
	}


	//止まっている時
	int ghplayer = Novice::LoadTexture("./Resources/Images/Player/stand/player.png");

	//歩く画像
	int ghPlayerWalk[10];
	ghPlayerWalk[0] = Novice::LoadTexture("./Resources/Images/Player/Walk/player_walk1.png");
	ghPlayerWalk[1] = Novice::LoadTexture("./Resources/Images/Player/Walk/player_walk2.png");
	ghPlayerWalk[2] = Novice::LoadTexture("./Resources/Images/Player/Walk/player_walk3.png");
	ghPlayerWalk[3] = Novice::LoadTexture("./Resources/Images/Player/Walk/player_walk4.png");
	ghPlayerWalk[4] = Novice::LoadTexture("./Resources/Images/Player/Walk/player_walk5.png");
	ghPlayerWalk[5] = Novice::LoadTexture("./Resources/Images/Player/Walk/player_walk6.png");
	ghPlayerWalk[6] = Novice::LoadTexture("./Resources/Images/Player/Walk/player_walk7.png");
	ghPlayerWalk[7] = Novice::LoadTexture("./Resources/Images/Player/Walk/player_walk8.png");
	ghPlayerWalk[8] = Novice::LoadTexture("./Resources/Images/Player/Walk/player_walk9.png");
	ghPlayerWalk[9] = Novice::LoadTexture("./Resources/Images/Player/Walk/player_walk10.png");

	int ghplayerCarry[10];
	ghplayerCarry[0] = Novice::LoadTexture("./Resources/Images/Player/Carry/player_Carry1.png");
	ghplayerCarry[1] = Novice::LoadTexture("./Resources/Images/Player/Carry/player_Carry2.png");
	ghplayerCarry[2] = Novice::LoadTexture("./Resources/Images/Player/Carry/player_Carry3.png");
	ghplayerCarry[3] = Novice::LoadTexture("./Resources/Images/Player/Carry/player_Carry4.png");
	ghplayerCarry[4] = Novice::LoadTexture("./Resources/Images/Player/Carry/player_Carry5.png");
	ghplayerCarry[5] = Novice::LoadTexture("./Resources/Images/Player/Carry/player_Carry6.png");
	ghplayerCarry[6] = Novice::LoadTexture("./Resources/Images/Player/Carry/player_Carry7.png");
	ghplayerCarry[7] = Novice::LoadTexture("./Resources/Images/Player/Carry/player_Carry8.png");
	ghplayerCarry[8] = Novice::LoadTexture("./Resources/Images/Player/Carry/player_Carry9.png");
	ghplayerCarry[9] = Novice::LoadTexture("./Resources/Images/Player/Carry/player_Carry10.png");

	int ghPlayerLadderClimd[2];
	ghPlayerLadderClimd[0] = Novice::LoadTexture("./Resources/Images/Player/ladder/player_ladder1.png");
	ghPlayerLadderClimd[1] = Novice::LoadTexture("./Resources/Images/Player/ladder/player_ladder2.png");

	if (!walk_.isWalk)
	{
		if (!isLadderClimd_)
		{
			//待機中
			if (directionNo == DIRECTION_RIGHT)
			{
				Novice::DrawSprite
				(static_cast<int>(pos_.screen.leftTop.x - 15.5f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
					ghplayer, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
				);
			}
			else if (directionNo == DIRECTION_LEFT)
			{
				Novice::DrawSprite
				(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
					ghplayer, -1.0f, 1.0f, 0.0f, 0xFFFFFFFF
				);
			}
		}
		else
		{
			if (walk_.frame <= 30)
			{
				Novice::DrawSprite
				(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
					ghPlayerLadderClimd[0], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
				);
			}
			else if (walk_.frame <= 60)
			{
				Novice::DrawSprite
				(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
					ghPlayerLadderClimd[1], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
				);
			}
		}

	}
	else
	{
		if (!isLadderClimd_)
		{
			if (!carry_.isCarry)
			{
				//歩きモーション
				if (directionNo == DIRECTION_RIGHT)
				{
					if (walk_.frame <= 6)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghPlayerWalk[0], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 12)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghPlayerWalk[1], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 18)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghPlayerWalk[2], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 24)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghPlayerWalk[3], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 30)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghPlayerWalk[4], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 36)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghPlayerWalk[5], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 42)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghPlayerWalk[6], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 48)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghPlayerWalk[7], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 54)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghPlayerWalk[8], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 60)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghPlayerWalk[9], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
				}
				else if (directionNo == DIRECTION_LEFT)
				{
					if (walk_.frame <= 6)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghPlayerWalk[0], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 12)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghPlayerWalk[1], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 18)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghPlayerWalk[2], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 24)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghPlayerWalk[3], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 30)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghPlayerWalk[4], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 36)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghPlayerWalk[5], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 42)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghPlayerWalk[6], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 48)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghPlayerWalk[7], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 54)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghPlayerWalk[8], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 60)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghPlayerWalk[9], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
				}
			}
			else
			{
				//運ぶモーション
				if (directionNo == DIRECTION_RIGHT)
				{
					if (walk_.frame <= 6)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghplayerCarry[0], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 12)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghplayerCarry[1], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 18)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghplayerCarry[2], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 24)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghplayerCarry[3], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 30)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghplayerCarry[4], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 36)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghplayerCarry[5], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 42)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghplayerCarry[6], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 48)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghplayerCarry[7], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 54)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghplayerCarry[8], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
					else if (walk_.frame <= 60)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
							ghplayerCarry[9], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
						);
					}
				}
				else if (directionNo == DIRECTION_LEFT)
				{
					if (walk_.frame <= 6)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghplayerCarry[0], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 12)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghplayerCarry[1], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 18)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghplayerCarry[2], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 24)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghplayerCarry[3], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 30)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghplayerCarry[4], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 36)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghplayerCarry[5], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 42)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghplayerCarry[6], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 48)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghplayerCarry[7], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 54)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghplayerCarry[8], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
					else if (walk_.frame <= 60)
					{
						Novice::DrawSprite
						(static_cast<int>(pos_.screen.rightTop.x + 16.0f), static_cast<int>(pos_.screen.rightTop.y - 24.0f),
							ghplayerCarry[9], -1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
					}
				}
			}
		}
		else
		{
			//梯子登るモーション
			if (walk_.frame <= 30)
			{
				Novice::DrawSprite
				(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
					ghPlayerLadderClimd[0], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
				);
			}
			else if (walk_.frame <= 60)
			{
				Novice::DrawSprite
				(static_cast<int>(pos_.screen.leftTop.x - 16.0f), static_cast<int>(pos_.screen.leftTop.y - 24.0f),
					ghPlayerLadderClimd[1], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF
				);
			}
		}

	}
}