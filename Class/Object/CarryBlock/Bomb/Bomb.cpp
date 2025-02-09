#include "Bomb.h"

/// <summary>
/// コンストラクタ
/// </summary>
Bomb::Bomb()
{
	// ID
	id_ = 0;

	// 配置しているかどうか（配置フラグ）
	isPut_ = false;

	// 種類
	type_ = BLOCK_BOMB;

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

	// 爆発しそうか（爆発フラグ）
	isExplosion_ = false;

	// フレーム
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
Bomb::~Bomb()
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
void Bomb::Putting(int column, int row)
{
	if (isPut_ == false)
	{
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

		// 爆発しそうか（爆発フラグ）
		isExplosion_ = false;

		// フレーム
		frame_ = { 0 , 300 , 0.0f };

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
void Bomb::Move()
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
		} else
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


	/*   爆発   */

	// 爆発しそうな（爆発フラグがtrueである）ときに、タイマーが動き、終了で爆発する
	if (isExplosion_)
	{
		frame_.current++;

		// 爆発する
		if (frame_.current > frame_.end)
		{
			// 一時的にマップを更新する
			MapUpdate(shape_.scale.x, shape_.scale.y);

			// 範囲内のブロックを消す
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					// 指定したブロックのみが消える
					if (Map::map_[(map_.leftBottom.row - 1) + i][(map_.leftBottom.column - 1) + j] == TILE_BLOCK ||
						Map::map_[(map_.leftBottom.row - 1) + i][(map_.leftBottom.column - 1) + j] == TILE_ROTTED ||
						Map::map_[(map_.leftBottom.row - 1) + i][(map_.leftBottom.column - 1) + j] < 0 ||
						Map::map_[(map_.leftBottom.row - 1) + i][(map_.leftBottom.column - 1) + j] == TILE_CONCRETE)
					{
						Map::map_[(map_.leftBottom.row - 1) + i][(map_.leftBottom.column - 1) + j] = TILE_NOTHING;
					}
				}
			}

			// 爆発した音
			int shExplosion = Novice::LoadAudio("./Resources/Sounds/Se/explosion.mp3");
			Novice::PlayAudio(shExplosion, 0, 0.3f);
		}
	}
}

/// <summary>
/// 描画する
/// </summary>
void Bomb::Draw()
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
		0, 0, 48, 48, ghWhite, 0xAA0000FF
	);
}