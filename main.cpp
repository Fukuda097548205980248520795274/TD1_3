#include <Novice.h>
#include "Constant.h"
#include "Enumeration.h"
#include "./Class/Scene/Scene.h"
#include "./Class/Map/Map.h"
#include "./Class/DrawMap/DrawMap.h"
#include "./Class/Object/Player/Player.h"
#include "./Class/Object/CarryBlock/Plastic/Plastic.h"
#include "./Class/Object/CarryBlock/Treasure/Treasure.h"
#include "./Class/Object/CarryBlock/IceGhost/IceGhost.h"
#include "./Class/Object/Enemy/Ghost/Ghost.h"
#include "./Class/Object/Particle/Dust/Dust.h"

const char kWindowTitle[] = "LC1C_20_フクダソウワ_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

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

	int alpha = 0;

	int active = false;


	/*   プレイヤー   */

	Player* player = new Player();


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
			// 宝
			block[i] = new Treasure();
		}
		else if (i < 24)
		{
			// 凍った敵
			block[i] = new IceGhost();
		}
	}


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
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
			// スタート画面

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				active = true;
				Scene::FadeOut(alpha, active);
				break;
			}


			break;

		case SCENE_STAGE:
			// ステージセレクト画面

			// 配置準備ができたら（配置準備フラグがtrueだったら）、ブロックを配置する
			if (Scene::isPutPreparation_)
			{
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

						case TILE_TREASURE:
							// 宝

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

							// 宝の数をカウントする
							Map::treasureNum++;

							break;

						case TILE_ICE_GHOST:
							// 凍った幽霊

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

			//フェードアウト

			/*if (active)
			{
				alpha -= 3;

				if (alpha <= 0)
				{
					alpha = 255;
					active = false;
				}

			}*/


			break;

		case SCENE_GAME:
			// ゲーム画面


			// 腐らせる
			Map::Rotten();

			/*   プレイヤー   */

			// 操作する
			player->Operation(keys, preKeys);

			// プレイヤーがブロックに乗る
			for (int i = 0; i < kBlockNum; i++)
			{
				player->BlockLanding(block[i]);
			}


			/*   敵   */

			// 敵を動かす
			for (int i = 0; i < kEnemyNum; i++)
			{
				enemy[i]->Move();
			}

			// 敵がブロックに乗る
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
					player->Carry(keys , preKeys , block[i], block[j]);

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
				} else if (Scene::selectStage_ == 1)
				{
					Map::LoadFile("./TextFiles/Stage/stage2.csv");
				} else if (Scene::selectStage_ == 2)
				{
					Map::LoadFile("./TextFiles/Stage/stage3.csv");
				} else if (Scene::selectStage_ == 3)
				{
					Map::LoadFile("./TextFiles/Stage/stage4.csv");
				} else if (Scene::selectStage_ == 4)
				{
					Map::LoadFile("./TextFiles/Stage/stage5.csv");
				} else if (Scene::selectStage_ == 5)
				{
					Map::LoadFile("./TextFiles/Stage/stage6.csv");
				} else if (Scene::selectStage_ == 6)
				{
					Map::LoadFile("./TextFiles/Stage/stage7.csv");
				} else if (Scene::selectStage_ == 7)
				{
					Map::LoadFile("./TextFiles/Stage/stage8.csv");
				} else if (Scene::selectStage_ == 8)
				{
					Map::LoadFile("./TextFiles/Stage/stage9.csv");
				} else if (Scene::selectStage_ == 9)
				{
					Map::LoadFile("./TextFiles/Stage/stage10.csv");
				} else if (Scene::selectStage_ == 10)
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

						case TILE_TREASURE:
							// 宝

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

							// 宝の数をカウントする
							Map::treasureNum++;

							break;

						case TILE_ICE_GHOST:
							// 凍った幽霊

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
				Scene::sceneNo_ = SCENE_STAGE;

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

			//タイトル名
			Novice::DrawBox(180, 150, 600, 200, 0.0f, 0xFFFFFFFF, kFillModeSolid);

			//スタートボタン
			Novice::DrawBox(300, 500, 350, 100, 0.0f, 0xFFFFFFFF, kFillModeSolid);

			//
			if (active)
			{
				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x00000000 + alpha, kFillModeSolid);
			}

			////デバック表示
			//Novice::ScreenPrintf(0, 0, "TITLE");

			break;

		case SCENE_STAGE:
			// ステージセレクト画面

			if (active)
			{
				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x00000000 + alpha, kFillModeSolid);
			}

			//デバック表示
			Novice::ScreenPrintf(0, 0, "STAGE_SELECT");
			Novice::ScreenPrintf(0, 50, "STAGE_SELECT:%d", Scene::selectStage_ + 1);

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

			// 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				enemy[i]->Draw();
			}

			//デバック表示
			Novice::ScreenPrintf(8, 8, "%d" , Map::treasureNum);

			break;

		}

		///
		/// ↑描画処理ここまで
		///


		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
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

	// ブロック
	for (int i = 0; i < kEnemyNum; i++)
	{
		delete enemy[i];
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
