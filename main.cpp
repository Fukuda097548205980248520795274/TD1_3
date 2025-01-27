#include <Novice.h>
#include "Constant.h"
#include "Enumeration.h"
#include "./Class/Scene/Scene.h"
#include "./Class/Map/Map.h"
#include "./Class/DrawMap/DrawMap.h"
#include "./Class/Object/Player/Player.h"
#include "./Class/Object/CarryBlock/Plastic/Plastic.h"
#include "./Class/Object/CarryBlock/Cushion/Cushion.h"
#include "./Class/Object/CarryBlock/Treasure/Treasure.h"
#include "./Class/Object/CarryBlock/Bomb/Bomb.h"
#include "./Class/Object/Enemy/Ghost/Ghost.h"
#include "./Class/Object/Particle/Snow/Snow.h"
#include "./Class/Object/Particle/Water/Water.h"
#include "./Class/Object/Particle/Debris/Debris.h"


const char kWindowTitle[] = "LC1C_20_フクダソウワ_ゆきどけ～";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kScreenWidth, kScreenHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };


	/*---------------
		変数を作る
	---------------*/

	/*   ゲームシステム   */

	// マップ
	Map::LoadFile("./TextFiles/Stage/stage1.csv");

	// プレイヤー
	Player* player = new Player();

	// 敵
	Enemy* enemy[kEnemyNum];
	for (int i = 0; i < kEnemyNum; i++)
	{
		// ゴースト
		if (i < 8)
		{
			enemy[i] = new Ghost();
		}
	}

	// ブロック
	CarryBlock* block[kBlockNum];
	for (int i = 0; i < kBlockNum; i++)
	{
		// プラスチック
		if (i < 8)
		{
			block[i] = new Plastic();
		}
		else if (i < 16)
		{
			// クッション
			block[i] = new Cushion();
		}
		else if (i < 24)
		{
			// 宝
			block[i] = new Treasure();
		}
		else if (i < 32)
		{
			// 凍った敵
			block[i] = new Bomb();
		}
	}


	// 雪
	Snow* snow[kSnowNum];
	for (int i = 0; i < kSnowNum; i++)
	{
		snow[i] = new Snow();
	}

	// 水
	Water* water[kParticleWater];
	for (int i = 0; i < kParticleWater; i++)
	{
		water[i] = new Water();
	}

	// 欠片
	Debris* debris[kParticleDebris];
	for (int i = 0; i < kParticleDebris; i++)
	{
		debris[i] = new Debris();
	}


	/*  　デバック表示　  */
	int isActive = true;


	/*-----------
		BGM
	----------*/

	// 雪空
	int shYukisora = Novice::LoadAudio("./Resources/Sounds/Bgm/Yukisora.mp3");
	int phYukisora = -1;

	// 雪の記憶
	int shYukinokioku = Novice::LoadAudio("./Resources/Sounds/Bgm/Yukinokioku.mp3");
	int phYukinokioku = -1;

	// 雪風
	int shYukikaze = Novice::LoadAudio("./Resources/Sounds/Bgm/Yukikaze.mp3");
	int phYukikaze = -1;

	// 雪の葬列
	int shYukinosouretu = Novice::LoadAudio("./Resources/Sounds/Bgm/Yukinosouretu.mp3");
	int phYukinosouretu = -1;

	// 雪小屋
	int shYukigoya = Novice::LoadAudio("./Resources/Sounds/Bgm/Yukigoya.mp3");
	int phYukigoya = -1;

	// A Dream of Cat
	int shADreamOfCat = Novice::LoadAudio("./Resources/Sounds/Bgm/A_Dream_of_a_Cat.mp3");
	int phADreamOfCat = -1;

	// in the stillness of twilight
	int shInTheStillnessOfTwilight = Novice::LoadAudio("./Resources/Sounds/Bgm/in_the_stillness_of_twilight.mp3");
	int phInTheStillnessOfTwilight = -1;

	// 軽井沢の鳥の鳴き声
	int shBard = Novice::LoadAudio("./Resources/Sounds/Bgm/bard1.mp3");
	int phBard = -1;


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0)
	{
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);


		///
		/// ↓更新処理ここから
		/// 

		if (!Novice::IsPlayingAudio(phBard) || phBard == -1)
		{
			if (rand() % 2 == 0)
			{
				shBard = Novice::LoadAudio("./Resources/Sounds/Bgm/bard1.mp3");
			}
			else
			{
				shBard = Novice::LoadAudio("./Resources/Sounds/Bgm/bard2.mp3");
			}

			phBard = Novice::PlayAudio(shBard, 0, 0.3f);
		}


		/*------------------------
			パーティクルを動かす
		------------------------*/

		if (Scene::isClear_ == false && Scene::isGameOver_ == false)
		{
			/*   放出   */

			// 雪
			if (Snow::coolTime <= 0)
			{
				for (int i = 0; i < kSnowNum; i++)
				{
					if (snow[i]->isEmission_ == false)
					{
						Snow::coolTime = 5;

						snow[i]->Emission({ static_cast<float>(rand() % (kScreenWidth + 400)) , static_cast<float>(kScreenHeight + 100) });

						break;
					}
				}
			}

			// 水
			if (Water::coolTime <= 0)
			{
				for (int row = 0; row < kMapRow; row++)
				{
					for (int column = 0; column < kMapColumn; column++)
					{
						if (Map::map_[row][column] < 0)
						{
							for (int i = 0; i < kParticleWater; i++)
							{
								if (water[i]->isEmission_ == false)
								{
									Water::coolTime = 40;

									water[i]->Emission({ static_cast<float>(column * kTileSize + rand() % kTileSize) ,
										static_cast<float>(kScreenHeight - (row * kTileSize + rand() % kTileSize)) });

									break;
								}
							}
						}
					}
				}
			}

			// 欠片
			for (int row = 0; row < kMapRow; row++)
			{
				for (int column = 0; column < kMapColumn; column++)
				{
					if (Map::map_[row][column] == -1)
					{
						for (int i = 0; i < 8; i++)
						{
							for (int j = 0; j < kParticleDebris; j++)
							{
								if (debris[j]->isEmission_ == false)
								{
									debris[j]->Emission({ static_cast<float>(column * kTileSize + kTileSize / 2) ,
										static_cast<float>(kScreenHeight - (row * kTileSize + kTileSize / 2)) });

									break;
								}
							}
						}
					}
				}
			}


			/*   画面   */

			// クールタイム
			if (Snow::coolTime > 0)
			{
				Snow::coolTime--;
			}

			// 雪
			for (int i = 0; i < kSnowNum; i++)
			{
				snow[i]->Move();
			}


			// クールタイム
			if (Water::coolTime > 0)
			{
				Water::coolTime--;
			}

			// 水
			for (int i = 0; i < kParticleWater; i++)
			{
				water[i]->Move();
			}

			// 欠片
			for (int i = 0; i < kParticleDebris; i++)
			{
				debris[i]->Move();
			}


			/*   プレイヤー   */

			// クールタイムを進める
			if (SnowCarry::coolTime > 0)
			{
				SnowCarry::coolTime--;
			}

			// クールタイムを進める
			if (SnowShining::coolTime > 0)
			{
				SnowShining::coolTime--;
			}

			// 動かす
			for (int i = 0; i < kParticleSnowCarry; i++)
			{
				player->snowCarry[i]->Move();
			}

			// 動かす
			for (int i = 0; i < kParticleLanding; i++)
			{
				player->landing[i]->Move();
			}

			// 動かす
			for (int i = 0; i < kParticleSnowShining; i++)
			{
				player->snowShining[i]->Move();
			}


			/*   ブロック   */

			// クールタイム
			if (Cold::coolTime > 0)
			{
				Cold::coolTime--;
			}

			for (int i = 0; i < kBlockNum; i++)
			{
				// 着地したときの粒子
				for (int j = 0; j < kParticleLanding; j++)
				{
					block[i]->landing_[j]->Move();
				}

				// 冷気
				for (int j = 0; j < kParticleCold; j++)
				{
					block[i]->cold_[j]->Move();
				}
			}


			/*   敵   */

			// クールタイム
			if (Hinoko::coolTime > 0)
			{
				Hinoko::coolTime--;
			}

			// 動かす
			for (int i = 0; i < kEnemyNum; i++)
			{
				for (int j = 0; j < kParticleHinoko; j++)
				{
					enemy[i]->hinoko_[j]->Move();
				}

				for (int j = 0; j < kParticleVapor; j++)
				{
					enemy[i]->vapor[j]->Move();
				}
			}
		}


		// クラスで画面切り替え
		Scene::Switch(keys, preKeys);

		// 画面切り替え
		switch (Scene::sceneNo_)
		{
		case SCENE_START:
#pragma region シーン:タイトル

			// スタート画面

			Map::LoadFile("./TextFiles/Scene/StageSelect.csv");

			// ブロックや敵を配置する
			for (int row = 0; row < kMapRow; row++)
			{
				for (int column = 0; column < kMapColumn; column++)
				{
					switch (Map::map_[row][column])
					{
					case TILE_PLAYER:
						// プレイヤー

						player->Puttting(column, row);

						// タイルを消す
						Map::map_[row][column] = TILE_NOTHING;

						break;


					case TILE_PLASTIC:
						// プラスチック

						for (int i = 0; i < kBlockNum; i++)
						{
							if (i < 8)
							{
								if (block[i]->id_ == 0)
								{
									block[i]->Putting(column, row);

									break;
								}
							}
						}

						// タイルを消す
						Map::map_[row][column] = TILE_NOTHING;

						break;

					case TILE_CUSHION:
						// クッション

						for (int i = 0; i < kBlockNum; i++)
						{
							if (i >= 8 && i < 16)
							{
								if (block[i]->id_ == 0)
								{
									block[i]->Putting(column, row);

									break;
								}
							}
						}

						// タイルを消す
						Map::map_[row][column] = TILE_NOTHING;

						break;

					case TILE_TREASURE:
						// 宝

						for (int i = 0; i < kBlockNum; i++)
						{
							if (i >= 16 && i < 24)
							{
								if (block[i]->id_ == 0)
								{
									block[i]->Putting(column, row);

									break;
								}
							}
						}

						// タイルを消す
						Map::map_[row][column] = TILE_NOTHING;

						// 宝の数をカウントする
						Map::treasureNum++;

						break;

					case TILE_BOMB:
						// 爆弾

						for (int i = 0; i < kBlockNum; i++)
						{
							if (i >= 24 && i < 32)
							{
								if (block[i]->id_ == 0)
								{
									block[i]->Putting(column, row);

									break;
								}
							}
						}

						// タイルを消す
						Map::map_[row][column] = TILE_NOTHING;

						break;

					case TILE_GHOST:
						// 幽霊

						for (int i = 0; i < kEnemyNum; i++)
						{
							if (i < 8)
							{
								if (enemy[i]->id_ == 0)
								{
									enemy[i]->Arrival(column, row);

									break;
								}
							}
						}

						// タイルを消す
						Map::map_[row][column] = TILE_NOTHING;

						break;
					}
				}
			}

			break;
#pragma endregion
		case SCENE_STAGE:
#pragma region シーン:ステージ
			// ステージセレクト画面

			player->Operation(keys, preKeys);

			// プレイヤーがブロックに乗る
			for (int i = 0; i < kBlockNum; i++)
			{
				player->BlockLanding(block[i]);
			}

			/*   デバック表示させる   */
			if (!preKeys[DIK_4] && keys[DIK_4])
			{
				if (!isActive)
				{
					isActive = true;
				}
				else
				{
					isActive = false;
				}

			}


			// 配置準備ができたら（配置準備フラグがtrueだったら）、ブロックを配置する
			if (Scene::isPutPreparation_)
			{
				// クリア、ゲームオーバーを初期化する
				Scene::isClear_ = false;
				Scene::isGameOver_ = false;

				// ブロックや敵を配置する
				for (int row = 0; row < kMapRow; row++)
				{
					for (int column = 0; column < kMapColumn; column++)
					{
						switch (Map::map_[row][column])
						{
						case TILE_PLAYER:
							// プレイヤー

							player->Puttting(column, row);

							// タイルを消す
							Map::map_[row][column] = TILE_NOTHING;

							break;


						case TILE_PLASTIC:
							// プラスチック

							for (int i = 0; i < kBlockNum; i++)
							{
								if (i < 8)
								{
									if (block[i]->id_ == 0)
									{
										block[i]->Putting(column, row);

										break;
									}
								}
							}

							// タイルを消す
							Map::map_[row][column] = TILE_NOTHING;

							break;

						case TILE_CUSHION:
							// クッション

							for (int i = 0; i < kBlockNum; i++)
							{
								if (i >= 8 && i < 16)
								{
									if (block[i]->id_ == 0)
									{
										block[i]->Putting(column, row);

										break;
									}
								}
							}

							// タイルを消す
							Map::map_[row][column] = TILE_NOTHING;

							break;

						case TILE_TREASURE:
							// 宝

							for (int i = 0; i < kBlockNum; i++)
							{
								if (i >= 16 && i < 24)
								{
									if (block[i]->id_ == 0)
									{
										block[i]->Putting(column, row);

										break;
									}
								}
							}

							// タイルを消す
							Map::map_[row][column] = TILE_NOTHING;

							// 宝の数をカウントする
							Map::treasureNum++;

							break;

						case TILE_BOMB:
							// 爆弾

							for (int i = 0; i < kBlockNum; i++)
							{
								if (i >= 24 && i < 32)
								{
									if (block[i]->id_ == 0)
									{
										block[i]->Putting(column, row);

										break;
									}
								}
							}

							// タイルを消す
							Map::map_[row][column] = TILE_NOTHING;

							break;

						case TILE_GHOST:
							// 幽霊

							for (int i = 0; i < kEnemyNum; i++)
							{
								if (i < 8)
								{
									if (enemy[i]->id_ == 0)
									{
										enemy[i]->Arrival(column, row);

										break;
									}
								}
							}

							// タイルを消す
							Map::map_[row][column] = TILE_NOTHING;

							break;
						}
					}
				}

				// ゲーム画面に切り替える
				Scene::sceneNo_ = SCENE_GAME;

				// 配置が完了した（配置準備フラグがfalseになる）
				Scene::isPutPreparation_ = false;
			}

			break;

#pragma endregion
		case SCENE_GAME:
#pragma region シーン:ゲーム
			// ゲーム画面

			// クリア、ゲームオーバーになるまで（クリア、ゲームオーバーフラグがfalseのときは）、操作できる
			if (Scene::isClear_ == false && Scene::isGameOver_ == false)
			{

				// エリアごとに曲を変える
				switch (Scene::areaNo_)
				{
				case AREA_STAR_LAND:
					// スター島

					if (!Novice::IsPlayingAudio(phYukisora) || phYukisora == -1)
					{
						phYukisora = Novice::PlayAudio(shYukisora, 0, 0.1f);
					}

					break;

				case AREA_REI_LAND:
					// レイ島

					if (!Novice::IsPlayingAudio(phYukinokioku) || phYukinokioku == -1)
					{
						phYukinokioku = Novice::PlayAudio(shYukinokioku, 0, 0.1f);
					}

					break;

				case AREA_TIKUTAKU_LAND:
					// チクタク島

					if (!Novice::IsPlayingAudio(phYukinosouretu) || phYukinosouretu == -1)
					{
						phYukinosouretu = Novice::PlayAudio(shYukinosouretu, 0, 0.1f);
					}

					break;

				case AREA_HOXTU_LAND:
					// ホッ島

					if (!Novice::IsPlayingAudio(phYukigoya) || phYukigoya == -1)
					{
						phYukigoya = Novice::PlayAudio(shYukigoya, 0, 0.1f);
					}

					break;

				case AREA_RAPIXTU_LAND:
					// ラピッ島

					if (!Novice::IsPlayingAudio(phYukikaze) || phYukikaze == -1)
					{
						phYukikaze = Novice::PlayAudio(shYukikaze, 0, 0.1f);
					}

					break;
				}

				// 溶かす
				Map::Rotten();


				/*   プレイヤー   */

				// 操作する
				player->Operation(keys, preKeys);

				// プレイヤーがブロックに乗る
				for (int i = 0; i < kBlockNum; i++)
				{
					player->BlockLanding(block[i]);
				}

				/*   デバック表示させる   */
				if (!preKeys[DIK_4] && keys[DIK_4])
				{
					if (!isActive)
					{
						isActive = true;
					}
					else
					{
						isActive = false;
					}

				}


				/*   敵   */

				// 敵を動かす
				for (int i = 0; i < kEnemyNum; i++)
				{
					enemy[i]->Move();
				}

				// ブロックに乗る
				for (int i = 0; i < kEnemyNum; i++)
				{
					for (int j = 0; j < kBlockNum; j++)
					{
						enemy[i]->BlockLanding(block[j]);
					}
				}


				/*   ブロック   */

				// ブロックを動かす
				for (int i = 0; i < kBlockNum; i++)
				{
					block[i]->Move();
				}

				// ブロックの当たり判定
				for (int i = 0; i < kBlockNum; i++)
				{
					for (int j = 0; j < kBlockNum; j++)
					{
						block[i]->BlockLanding(block[j]);
					}
				}

				// ブロックを運ぶための当たり判定
				for (int i = 0; i < kBlockNum; i++)
				{
					for (int j = 0; j < kBlockNum; j++)
					{
						player->Carry(keys, preKeys, block[i], block[j]);

					}
				}

				// 乗っかり、乗っかかりフラグをfalseに戻す
				for (int i = 0; i < kBlockNum; i++)
				{
					block[i]->isRide_ = false;
					block[i]->isUnderRide_ = false;
				}


				/*   当たり判定   */

				for (int i = 0; i < kEnemyNum; i++)
				{
					player->Hit(enemy[i]);
				}


				/*   リセット   */

				// Rキーでリセットする
				if (!preKeys[DIK_R] && keys[DIK_R])
				{
					// クリアを初期化する（クリアフラグをfalseにする）
					Scene::isClear_ = false;
					Scene::isGameOver_ = false;

					// ブロックを初期化する
					for (int i = 0; i < kBlockNum; i++)
					{
						block[i]->InitialValue();
					}

					// 敵を初期化する
					for (int i = 0; i < kEnemyNum; i++)
					{
						enemy[i]->InitialValue();
					}

					// パーティクルを初期化する
					for (int i = 0; i < kParticleWater; i++)
					{
						water[i]->isEmission_ = false;
					}

					// 宝の数を初期化する
					Map::treasureNum = 0;

					switch (Scene::selectStage_)
					{
					case 1:

						switch (Scene::areaNo_)
						{
						case AREA_STAR_LAND:

							Map::LoadFile("./TextFiles/Stage/area1/stage1.csv");

							break;

						case AREA_REI_LAND:

							Map::LoadFile("./TextFiles/Stage/area2/stage1.csv");

							break;

						case AREA_TIKUTAKU_LAND:

							Map::LoadFile("./TextFiles/Stage/area3/stage1.csv");

							break;

						case AREA_HOXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area4/stage1.csv");

							break;

						case AREA_RAPIXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area5/stage1.csv");

							break;
						}

						break;

					case 2:

						switch (Scene::areaNo_)
						{
						case AREA_STAR_LAND:

							Map::LoadFile("./TextFiles/Stage/area1/stage2.csv");

							break;

						case AREA_REI_LAND:

							Map::LoadFile("./TextFiles/Stage/area2/stage2.csv");

							break;

						case AREA_TIKUTAKU_LAND:

							Map::LoadFile("./TextFiles/Stage/area3/stage2.csv");

							break;

						case AREA_HOXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area4/stage2.csv");

							break;

						case AREA_RAPIXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area5/stage2.csv");

							break;
						}

						break;

					case 3:

						switch (Scene::areaNo_)
						{
						case AREA_STAR_LAND:

							Map::LoadFile("./TextFiles/Stage/area1/stage3.csv");

							break;

						case AREA_REI_LAND:

							Map::LoadFile("./TextFiles/Stage/area2/stage3.csv");

							break;

						case AREA_TIKUTAKU_LAND:

							Map::LoadFile("./TextFiles/Stage/area3/stage3.csv");

							break;

						case AREA_HOXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area4/stage3.csv");

							break;

						case AREA_RAPIXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area5/stage3.csv");

							break;
						}

						break;

					case 4:

						switch (Scene::areaNo_)
						{
						case AREA_STAR_LAND:

							Map::LoadFile("./TextFiles/Stage/area1/stage4.csv");

							break;

						case AREA_REI_LAND:

							Map::LoadFile("./TextFiles/Stage/area2/stage4.csv");

							break;

						case AREA_TIKUTAKU_LAND:

							Map::LoadFile("./TextFiles/Stage/area3/stage4.csv");

							break;

						case AREA_HOXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area4/stage4.csv");

							break;

						case AREA_RAPIXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area5/stage4.csv");

							break;
						}

						break;

					case 5:

						switch (Scene::areaNo_)
						{
						case AREA_STAR_LAND:

							Map::LoadFile("./TextFiles/Stage/area1/stage5.csv");

							break;

						case AREA_REI_LAND:

							Map::LoadFile("./TextFiles/Stage/area2/stage5.csv");

							break;

						case AREA_TIKUTAKU_LAND:

							Map::LoadFile("./TextFiles/Stage/area3/stage5.csv");

							break;

						case AREA_HOXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area4/stage5.csv");

							break;

						case AREA_RAPIXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area5/stage5.csv");

							break;
						}

						break;

					case 6:

						switch (Scene::areaNo_)
						{
						case AREA_STAR_LAND:

							Map::LoadFile("./TextFiles/Stage/area1/stage6.csv");

							break;

						case AREA_REI_LAND:

							Map::LoadFile("./TextFiles/Stage/area2/stage6.csv");

							break;

						case AREA_TIKUTAKU_LAND:

							Map::LoadFile("./TextFiles/Stage/area3/stage6.csv");

							break;

						case AREA_HOXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area4/stage6.csv");

							break;

						case AREA_RAPIXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area5/stage6.csv");

							break;
						}

						break;

					case 7:

						switch (Scene::areaNo_)
						{
						case AREA_STAR_LAND:

							Map::LoadFile("./TextFiles/Stage/area1/stage7.csv");

							break;

						case AREA_REI_LAND:

							Map::LoadFile("./TextFiles/Stage/area2/stage7.csv");

							break;

						case AREA_TIKUTAKU_LAND:

							Map::LoadFile("./TextFiles/Stage/area3/stage7.csv");

							break;

						case AREA_HOXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area4/stage7.csv");

							break;

						case AREA_RAPIXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area5/stage7.csv");

							break;
						}

						break;

					case 8:

						switch (Scene::areaNo_)
						{
						case AREA_STAR_LAND:

							Map::LoadFile("./TextFiles/Stage/area1/stage8.csv");

							break;

						case AREA_REI_LAND:

							Map::LoadFile("./TextFiles/Stage/area2/stage8.csv");

							break;

						case AREA_TIKUTAKU_LAND:

							Map::LoadFile("./TextFiles/Stage/area3/stage8.csv");

							break;

						case AREA_HOXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area4/stage8.csv");

							break;

						case AREA_RAPIXTU_LAND:

							Map::LoadFile("./TextFiles/Stage/area5/stage8.csv");

							break;
						}

						break;
					}

					// ブロックや敵を配置する
					for (int row = 0; row < kMapRow; row++)
					{
						for (int column = 0; column < kMapColumn; column++)
						{
							switch (Map::map_[row][column])
							{
							case TILE_PLAYER:
								// プレイヤー

								player->Puttting(column, row);

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;


							case TILE_PLASTIC:
								// プラスチック

								for (int i = 0; i < kBlockNum; i++)
								{
									if (i < 8)
									{
										if (block[i]->id_ == 0)
										{
											block[i]->Putting(column, row);

											break;
										}
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_CUSHION:
								// クッション

								for (int i = 0; i < kBlockNum; i++)
								{
									if (i >= 8 && i < 16)
									{
										if (block[i]->id_ == 0)
										{
											block[i]->Putting(column, row);

											break;
										}
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_TREASURE:
								// 宝

								for (int i = 0; i < kBlockNum; i++)
								{
									if (i >= 16 && i < 24)
									{
										if (block[i]->id_ == 0)
										{
											block[i]->Putting(column, row);

											break;
										}
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								// 宝の数をカウントする
								Map::treasureNum++;

								break;

							case TILE_BOMB:
								// 爆弾

								for (int i = 0; i < kBlockNum; i++)
								{
									if (i >= 24 && i < 32)
									{
										if (block[i]->id_ == 0)
										{
											block[i]->Putting(column, row);

											break;
										}
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_GHOST:
								// 幽霊

								for (int i = 0; i < kEnemyNum; i++)
								{
									if (i < 8)
									{
										if (enemy[i]->id_ == 0)
										{
											enemy[i]->Arrival(column, row);

											break;
										}
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;
							}
						}
					}
				}


				/*   敗北条件   */

				// プレイヤーがやられたら（復活フラグがfalseになったら）ゲームオーバー
				if (player->respawn_.isRespawn == false)
				{
					Scene::isGameOver_ = true;
				}


				/*   クリア条件   */

				// 残りの宝がなくなったらクリア（クリアフラグがtrueになる）
				if (Map::treasureNum <= 0)
				{
					Scene::isClear_ = true;
				}
			}
			else
			{
				// Bgmを止める
				Novice::StopAudio(phYukisora);
				Novice::StopAudio(phYukinokioku);
				Novice::StopAudio(phYukikaze);
				Novice::StopAudio(phYukinosouretu);
				Novice::StopAudio(phYukigoya);

				// 宝の数を初期化する
				Map::treasureNum = 0;

				// クリアした（クリアフラグがtrueである）ときのBGM
				if (Scene::isClear_)
				{
					if (!Novice::IsPlayingAudio(phADreamOfCat) || phADreamOfCat == -1)
					{
						phADreamOfCat = Novice::PlayAudio(shADreamOfCat, 0, 0.1f);
					}
				}
				else if (Scene::isGameOver_)
				{
					// ゲームオーバーの（ゲームオーバーフラグがtrueである）ときのBGM

					if (!Novice::IsPlayingAudio(phInTheStillnessOfTwilight) || phInTheStillnessOfTwilight == -1)
					{
						phInTheStillnessOfTwilight = Novice::PlayAudio(shInTheStillnessOfTwilight, 0, 0.1f);
					}
				}

				// スペースキーで、ステージセレクトに戻る
				if (!preKeys[DIK_SPACE] && keys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
				{
					Scene::sceneNo_ = SCENE_STAGE;


					// Bgmを止める
					Novice::StopAudio(phADreamOfCat);
					Novice::StopAudio(phInTheStillnessOfTwilight);

					// ブロックを初期化する
					for (int i = 0; i < kBlockNum; i++)
					{
						block[i]->InitialValue();
					}

					// 敵を初期化する
					for (int i = 0; i < kEnemyNum; i++)
					{
						enemy[i]->InitialValue();
					}

					// パーティクルを初期化する
					for (int i = 0; i < kParticleWater; i++)
					{
						water[i]->isEmission_ = false;
					}

					// クリア、ゲームオーバーを初期化する
					Scene::isClear_ = false;
					Scene::isGameOver_ = false;

					// ステージセレクトのマップ
					Map::LoadFile("./TextFiles/Scene/StageSelect.csv");

					// ブロックや敵を配置する
					for (int row = 0; row < kMapRow; row++)
					{
						for (int column = 0; column < kMapColumn; column++)
						{
							switch (Map::map_[row][column])
							{
							case TILE_PLAYER:
								// プレイヤー

								player->Puttting(column, row);

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;


							case TILE_PLASTIC:
								// プラスチック

								for (int i = 0; i < kBlockNum; i++)
								{
									if (i < 8)
									{
										if (block[i]->id_ == 0)
										{
											block[i]->Putting(column, row);

											break;
										}
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_CUSHION:
								// クッション

								for (int i = 0; i < kBlockNum; i++)
								{
									if (i >= 8 && i < 16)
									{
										if (block[i]->id_ == 0)
										{
											block[i]->Putting(column, row);

											break;
										}
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_TREASURE:
								// 宝

								for (int i = 0; i < kBlockNum; i++)
								{
									if (i >= 16 && i < 24)
									{
										if (block[i]->id_ == 0)
										{
											block[i]->Putting(column, row);

											break;
										}
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								// 宝の数をカウントする
								Map::treasureNum++;

								break;

							case TILE_ICE_GHOST:
								// 凍った幽霊

								for (int i = 0; i < kBlockNum; i++)
								{
									if (i >= 24 && i < 32)
									{
										if (block[i]->id_ == 0)
										{
											block[i]->Putting(column, row);

											break;
										}
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_GHOST:
								// 幽霊

								for (int i = 0; i < kEnemyNum; i++)
								{
									if (i < 8)
									{
										if (enemy[i]->id_ == 0)
										{
											enemy[i]->Arrival(column, row);

											break;
										}
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;
							}
						}
					}
				}
			}


			break;
#pragma endregion
		}

		///
		/// ↑更新処理ここまで
		/// 

		///
		/// ↓描画処理ここから
		///

		/*-------------------------
			パーティクルを描画する
		-------------------------*/



		// 画面切り替え
		switch (Scene::sceneNo_)
		{
		case SCENE_START:
			// スタート画面

			//デバック表示
			Novice::ScreenPrintf(0, 0, "TITLE");

			break;

		case SCENE_STAGE:
			// ステージセレクト画面


			Map::Draw();

			// ブロック
			for (int i = 0; i < kBlockNum; i++)
			{
				block[i]->Draw();
			}

			// 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				enemy[i]->Draw();
			}

			// プレイヤー
			player->Draw();




			//デバック表示
			Novice::ScreenPrintf(8, 8, "STAGE_SELECT");

			Novice::ScreenPrintf(8, 28, "Debug display:4");

			if (isActive)
			{
				Novice::ScreenPrintf(8, 48, "device:keyboard | controller");
				Novice::ScreenPrintf(8, 68, "Move:  AD       |  leftStick");
				Novice::ScreenPrintf(8, 88, "JUMP:  J        |      A    ");
				Novice::ScreenPrintf(8, 108, "Ladder: W       | leftStick(up)");
				Novice::ScreenPrintf(8, 128, "Interact:SPASE  | leftStick(up)");
			}

			break;

		case SCENE_GAME:
			// ゲーム画面

			// マップ
			Map::Draw();

			// ブロック
			for (int i = 0; i < kBlockNum; i++)
			{
				block[i]->Draw();
			}


			/*   敵   */

			// 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				enemy[i]->Draw();
			}


			/*   プレイヤー   */

			// プレイヤー
			player->Draw();


			/*   クリア画面   */
			if (Scene::isClear_)
			{
				Novice::ScreenPrintf(600, 350, "GAMECLEAR"); 
				Novice::ScreenPrintf(580, 390, "Spase to push");
			}

			/*   ゲームオーバー画面   */
			if (Scene::isGameOver_)
			{
				Novice::ScreenPrintf(600, 350, "GAMEOVER");
				Novice::ScreenPrintf(580, 390, "Spase to push");
			}

			//デバック表示
			Novice::ScreenPrintf(8, 8, "GAME");

			Novice::ScreenPrintf(500, 8, "Remaining treasure:%d", Map::treasureNum);

			Novice::ScreenPrintf(8, 28, "Debug display:4");

			if (isActive)
			{
				Novice::ScreenPrintf(8, 48, "device:keyboard   | Controller");
				Novice::ScreenPrintf(8, 68, "Move:  AD         | LeftStick");
				Novice::ScreenPrintf(8, 88, "JUMP:  J          |      A    ");
				Novice::ScreenPrintf(8, 108, "Ladder: W         | LeftStick(up)");
				Novice::ScreenPrintf(8, 128, "CarryBlock:SPASE  |     R2    ");
			}


			break;

		}


		// 雪
		for (int i = 0; i < kSnowNum; i++)
		{
			snow[i]->Draw();
		}

		// 水
		for (int i = 0; i < kParticleWater; i++)
		{
			water[i]->Draw();
		}

		// 欠片
		for (int i = 0; i < kParticleDebris; i++)
		{
			debris[i]->Draw();
		}


		/*   プレイヤー   */

		for (int i = 0; i < kParticleSnowCarry; i++)
		{
			player->snowCarry[i]->Draw();
		}

		for (int i = 0; i < kParticleLanding; i++)
		{
			player->landing[i]->Draw();
		}

		for (int i = 0; i < kParticleSnowShining; i++)
		{
			player->snowShining[i]->Draw();
		}


		/*   ブロック   */

		for (int i = 0; i < kBlockNum; i++)
		{
			for (int j = 0; j < kParticleLanding; j++)
			{
				block[i]->landing_[j]->Draw();
			}

			// 冷気
			for (int j = 0; j < kParticleCold; j++)
			{
				block[i]->cold_[j]->Draw();
			}
		}


		/*   敵   */

		for (int i = 0; i < kEnemyNum; i++)
		{
			// 火の粉
			for (int j = 0; j < kParticleHinoko; j++)
			{
				enemy[i]->hinoko_[j]->Draw();
			}

			// 蒸気
			for (int j = 0; j < kParticleVapor; j++)
			{
				enemy[i]->vapor[j]->Draw();
			}
		}


		///
		/// ↑描画処理ここまで
		///


		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0)
		{
			break;
		}
	}



	/*--------------------------
		インスタンスを削除する
	--------------------------*/

	// プレイヤー
	delete player;

	// ブロック
	for (int i = 0; i < kBlockNum; i++)
	{
		delete block[i];
	}

	// 敵
	for (int i = 0; i < kEnemyNum; i++)
	{
		delete enemy[i];
	}

	// 雪
	for (int i = 0; i < kSnowNum; i++)
	{
		delete snow[i];
	}

	// 水
	for (int i = 0; i < kParticleWater; i++)
	{
		delete water[i];
	}

	// 欠片
	for (int i = 0; i < kParticleDebris; i++)
	{
		delete debris[i];
	}


	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
