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

const char kWindowTitle[] = "LC1C_20_フクダソウワ_タイトル";

// シーン
SCENE Scene::sceneNo_ = SCENE_START;
int Scene::gameFrame_ = 0;
int Scene::isOperation_ = false;
int Scene::selectStage_ = 0;
int Scene::isPutPreparation_ = false;

// マップ
int Map::map_[kMapRow][kMapColumn];
int Map::treasureNum;

// 描画用マップ
int DrawMap::map_[kMapRow][kMapColumn];

// 運べるブロック
int CarryBlock::countID;

// 敵
int Enemy::countID;


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
#pragma region マップ
	// マップ
	Map::LoadFile("./TextFiles/Stage/stage1.csv");
#pragma endregion

#pragma region シーン

	int alpha = 0;

	int active = false;
#pragma endregion


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

		// 画面切り替え
		Scene::Switch(keys, preKeys);

		switch (Scene::sceneNo_)
		{
		case SCENE_START:
			// スタート画面

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				active = true;
				break;
			}

			if (active)
			{
				alpha += 3;
				if (alpha >= 255)
				{
					alpha = 255;
					Scene::sceneNo_ = SCENE_STAGE;
				}
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
			if (active)
			{
				alpha -= 3;

				if (alpha <= 0)
				{
					alpha = 255;
					active = false;
				}

			}


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
					player->Carry(block[i], block[j]);

				}
			}

			// 乗っかり、乗っかかりフラグをfalseに戻す
			for (int i = 0; i < kBlockNum; i++)
			{
				block[i]->isRide_ = false;
				block[i]->isUnderRide_ = false;
			}


			break;
		}

		///
		/// ↑更新処理ここまで
		/// 

		///
		/// ↓描画処理ここから
		///

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

			//デバック表示
			Novice::ScreenPrintf(0, 0, "TITLE");

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
			Novice::ScreenPrintf(0, 0, "GAME");

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
