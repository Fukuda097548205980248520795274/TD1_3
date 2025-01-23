#include <Novice.h>
#include "Constant.h"
#include "Enumeration.h"
#include "./Class/Scene/Scene.h"
#include "./Class/Map/Map.h"
#include "./Class/DrawMap/DrawMap.h"
#include "./Class/Object/Player/Player.h"
#include "./Class/Object/Water/Water.h"
#include "./Class/Object/CarryBlock/Plastic/Plastic.h"
#include "./Class/Object/CarryBlock/Cushion/Cushion.h"
#include "./Class/Object/CarryBlock/Treasure/Treasure.h"
#include "./Class/Object/CarryBlock/IceGhost/IceGhost.h"
#include "./Class/Object/Enemy/Ghost/Ghost.h"
#include "./Class/Object/Particle/Dust/Dust.h"
#include "./Hinoko.h"
#include"./Snow.h"


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

	// マップ
	Map::LoadFile("./TextFiles/Stage/stage1.csv");

	/*   プレイヤー   */

	Player* player = new Player();


	/*   水   */

	//Water* water[kWaterNum];
	//for (int i = 0; i < kWaterNum; i++)
	//{
	//	water[i] = new Water();
	//}


	/*   雪   */
	Snow* snow[kSnowNum];
	for (int i = 0; i < kSnowNum; i++)
	{
		snow[i] = new Snow();
	}

	/*   炎   */
	Hinoko* hinoko[kHinokoNum];
	for (int i = 0; i < kHinokoNum; i++)
	{
		hinoko[i] = new Hinoko();
	}


	/*   敵   */

	Enemy* enemy[kEnemyNum];

	for (int i = 0; i < kEnemyNum; i++)
	{
		// ゴースト
		if (i < 8)
		{
			enemy[i] = new Ghost();
		}
	}


	/*   ブロック   */

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
			block[i] = new IceGhost();
		}
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


			if (Scene::selectStage_ == 1)
			{
				if (!Novice::IsPlayingAudio(phYukisora) || phYukisora == -1)
				{
					phYukisora = Novice::PlayAudio(shYukisora, 0, 0.1f);
				}
			}
			else if (Scene::selectStage_ == 2)
			{
				if (!Novice::IsPlayingAudio(phYukinokioku) || phYukinokioku == -1)
				{
					phYukinokioku = Novice::PlayAudio(shYukinokioku, 0, 0.1f);
				}
			}
			else if (Scene::selectStage_ == 3)
			{
				if (!Novice::IsPlayingAudio(phYukinosouretu) || phYukinosouretu == -1)
				{
					phYukinosouretu = Novice::PlayAudio(shYukinosouretu, 0, 0.1f);
				}
			}
			else if (Scene::selectStage_ == 4)
			{
				if (!Novice::IsPlayingAudio(phYukigoya) || phYukigoya == -1)
				{
					phYukigoya = Novice::PlayAudio(shYukigoya, 0, 0.1f);
				}
			}
			else if (Scene::selectStage_ == 5)
			{
				if (!Novice::IsPlayingAudio(phYukikaze) || phYukikaze == -1)
				{
					phYukikaze = Novice::PlayAudio(shYukikaze, 0, 0.1f);
				}
			}


			// 溶かす
			Map::Rotten();

			for (int i = 0; i < kSnowNum; i++)
			{
				if (snow[i]->isEmission_ == false)
				{
					snow[i]->Emission();

					break;
				}
			}

			
			for (int i = 0; i < kHinokoNum; i++)
			{
				for (int j = 0; j < kEnemyNum; j++)
				{
					if (enemy[j]->isArrival_)
					{
						if (!hinoko[i]->isEmission_)
						{
							hinoko[i]->Emission(enemy[j]->pos_.screen.leftTop.x + (enemy[i]->shape_.scale.x * 0.5f),
								enemy[j]->pos_.screen.leftTop.x + (enemy[i]->shape_.scale.x * 0.5f),
								0.0f, 0);
						}
					}
				}
			}

			// しずくを落とす
			//for (int row = 0; row < kMapRow; row++)
			//{
			//	for (int column = 0; column < kMapColumn; column++)
			//	{
			//		// 解けている氷
			//		if (Map::map_[row][column] < 0)
			//		{
			//			if (-Map::map_[row][column] % 120 == 0)
			//			{
			//				for (int i = 0; i < kWaterNum; i++)
			//				{
			//					if (water[i]->isEmission_ == false)
			//					{
			//						water[i]->Emission(column, row);
			//
			//						break;
			//					}
			//				}
			//			}
			//		}
			//	}
			//}



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


			/*   水   */

			// 動かす
			//for (int i = 0; i < kWaterNum; i++)
			//{
			//	water[i]->Move();
			//}

			// ブロックに触れたらきえる
			//for (int i = 0; i < kWaterNum; i++)
			//{
			//	for (int j = 0; j < kBlockNum; j++)
			//	{
			//		water[i]->Hit(block[j]);
			//	}
			//}

					//雪
			for (int i = 0; i < kSnowNum; i++)
			{
				snow[i]->Move();
			}

			for (int i = 0; i < kHinokoNum; i++)
			{
				hinoko[i]->Move();
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


			/*   リセット   */

			// Rキーでリセットする
			if (!preKeys[DIK_R] && keys[DIK_R])
			{
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

				// マップを再読み込みする
				if (Scene::selectStage_ == 0)
				{
					Map::LoadFile("./TextFiles/Stage/stage1.csv");
				}
				else if (Scene::selectStage_ == 1)
				{
					Map::LoadFile("./TextFiles/Stage/stage2.csv");
				}
				else if (Scene::selectStage_ == 2)
				{
					Map::LoadFile("./TextFiles/Stage/stage3.csv");
				}
				else if (Scene::selectStage_ == 3)
				{
					Map::LoadFile("./TextFiles/Stage/stage4.csv");
				}
				else if (Scene::selectStage_ == 4)
				{
					Map::LoadFile("./TextFiles/Stage/stage5.csv");
				}
				else if (Scene::selectStage_ == 5)
				{
					Map::LoadFile("./TextFiles/Stage/stage6.csv");
				}
				else if (Scene::selectStage_ == 6)
				{
					Map::LoadFile("./TextFiles/Stage/stage7.csv");
				}
				else if (Scene::selectStage_ == 7)
				{
					Map::LoadFile("./TextFiles/Stage/stage8.csv");
				}
				else if (Scene::selectStage_ == 8)
				{
					Map::LoadFile("./TextFiles/Stage/stage9.csv");
				}
				else if (Scene::selectStage_ == 9)
				{
					Map::LoadFile("./TextFiles/Stage/stage10.csv");
				}
				else if (Scene::selectStage_ == 10)
				{
					Map::LoadFile("./TextFiles/Stage/stage3.csv");
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


			/*   クリア条件   */

			// 残りの宝がなくなったら、スタート画面に戻る
			if (Map::treasureNum <= 0)
			{
				Scene::sceneNo_ = SCENE_START;

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

			// プレイヤー
			player->Draw();

			// ブロック
			for (int i = 0; i < kBlockNum; i++)
			{
				block[i]->Draw();
			}

			// 水
			//for (int i = 0; i < kWaterNum; i++)
			//{
			//	water[i]->Draw();
			//}

			// 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				enemy[i]->Draw();
			}




			//デバック表示
			Novice::ScreenPrintf(8, 8, "STAGE_SELECT");

			if (isActive)
			{
				Novice::ScreenPrintf(8, 28, "Move : AD");
				Novice::ScreenPrintf(8, 48, "JUMP : J");
				Novice::ScreenPrintf(8, 68, "Ladder : W");
				Novice::ScreenPrintf(8, 88, "Interact : SPASE");
			}

			break;

		case SCENE_GAME:
			// ゲーム画面

			// マップ
			Map::Draw();

			// プレイヤー
			player->Draw();

			// ブロック
			for (int i = 0; i < kBlockNum; i++)
			{
				block[i]->Draw();
			}

			// 水
			//for (int i = 0; i < kWaterNum; i++)
			//{
			//	water[i]->Draw();
			//}

			// 雪
			for (int i = 0; i < kSnowNum; i++)
			{
				snow[i]->Draw();
			}

			// 炎
			for (int i = 0; i < kHinokoNum; i++)
			{
				hinoko[i]->Draw();
			}

			// 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				enemy[i]->Draw();
			}


			//デバック表示
			Novice::ScreenPrintf(8, 8, "GAME");
			Novice::ScreenPrintf(500, 8, "Remaining treasure:%d", Map::treasureNum);

			if (isActive)
			{
				Novice::ScreenPrintf(8, 28, "Move : AD");
				Novice::ScreenPrintf(8, 48, "JUMP : J");
				Novice::ScreenPrintf(8, 68, "Ladder : W");
				Novice::ScreenPrintf(8, 88, "CarryBlock : SPACE");
			}

			break;

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

	// 水
	//for (int i = 0; i < kWaterNum; i++)
	//{
	//	delete water[i];
	//}

	// 雪
	for (int i = 0; i < kSnowNum; i++)
	{
		delete snow[i];
	}

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

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
