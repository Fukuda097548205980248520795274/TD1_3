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

	// 種類
	type_ = BLOCK_TREASURE;

	// 図形
	shape_.scale = { 23.0f , 23.0f };
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
Treasure::~Treasure()
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
/// 動かす
/// </summary>
void Treasure::Move()
{
	// 配置していない（配置フラグがfalseである）ブロックは、処理しない
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
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_LADDER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GOAL &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GOAL ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_WATER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_WATER ||
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

			// 壊れた効果音
			int shBreak = Novice::LoadAudio("./Resources/Sounds/Se/treasureBreak.mp3");

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


				// 早く落下すると、壊れる（配置フラグがfalseになる）
				if (vel_.y < -12.0f)
				{
					isPut_ = false;

					// 壊れた音を流す
					Novice::PlayAudio(shBreak, 0, 0.3f);

					// ゲームオーバーになる（ゲームオーバーフラグがtrueになる）
					Scene::isGameOver_ = true;

					// IDを消す
					id_ = 0;
					--countID;
				}
				else
				{
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
				}

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


					// 早く落下すると、壊れる（配置フラグがfalseになる）
					if (vel_.y < -12.0f)
					{
						isPut_ = false;

						// 壊れた音を流す
						Novice::PlayAudio(shBreak, 0, 0.3f);

						// ゲームオーバーになる（ゲームオーバーフラグがtrueになる）
						Scene::isGameOver_ = true;

						// IDを消す
						id_ = 0;
						--countID;
					} else
					{
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
					}

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


					// 早く落下すると、壊れる（配置フラグがfalseになる）
					if (vel_.y < -12.0f)
					{
						isPut_ = false;

						// 壊れた音を流す
						Novice::PlayAudio(shBreak, 0, 0.3f);

						// ゲームオーバーになる（ゲームオーバーフラグがtrueになる）
						Scene::isGameOver_ = true;

						// IDを消す
						id_ = 0;
						--countID;
					} 
					else
					{
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
					}

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


					// 早く落下すると、壊れる（配置フラグがfalseになる）
					if (vel_.y < -12.0f)
					{
						isPut_ = false;

						// 壊れた音を流す
						Novice::PlayAudio(shBreak, 0, 0.3f);

						// ゲームオーバーになる（ゲームオーバーフラグがtrueになる）
						Scene::isGameOver_ = true;

						// IDを消す
						id_ = 0;
						--countID;
					} else
					{
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
					}

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


					// 早く落下すると、壊れる（配置フラグがfalseになる）
					if (vel_.y < -12.0f)
					{
						isPut_ = false;

						// 壊れた音を流す
						Novice::PlayAudio(shBreak, 0, 0.3f);

						// ゲームオーバーになる（ゲームオーバーフラグがtrueになる）
						Scene::isGameOver_ = true;

						// IDを消す
						id_ = 0;
						--countID;
					} else
					{
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
					}

					vel_.y = 0.0f;
				}
			}
		}
	}

	// 動かす
	shape_.translate.y += vel_.y;
	LocalToScreen();


	/*   ゴール   */

	// ゴールについたら消える（配置フラグがfalseになる）
	if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_GOAL ||
		Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_GOAL ||
		Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GOAL ||
		Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GOAL)
	{
		isPut_ = false;

		// IDを消す
		id_ = 0;
		--countID;

		// ゴールの効果音
		int shGet = Novice::LoadAudio("./Resources/Sounds/Se/treasureGet.mp3");
		Novice::PlayAudio(shGet, 0, 0.3f);

		// 残りの宝の数が減る
		Map::treasureNum--;
	}


	/*   パーティクル   */

	for (int i = 0; i < kParticleCold; i++)
	{
		if (cold_[i]->isEmission_ == false)
		{
			cold_[i]->Emission({ shape_.translate.x - shape_.scale.x - 8.0f + static_cast<float>(rand() % (static_cast<int>(shape_.scale.x) * 2 + 16)) ,
					shape_.translate.y - shape_.scale.y + static_cast<float>(rand() % (static_cast<int>(shape_.scale.y + 4.0f) * 2)) });

			break;
		}
	}

	for (int i = 0; i < kParticleCold; i++)
	{
		if (cold_[i]->isEmission_ == false)
		{
			cold_[i]->Emission({ shape_.translate.x - shape_.scale.x - 20.0f + static_cast<float>(rand() % (static_cast<int>(shape_.scale.x) * 2 + 40)) ,
					shape_.translate.y - shape_.scale.y + static_cast<float>(rand() % (static_cast<int>(shape_.scale.y / 2))) });

			break;
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

							// 壊れた効果音
							int shBreak = Novice::LoadAudio("./Resources/Sounds/Se/treasureBreak.mp3");

							// 押し込み処理
							shape_.translate.y = block->shape_.translate.y + block->shape_.scale.y + shape_.scale.y;
							LocalToScreen();

							jump_.isJump = false;
							jump_.fallingVel = 0.0f;

							// 乗っかる（乗っかりフラグがtrueになる）
							isRide_ = true;

							// 乗っかかる（乗っかかりフラグがtrueになる）
							block->isUnderRide_ = true;


							// 早く落下すると、壊れる（配置フラグがfalseになる）
							if (vel_.y < -12.0f)
							{
								// 先に移動速度が消える
								vel_.y = 0.0f;

								// 着地したブロックがクッションだと、壊れない
								if (block->type_ != BLOCK_CUSHION)
								{
									isPut_ = false;

									// 壊れた音を流す
									Novice::PlayAudio(shBreak, 0, 0.3f);

									// ゲームオーバーになる（ゲームオーバーフラグがtrueになる）
									Scene::isGameOver_ = true;

									// IDを消す
									id_ = 0;
									--countID;
								}
								else
								{
									// 壊れる速度でクッションに着地したら、クッションが壊れる（配置フラグがfalesになる）
									block->isPut_ = false;

									// IDを消す
									block->id_ = 0;
									--countID;


									// 宝が少し飛び跳ねる（ジャンプフラグがfalseになるs）
									jump_.isJump = true;
									jump_.fallingVel = -0.5f;

									// 上方向に上昇する
									vel_.y = 6.0f;
								}
							}
							else
							{
								// 早く落下したら、効果音を鳴らす
								if (vel_.y < -4.0f)
								{
									Novice::PlayAudio(shLanding, 0, 0.3f);

									// 着地したブロックがクッションだと、クッションが壊れる
									if (block->type_ == BLOCK_CUSHION)
									{
										// 壊れる速度でクッションに着地したら、クッションが壊れる（配置フラグがfalesになる）
										block->isPut_ = false;

										// IDを消す
										block->id_ = 0;
										--countID;
									}
									

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

								// 後に移動速度が消える
								vel_.y = 0.0f;
							}
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
void Treasure::Draw()
{
	// 配置されていない（配置フラグがfalseである）ときは、描画し ない
	if (isPut_ == false)
	{
		return;
	}

	// 白い画像
	/*int ghWhite = Novice::LoadTexture("./NoviceResources/white1x1.png");*/

	int ghIceBlock = Novice::LoadTexture("./Resources/Images/Map/Treasure.png");

	Novice::DrawQuad
	(
		static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
		static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
		static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
		static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
		0, 0, 48, 48, ghIceBlock, 0xFFFFFFFF
	);
}