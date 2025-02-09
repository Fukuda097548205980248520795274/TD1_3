#include "CarryBlock.h"

int CarryBlock::countID;

/// <summary>
/// 初期化する
/// </summary>
void CarryBlock::InitialValue()
{

	// ID
	id_ = 0;
	--countID;

	// 配置しているかどうか（配置フラグ）
	isPut_ = false;

	// 図形
	shape_.scale = { 23.0f , 24.0f };
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

	// 凍っている時間
	frame_ = { 0 , 0 , 0.0f };

	// パーティクルを消す
	for (int i = 0; i < kParticleLanding; i++)
	{
		landing_[i]->isEmission_ = false;
	}

	for (int i = 0; i < kParticleCold; i++)
	{
		cold_[i]->isEmission_ = false;
	}
}

/// <summary>
/// 配置する
/// </summary>
/// <param name="column">列</param>
/// <param name="row">行</param>
void CarryBlock::Putting(int column, int row)
{
	// 配置している（配置フラグがtrueである）ブロックは、配置しない
	if (isPut_)
	{
		return;
	}


	// ID
	id_ = ++countID;

	// 配置する（配置フラグをtrueにする）
	isPut_ = true;

	// 位置
	shape_.translate = { static_cast<float>(column * kTileSize + kTileSize / 2)  ,
		static_cast<float>((kMapRow - 1 - row) * kTileSize + kTileSize / 2) };
	LocalToScreen();

	// ジャンプ
	jump_.isJump = false;
	jump_.fallingVel = 0.0f;

	// 移動速度
	vel_ = { 0.0f , 0.0f };


	// パーティクルを消す
	for (int i = 0; i < kParticleLanding; i++)
	{
		landing_[i]->isEmission_ = false;
	}

	for (int i = 0; i < kParticleCold; i++)
	{
		cold_[i]->isEmission_ = false;
	}
}

/// <summary>
/// 動かす
/// </summary>
void CarryBlock::Move()
{
	// 配置されていない（配置フラグがfalseである）ブロックは、処理をしない
	if (isPut_ == false)
	{
		return;
	}


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
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_NOTHING &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_NOTHING &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GOAL ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_NOTHING &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER ||
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
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER ||
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
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GOAL &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GOAL &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_TOP ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GOAL &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GOAL &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP ||

			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_NOTHING ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GOAL ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_TOP ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER &&
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
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER ||
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
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER ||
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
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER ||
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
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER ||
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
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_WATER ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_WATER ||
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
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_CONCRETE ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_CONCRETE ||
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

			// 着地した効果音
			int shLanding = Novice::LoadAudio("./Resources/Sounds/Se/blockLanding.mp3");

			// 落ちた先に地面があったら、着地する（ジャンプフラグがfalseになる）
			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GROUND ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GROUND ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_BLOCK ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_BLOCK ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_ROTTED ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_ROTTED ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_CONCRETE ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_CONCRETE ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] < 0 || Map::map_[map_.rightBottom.row][map_.rightBottom.column] < 0)
			{
				// 押し込み処理
				shape_.translate.y = static_cast<float>(kScreenHeight - map_.leftBottom.row * kTileSize) + shape_.scale.y;
				LocalToScreen();

				// 早く落下したら、効果音を鳴らす
				if (vel_.y < -4.0f)
				{
					Novice::PlayAudio(shLanding, 0, 0.3f);

					// 粒子が出る
					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < kParticleLanding; j++)
						{
							if (landing_[j]->isEmission_ == false)
							{
								landing_[j]->Emission({ shape_.translate.x , shape_.translate.y - shape_.scale.y });

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

					// 早く落下したら、効果音を鳴らす
					if (vel_.y < -4.0f)
					{
						Novice::PlayAudio(shLanding, 0, 0.3f);

						// 粒子が出る
						for (int i = 0; i < 4; i++)
						{
							for (int j = 0; j < kParticleLanding; j++)
							{
								if (landing_[j]->isEmission_ == false)
								{
									landing_[j]->Emission({ shape_.translate.x , shape_.translate.y - shape_.scale.y });

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

					// 早く落下したら、効果音を鳴らす
					if (vel_.y < -4.0f)
					{
						Novice::PlayAudio(shLanding, 0, 0.3f);

						// 粒子が出る
						for (int i = 0; i < 4; i++)
						{
							for (int j = 0; j < kParticleLanding; j++)
							{
								if (landing_[j]->isEmission_ == false)
								{
									landing_[j]->Emission({ shape_.translate.x , shape_.translate.y - shape_.scale.y });

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

					// 早く落下したら、効果音を鳴らす
					if (vel_.y < -4.0f)
					{
						Novice::PlayAudio(shLanding, 0, 0.3f);

						// 粒子が出る
						for (int i = 0; i < 4; i++)
						{
							for (int j = 0; j < kParticleLanding; j++)
							{
								if (landing_[j]->isEmission_ == false)
								{
									landing_[j]->Emission({ shape_.translate.x , shape_.translate.y - shape_.scale.y });

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

					// 早く落下したら、効果音を鳴らす
					if (vel_.y < -4.0f)
					{
						Novice::PlayAudio(shLanding, 0, 0.3f);

						// 粒子が出る
						for (int i = 0; i < 4; i++)
						{
							for (int j = 0; j < kParticleLanding; j++)
							{
								if (landing_[j]->isEmission_ == false)
								{
									landing_[j]->Emission({ shape_.translate.x , shape_.translate.y - shape_.scale.y });

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
			} else if (vel_.y < 0.0f)
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
							// 着地した効果音
							int shLanding = Novice::LoadAudio("./Resources/Sounds/Se/blockLanding.mp3");

							// 押し込み処理
							shape_.translate.y = block->shape_.translate.y + block->shape_.scale.y + shape_.scale.y;
							LocalToScreen();

							// 着地したブロックがクッションだと、クッションが壊れる
							if (block->type_ == BLOCK_CUSHION)
							{
								// 壊れる速度でクッションに着地したら、クッションが壊れる（配置フラグがfalesになる）
								block->isPut_ = false;

								// 粒子が出る
								for (int i = 0; i < 8; i++)
								{
									for (int j = 0; j < kParticleLanding; j++)
									{
										if (landing_[j]->isEmission_ == false)
										{
											landing_[j]->Emission({ block->shape_.translate.x , block->shape_.translate.y });

											break;
										}
									}
								}

								// 着地した効果音
								int shCushion = Novice::LoadAudio("./Resources/Sounds/Se/walk.mp3");
								Novice::PlayAudio(shCushion, 0, 0.6f);

								// IDを消す
								block->id_ = 0;
								--countID;
							}

							// 早く落下したら、効果音を鳴らす
							if (vel_.y < -4.0f)
							{
								Novice::PlayAudio(shLanding, 0, 0.3f);

								// 粒子が出る
								for (int i = 0; i < 4; i++)
								{
									for (int j = 0; j < kParticleLanding; j++)
									{
										if (landing_[j]->isEmission_ == false)
										{
											landing_[i]->Emission({ shape_.translate.x , shape_.translate.y - shape_.scale.y });

											break;
										}
									}
								}
							}

							jump_.isJump = false;
							jump_.fallingVel = 0.0f;

							vel_.y = 0.0f;


							// 乗っかる（乗っかりフラグがtrueになる）
							isRide_ = true;

							// 乗っかかる（乗っかかりフラグがtrueになる）
							block->isUnderRide_ = true;
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
void CarryBlock::Draw()
{

}