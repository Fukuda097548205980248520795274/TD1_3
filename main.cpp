#include <Novice.h>
#include "Constant.h"
#include "Enumeration.h"
#include "./Class/Map/Map.h"
#include "./Class/DrawMap/DrawMap.h"
#include "./Class/Object/Player/Player.h"
#include "./Class/Object/CarryBlock/Plastic/Plastic.h"
#include "./Class/Object/CarryBlock/Treasure/Treasure.h"
#include "./Class/Object/CarryBlock/IceGhost/IceGhost.h"
#include "./Class/Object/Enemy/Ghost/Ghost.h"

const char kWindowTitle[] = "LC1C_20_フクダソウワ_タイトル";

// マップ
int Map::map_[kMapRow][kMapColumn];
int Map::treasureNum;

// 描画用マップ
int DrawMap::map_[kMapRow][kMapColumn];

// プラスチック
int Plastic::countID;

// 宝
int Treasure::countID;

// 凍った幽霊
int IceGhost::countID;

// 幽霊
int Ghost::countID;


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
	int nextStage = 0;

	int scene = TITLE;

	int alpha = 0;

	int active = false;
#pragma endregion

#pragma region プレイヤー
	// プレイヤー
	Player* player = new Player();
#pragma endregion

#pragma region 敵
	// ゴースト
	Ghost* ghost[kBlockNum];
	for (int i = 0; i < kBlockNum; i++)
	{
		ghost[i] = new Ghost();
	}
#pragma endregion

#pragma region ブロック
	// プラスチック
	Plastic* plastic[kBlockNum];
	for (int i = 0; i < kBlockNum; i++)
	{
		plastic[i] = new Plastic();
	}

	// 宝
	Treasure* treasure[kBlockNum];
	for (int i = 0; i < kBlockNum; i++)
	{
		treasure[i] = new Treasure();
	}

	// 凍っているゴースト
	IceGhost* iceGhost[kBlockNum];
	for (int i = 0; i < kBlockNum; i++)
	{
		iceGhost[i] = new IceGhost();
	}

	/*#pragma region ブロックの配置
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
						if (plastic[i]->id_ == 0)
						{
							plastic[i]->Putting(column, row);

							break;
						}
					}

					// タイルを消す
					Map::map_[row][column] = TILE_NOTHING;

					break;

				case TILE_TREASURE:
					// 宝

					for (int i = 0; i < kBlockNum; i++)
					{
						if (treasure[i]->id_ == 0)
						{
							treasure[i]->Putting(column, row);

							break;
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
						if (iceGhost[i]->id_ == 0)
						{
							iceGhost[i]->Putting(column, row);

							break;
						}
					}

					// タイルを消す
					Map::map_[row][column] = TILE_NOTHING;

					break;

				case TILE_GHOST:
					// 幽霊

					for (int i = 0; i < kBlockNum; i++)
					{
						if (ghost[i]->id_ == 0)
						{
							ghost[i]->Arrival(column, row);

							break;
						}
					}

					// タイルを消す
					Map::map_[row][column] = TILE_NOTHING;

					break;
				}
			}
		}

	#pragma endregion*/

#pragma region 画像の読み込み
	// 画像
	int ghWhite = Novice::LoadTexture("./NoviceResources/white1x1.png");
#pragma endregion

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		switch (scene)
		{
		case TITLE:
#pragma region TITLE

			///
			/// ↓更新処理ここから
			///

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
					scene = STAGE_SELECT;
				}
			}

			///
			/// ↑更新処理ここまで
			///


			///
			/// ↓描画処理ここから
			///


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
			Novice::ScreenPrintf(0, 20, "alpha:%d", alpha);

			///
			/// ↑描画処理ここまで
			///

			break;
#pragma endregion
		case STAGE_SELECT:
#pragma region STAGE_SELECT
			///
			/// ↓更新処理ここから
			///

			if (keys[DIK_A] && !preKeys[DIK_A])
			{
				if (nextStage > 0)
				{
					nextStage--;
				}
			}

			if (keys[DIK_D] && !preKeys[DIK_D])
			{
				if (nextStage < 20)
				{
					nextStage++;
				}

			}

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				if (nextStage == 0)
				{
					Map::LoadFile("./TextFiles/Stage/stage1.csv");
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
									if (plastic[i]->id_ == 0)
									{
										plastic[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_TREASURE:
								// 宝

								for (int i = 0; i < kBlockNum; i++)
								{
									if (treasure[i]->id_ == 0)
									{
										treasure[i]->Putting(column, row);

										break;
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
									if (iceGhost[i]->id_ == 0)
									{
										iceGhost[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_GHOST:
								// 幽霊

								for (int i = 0; i < kBlockNum; i++)
								{
									if (ghost[i]->id_ == 0)
									{
										ghost[i]->Arrival(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;
							}
						}
					}
				}
				else if (nextStage == 1)
				{
					Map::LoadFile("./TextFiles/Stage/stage2.csv");
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
									if (plastic[i]->id_ == 0)
									{
										plastic[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_TREASURE:
								// 宝

								for (int i = 0; i < kBlockNum; i++)
								{
									if (treasure[i]->id_ == 0)
									{
										treasure[i]->Putting(column, row);

										break;
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
									if (iceGhost[i]->id_ == 0)
									{
										iceGhost[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_GHOST:
								// 幽霊

								for (int i = 0; i < kBlockNum; i++)
								{
									if (ghost[i]->id_ == 0)
									{
										ghost[i]->Arrival(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;
							}
						}
					}
				}
				else if (nextStage == 2)
				{
					Map::LoadFile("./TextFiles/Stage/stage3.csv");
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
									if (plastic[i]->id_ == 0)
									{
										plastic[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_TREASURE:
								// 宝

								for (int i = 0; i < kBlockNum; i++)
								{
									if (treasure[i]->id_ == 0)
									{
										treasure[i]->Putting(column, row);

										break;
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
									if (iceGhost[i]->id_ == 0)
									{
										iceGhost[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_GHOST:
								// 幽霊

								for (int i = 0; i < kBlockNum; i++)
								{
									if (ghost[i]->id_ == 0)
									{
										ghost[i]->Arrival(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;
							}
						}
					}
				}
				else if (nextStage == 3)
				{
					Map::LoadFile("./TextFiles/Stage/stage3.csv");
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
									if (plastic[i]->id_ == 0)
									{
										plastic[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_TREASURE:
								// 宝

								for (int i = 0; i < kBlockNum; i++)
								{
									if (treasure[i]->id_ == 0)
									{
										treasure[i]->Putting(column, row);

										break;
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
									if (iceGhost[i]->id_ == 0)
									{
										iceGhost[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_GHOST:
								// 幽霊

								for (int i = 0; i < kBlockNum; i++)
								{
									if (ghost[i]->id_ == 0)
									{
										ghost[i]->Arrival(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;
							}
						}
					}
				}
				else if (nextStage == 4)
				{
					Map::LoadFile("./TextFiles/Stage/stage3.csv");
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
									if (plastic[i]->id_ == 0)
									{
										plastic[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_TREASURE:
								// 宝

								for (int i = 0; i < kBlockNum; i++)
								{
									if (treasure[i]->id_ == 0)
									{
										treasure[i]->Putting(column, row);

										break;
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
									if (iceGhost[i]->id_ == 0)
									{
										iceGhost[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_GHOST:
								// 幽霊

								for (int i = 0; i < kBlockNum; i++)
								{
									if (ghost[i]->id_ == 0)
									{
										ghost[i]->Arrival(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;
							}
						}
					}
				}
				else if (nextStage == 5)
				{
					Map::LoadFile("./TextFiles/Stage/stage3.csv");
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
									if (plastic[i]->id_ == 0)
									{
										plastic[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_TREASURE:
								// 宝

								for (int i = 0; i < kBlockNum; i++)
								{
									if (treasure[i]->id_ == 0)
									{
										treasure[i]->Putting(column, row);

										break;
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
									if (iceGhost[i]->id_ == 0)
									{
										iceGhost[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_GHOST:
								// 幽霊

								for (int i = 0; i < kBlockNum; i++)
								{
									if (ghost[i]->id_ == 0)
									{
										ghost[i]->Arrival(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;
							}
						}
					}
				}
				else if (nextStage == 6)
				{
					Map::LoadFile("./TextFiles/Stage/stage3.csv");
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
									if (plastic[i]->id_ == 0)
									{
										plastic[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_TREASURE:
								// 宝

								for (int i = 0; i < kBlockNum; i++)
								{
									if (treasure[i]->id_ == 0)
									{
										treasure[i]->Putting(column, row);

										break;
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
									if (iceGhost[i]->id_ == 0)
									{
										iceGhost[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_GHOST:
								// 幽霊

								for (int i = 0; i < kBlockNum; i++)
								{
									if (ghost[i]->id_ == 0)
									{
										ghost[i]->Arrival(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;
							}
						}
					}
				}
				else if (nextStage == 7)
				{
					Map::LoadFile("./TextFiles/Stage/stage3.csv");
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
									if (plastic[i]->id_ == 0)
									{
										plastic[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_TREASURE:
								// 宝

								for (int i = 0; i < kBlockNum; i++)
								{
									if (treasure[i]->id_ == 0)
									{
										treasure[i]->Putting(column, row);

										break;
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
									if (iceGhost[i]->id_ == 0)
									{
										iceGhost[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_GHOST:
								// 幽霊

								for (int i = 0; i < kBlockNum; i++)
								{
									if (ghost[i]->id_ == 0)
									{
										ghost[i]->Arrival(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;
							}
						}
					}
				}
				else if (nextStage == 8)
				{
					Map::LoadFile("./TextFiles/Stage/stage3.csv");
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
									if (plastic[i]->id_ == 0)
									{
										plastic[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_TREASURE:
								// 宝

								for (int i = 0; i < kBlockNum; i++)
								{
									if (treasure[i]->id_ == 0)
									{
										treasure[i]->Putting(column, row);

										break;
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
									if (iceGhost[i]->id_ == 0)
									{
										iceGhost[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_GHOST:
								// 幽霊

								for (int i = 0; i < kBlockNum; i++)
								{
									if (ghost[i]->id_ == 0)
									{
										ghost[i]->Arrival(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;
							}
						}
					}
				}
				else if (nextStage == 9)
				{
					Map::LoadFile("./TextFiles/Stage/stage3.csv");
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
									if (plastic[i]->id_ == 0)
									{
										plastic[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_TREASURE:
								// 宝

								for (int i = 0; i < kBlockNum; i++)
								{
									if (treasure[i]->id_ == 0)
									{
										treasure[i]->Putting(column, row);

										break;
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
									if (iceGhost[i]->id_ == 0)
									{
										iceGhost[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_GHOST:
								// 幽霊

								for (int i = 0; i < kBlockNum; i++)
								{
									if (ghost[i]->id_ == 0)
									{
										ghost[i]->Arrival(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;
							}
						}
					}
				}
				else if (nextStage == 10)
				{
					Map::LoadFile("./TextFiles/Stage/stage3.csv");
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
									if (plastic[i]->id_ == 0)
									{
										plastic[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_TREASURE:
								// 宝

								for (int i = 0; i < kBlockNum; i++)
								{
									if (treasure[i]->id_ == 0)
									{
										treasure[i]->Putting(column, row);

										break;
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
									if (iceGhost[i]->id_ == 0)
									{
										iceGhost[i]->Putting(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;

							case TILE_GHOST:
								// 幽霊

								for (int i = 0; i < kBlockNum; i++)
								{
									if (ghost[i]->id_ == 0)
									{
										ghost[i]->Arrival(column, row);

										break;
									}
								}

								// タイルを消す
								Map::map_[row][column] = TILE_NOTHING;

								break;
							}
						}
					}
				}
				scene = GAME;
			}

			if (keys[DIK_TAB] && !preKeys[DIK_TAB])
			{
				scene = TITLE;
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

			///
			/// ↑更新処理ここまで
			///


			///
			/// ↓描画処理ここから
			///

			if (active)
			{
				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x00000000 + alpha, kFillModeSolid);
			}

			//デバック表示
			Novice::ScreenPrintf(0, 0, "STAGE_SELECT");
			Novice::ScreenPrintf(0, 20, "alpha:%d", active);
			Novice::ScreenPrintf(100, 20, "alpha:%d", alpha);
			Novice::ScreenPrintf(0, 50, "STAGE_SELECT:%d", nextStage);

			///
			/// ↑描画処理ここまで
			///

			break;
#pragma endregion
		case GAME:
#pragma region GAME

			///
			/// ↓更新処理ここから
			///

			// 腐らせる
			Map::Rotten();

			// 操作する
			player->Operation(keys, preKeys);

			// プレイヤーがブロックに乗る
			for (int i = 0; i < kBlockNum; i++)
			{
				player->BlockLanding(plastic[i]);
				player->BlockLanding(treasure[i]);
				player->BlockLanding(iceGhost[i]);
			}

			// ブロックを動かす
			for (int i = 0; i < kBlockNum; i++)
			{
				plastic[i]->Move();
				treasure[i]->Move();
				iceGhost[i]->Move();

				ghost[i]->Move();
			}

			// ブロックの当たり判定
			for (int i = 0; i < kBlockNum; i++)
			{
				for (int j = 0; j < kBlockNum; j++)
				{
					// 同じ種類のブロック
					if (i != j)
					{
						plastic[i]->BlockLanding(plastic[j]);
						treasure[i]->BlockLanding(treasure[j]);
						iceGhost[i]->BlockLanding(iceGhost[j]);
					}

					// 別種類のブロック
					plastic[i]->BlockLanding(treasure[j]);
					plastic[i]->BlockLanding(iceGhost[j]);

					treasure[i]->BlockLanding(plastic[j]);
					treasure[i]->BlockLanding(iceGhost[j]);

					iceGhost[i]->BlockLanding(plastic[j]);
					iceGhost[i]->BlockLanding(treasure[j]);

					ghost[i]->BlockLanding(plastic[j]);
					ghost[i]->BlockLanding(treasure[j]);
					ghost[i]->BlockLanding(iceGhost[j]);

				}
			}

			// ブロックの当たり判定
			for (int i = 0; i < kBlockNum; i++)
			{
				for (int j = 0; j < kBlockNum; j++)
				{
					// 同じ種類のブロック
					if (i != j)
					{
						player->Carry(plastic[i], plastic[j]);
						player->Carry(treasure[i], treasure[j]);
						player->Carry(iceGhost[i], iceGhost[j]);
					}

					// 別種類のブロック
					player->Carry(plastic[i], treasure[j]);
					player->Carry(plastic[i], iceGhost[j]);

					player->Carry(treasure[i], plastic[j]);
					player->Carry(treasure[i], iceGhost[j]);

					player->Carry(iceGhost[i], plastic[j]);
					player->Carry(iceGhost[i], treasure[j]);

				}
			}

			///
			/// ↑更新処理ここまで
			///

			///
			/// ↓描画処理ここから
			///

			// マップ
			Map::Draw(ghWhite);

			// プレイヤー
			player->Draw(ghWhite);

			// ブロック
			for (int i = 0; i < kBlockNum; i++)
			{
				plastic[i]->Draw(ghWhite);
				treasure[i]->Draw(ghWhite);
				iceGhost[i]->Draw(ghWhite);
				ghost[i]->Draw(ghWhite);
			}

			Novice::ScreenPrintf(8, 8, "%d", treasure[1]->hp_);

			//デバック表示
			Novice::ScreenPrintf(0, 0, "GAME");

			break;

			///
			/// ↑描画処理ここまで
			///
#pragma endregion
		case GAME_OVER:
#pragma region GAME_OVER
			///
			/// ↓更新処理ここから
			///

			///
			/// ↑更新処理ここまで
			///


			///
			/// ↓描画処理ここから
			///

			///
			/// ↑描画処理ここまで
			///

			break;
#pragma endregion
		case CLEAR:
#pragma region CLEAR
			///
			/// ↓更新処理ここから
			///

			///
			/// ↑更新処理ここまで
			///


			///
			/// ↓描画処理ここから
			///

			///
			/// ↑描画処理ここまで
			///

			break;
#pragma endregion
		}

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
		delete plastic[i];
		delete treasure[i];
		delete iceGhost[i];
		delete ghost[i];
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}