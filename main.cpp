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

#include "Switching.h"
#include "./Class/Texture/Texture.h"
#include "./Class/Texture/Title/Title.h"
#include "./Class/Texture/Transition/Transition.h"
#include "./Class/Texture/SignboardWaremono/SignboardWaremono.h"
#include "./Class/Texture/SignboardKonayuki/SignboardKonayuki.h"
#include "./Class/Texture/SignboardScaffold/SignboardScaffold.h"
#include "./Class/Texture/SpaceOrA/SpaceOrA.h"


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

	// ゲームフレーム
	int gameFrame = 0;

	// ロードしているかどうか（ロードフラグ）
	int isLoad = true;

	//ゲームオーバー時、ステージ選択画面に戻る為のフラグ
	int isStageStop = false;

	//ゲームオーバー時、リスタートさせる為のフラグ
	int isRestart = false;

	/*   ゲームシステム   */

	// マップ
	Map::LoadFile("./TextFiles/Stage/stage1.csv");

	// プレイヤー
	Player* player = new Player();

	Switching* switching = new Switching;

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


	/*   画像   */

	Texture* texture[kTexutreNum];
	texture[0] = new Title();
	texture[1] = new SignboardWaremono();
	texture[2] = new SignboardKonayuki();
	texture[3] = new SignboardScaffold();
	texture[4] = new SpaceOrA();
	texture[5] = new Transition();



	/*  　デバック表示　  */
	int isActive = true;


	/*----------
	    画像
	----------*/

	// タイトルの背景
	int ghTitleBg = Novice::LoadTexture("./Resources/Images/Scene/title_bg.png");


	/*--------
		SE
	--------*/

	// 鈴の音
	int shBell = Novice::LoadAudio("./Resources/Sounds/Se/bell.mp3");
	int shBell2 = Novice::LoadAudio("./Resources/Sounds/Se/bell2.mp3");

	// 凍結の音
	int shFrozen = Novice::LoadAudio("./Resources/Sounds/Se/frozen.mp3");

	// 選ぶときの音
	int shPick = Novice::LoadAudio("./Resources/Sounds/Se/pick.mp3");


	/*-----------
		BGM
	----------*/

	// LostMemories
	int shLostMemories = Novice::LoadAudio("./Resources/Sounds/Bgm/lost_memories.mp3");
	int phLostMemories = -1;

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

	// A Dream of Cat
	int shADreamOfCat = Novice::LoadAudio("./Resources/Sounds/Bgm/A_Dream_of_a_Cat.mp3");
	int phADreamOfCat = -1;

	// in the stillness of twilight
	int shInTheStillnessOfTwilight = Novice::LoadAudio("./Resources/Sounds/Bgm/in_the_stillness_of_twilight.mp3");
	int phInTheStillnessOfTwilight = -1;

	// 軽井沢の鳥の鳴き声
	int shBard = Novice::LoadAudio("./Resources/Sounds/Bgm/bard1.mp3");
	int phBard = -1;

	// 風
	int shWind = Novice::LoadAudio("./Resources/Sounds/Bgm/wind.mp3");
	int phWind = -1;


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

		// 鳥の鳴き声
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

		// 風
		if (!Novice::IsPlayingAudio(phWind) || phWind == -1)
		{
			phWind = Novice::PlayAudio(shWind, 0, 0.02f);
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


		/*------------------
			ゲームシステム
		------------------*/

		// ロード中にフレームを動かす
		if (isLoad)
		{
			gameFrame++;
		}

		// 画像
		for (int i = 0; i < kTexutreNum; i++)
		{
			texture[i]->Move(gameFrame);
		}

		// 画面切り替え
		switch (Scene::sceneNo_)
		{
		case SCENE_START:
			// スタート画面

			// BGM
			if (!Novice::IsPlayingAudio(phLostMemories) || phLostMemories == -1)
			{
				phLostMemories = Novice::PlayAudio(shLostMemories, 0, 0.1f);
			}

			/*   操作   */

			// 0 ~ 239フレームで、スペースキーを押すと、スキップできる
			if (!preKeys[DIK_SPACE] && keys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
			{
				if (gameFrame > 0 && gameFrame < 240)
				{
					if (isLoad)
					{
						gameFrame = 239;
					}
				}
			}

			// 240フレームで、ロードを終了する（ロードフラグをfalseにする）
			if (gameFrame == 240)
			{
				if (isLoad)
				{
					isLoad = false;
				}
			}

			// 240フレームで、スペースキーを押すと、ロードする（ロードフラグがtrueになる）
			if (!preKeys[DIK_SPACE] && keys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
			{
				if (gameFrame == 240)
				{
					if (isLoad == false)
					{
						isLoad = true;

						// 鈴の音
						Novice::PlayAudio(shBell2, 0, 0.3f);
					}
				}
			}

			// 241 ~ 259フレームで、スペースキーを押すと、スキップできる
			if (!preKeys[DIK_SPACE] && keys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
			{
				if (gameFrame > 240 && gameFrame < 360)
				{
					if (isLoad)
					{
						gameFrame = 359;
					}
				}
			}

			// 360フレームで、エリアセレクト画面に移る
			if (gameFrame == 360)
			{
				if (isLoad)
				{
					Scene::sceneNo_ = SCENE_STAGE;
				}
			}


			break;

		case SCENE_STAGE:
			// ステージセレクト画面

			// BGM
			if (!Novice::IsPlayingAudio(phLostMemories) || phLostMemories == -1)
			{
				phLostMemories = Novice::PlayAudio(shLostMemories, 0, 0.1f);
			}


			/*   操作   */

			// 420フレームで、ロードが終了する（ロードフラグがfalseになる）
			if (gameFrame == 420)
			{
				if (isLoad)
				{
					isLoad = false;
				}

				// ステージを選ぶ
				switch (Scene::areaNo_)
				{
				case AREA_1:

					switch (Scene::stageNo_)
					{
					case STAGE_1:

						Map::LoadFile("./TextFiles/Stage/area1/stage1.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_2;

							// 効果音
							Novice::PlayAudio(shPick , 0 , 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_7;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_2:

						Map::LoadFile("./TextFiles/Stage/area1/stage2.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_3;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_1;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_8;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_3:

						Map::LoadFile("./TextFiles/Stage/area1/stage3.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_4;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_2;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_9;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_4:

						Map::LoadFile("./TextFiles/Stage/area1/stage4.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_5;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_3;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_10;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_5:

						Map::LoadFile("./TextFiles/Stage/area1/stage5.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_6;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_4;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_11;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_6:

						Map::LoadFile("./TextFiles/Stage/area1/stage6.csv");

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_5;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::areaNo_ = AREA_2;
							Scene::stageNo_ = STAGE_1;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_12;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_7:

						Map::LoadFile("./TextFiles/Stage/area1/stage7.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_8;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_1;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_8:

						Map::LoadFile("./TextFiles/Stage/area1/stage8.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_9;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_7;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_2;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_9:

						Map::LoadFile("./TextFiles/Stage/area1/stage9.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_10;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_8;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_3;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_10:

						Map::LoadFile("./TextFiles/Stage/area1/stage10.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_11;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_9;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_4;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_11:

						Map::LoadFile("./TextFiles/Stage/area1/stage11.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_12;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_10;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_5;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_12:

						Map::LoadFile("./TextFiles/Stage/area1/stage12.csv");

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_11;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::areaNo_ = AREA_2;
							Scene::stageNo_ = STAGE_7;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_6;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;
					}

					break;
					
				case AREA_2:

					switch (Scene::stageNo_)
					{
					case STAGE_1:

						Map::LoadFile("./TextFiles/Stage/area2/stage1.csv");

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::areaNo_ = AREA_1;
							Scene::stageNo_ = STAGE_6;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_2;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_7;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_2:

						Map::LoadFile("./TextFiles/Stage/area2/stage2.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_3;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_1;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_8;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_3:

						Map::LoadFile("./TextFiles/Stage/area2/stage3.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_4;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_2;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_9;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_4:

						Map::LoadFile("./TextFiles/Stage/area2/stage4.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_5;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_3;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_10;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_5:

						Map::LoadFile("./TextFiles/Stage/area2/stage5.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_6;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_4;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_11;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_6:

						Map::LoadFile("./TextFiles/Stage/area2/stage6.csv");

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_5;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::areaNo_ = AREA_3;
							Scene::stageNo_ = STAGE_1;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_12;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_7:

						Map::LoadFile("./TextFiles/Stage/area2/stage7.csv");

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::areaNo_ = AREA_1;
							Scene::stageNo_ = STAGE_12;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_8;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_1;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_8:

						Map::LoadFile("./TextFiles/Stage/area2/stage8.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_9;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_7;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_2;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_9:

						Map::LoadFile("./TextFiles/Stage/area2/stage9.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_10;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_8;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_3;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_10:

						Map::LoadFile("./TextFiles/Stage/area2/stage10.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_11;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_9;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_4;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_11:

						Map::LoadFile("./TextFiles/Stage/area2/stage11.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_12;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_10;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_5;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_12:

						Map::LoadFile("./TextFiles/Stage/area2/stage12.csv");

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_11;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::areaNo_ = AREA_3;
							Scene::stageNo_ = STAGE_7;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_6;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;
					}

					break;

				case AREA_3:

					switch (Scene::stageNo_)
					{
					case STAGE_1:

						Map::LoadFile("./TextFiles/Stage/area3/stage1.csv");

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::areaNo_ = AREA_2;
							Scene::stageNo_ = STAGE_6;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_2;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_7;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_2:

						Map::LoadFile("./TextFiles/Stage/area3/stage2.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_3;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_1;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_8;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_3:

						Map::LoadFile("./TextFiles/Stage/area3/stage3.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_4;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_2;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_9;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_4:

						Map::LoadFile("./TextFiles/Stage/area3/stage4.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_5;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_3;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_10;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_5:

						Map::LoadFile("./TextFiles/Stage/area3/stage5.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_6;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_4;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_11;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_6:

						Map::LoadFile("./TextFiles/Stage/area3/stage6.csv");

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_5;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_S] && keys[DIK_S])
						{
							Scene::stageNo_ = STAGE_12;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_7:

						Map::LoadFile("./TextFiles/Stage/area3/stage7.csv");

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::areaNo_ = AREA_2;
							Scene::stageNo_ = STAGE_12;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_8;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_1;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_8:

						Map::LoadFile("./TextFiles/Stage/area3/stage8.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_9;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_7;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_2;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_9:

						Map::LoadFile("./TextFiles/Stage/area3/stage9.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_10;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_8;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_3;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_10:

						Map::LoadFile("./TextFiles/Stage/area3/stage10.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_11;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_9;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_4;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_11:

						Map::LoadFile("./TextFiles/Stage/area3/stage11.csv");

						if (!preKeys[DIK_D] && keys[DIK_D])
						{
							Scene::stageNo_ = STAGE_12;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_10;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_5;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;

					case STAGE_12:

						Map::LoadFile("./TextFiles/Stage/area3/stage12.csv");

						if (!preKeys[DIK_A] && keys[DIK_A])
						{
							Scene::stageNo_ = STAGE_11;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						if (!preKeys[DIK_W] && keys[DIK_W])
						{
							Scene::stageNo_ = STAGE_6;

							// 効果音
							Novice::PlayAudio(shPick, 0, 0.3f);
						}

						break;
					}

					break;
				}

				// スペースキーで、配置準備する（配置準備フラグがtrueになる）
				if (!preKeys[DIK_SPACE] && keys[DIK_SPACE])
				{
					// 凍結の音
					Novice::PlayAudio(shFrozen , 0 , 0.3f);

					Scene::isPutPreparation_ = true;
				}

				// 配置準備ができたら、ロードを開始する（ロードフラグをtrueにする）
				if (isLoad == false)
				{
					if (Scene::isPutPreparation_)
					{
						isLoad = true;
					}
				}
			}

			// 480フレームで、ブロックを配置し、ゲーム画面に切り替える
			if (gameFrame == 480)
			{
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

					// bgmを止める
					Novice::StopAudio(phLostMemories);

					// 鈴を鳴らす
					Novice::PlayAudio(shBell, 0, 0.5f);
				}
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

			break;

#pragma endregion
		case SCENE_GAME:
#pragma region シーン:ゲーム
			// ゲーム画面


			// 540フレームで、ロードを終了し、ゲームを開始する（ロードフラグをfalseにする）
			if (gameFrame == 540)
			{
				if (isLoad)
				{
					isLoad = false;
				}
			}

			// 600フレームで、ロードを終了し、ゲーム後の操作を可能にする（ロードフラグをfalseにする）
			if (gameFrame == 600)
			{
				if (isLoad)
				{
					isLoad = false;
				}
			}


			// クリア、ゲームオーバーになるまで（クリア、ゲームオーバーフラグがfalseのときは）、操作できる
			if (Scene::isClear_ == false && Scene::isGameOver_ == false)
			{
				// 540フレームで操作できる
				if (gameFrame == 540)
				{
					// エリアごとに曲を変える
					switch (Scene::areaNo_)
					{
					case AREA_1:
						// エリア1

						if (!Novice::IsPlayingAudio(phYukisora) || phYukisora == -1)
						{
							phYukisora = Novice::PlayAudio(shYukisora, 0, 0.1f);
						}

						break;

					case AREA_2:
						// エリア2

						if (!Novice::IsPlayingAudio(phYukinokioku) || phYukinokioku == -1)
						{
							phYukinokioku = Novice::PlayAudio(shYukinokioku, 0, 0.1f);
						}

						break;

					case AREA_3:
						// エリア3

						if (!Novice::IsPlayingAudio(phYukinosouretu) || phYukinosouretu == -1)
						{
							phYukinosouretu = Novice::PlayAudio(shYukinosouretu, 0, 0.1f);
						}

						break;

					case AREA_4:
						// エリア4
						
						if (!Novice::IsPlayingAudio(phYukikaze) || phYukikaze == -1)
						{
							phYukikaze = Novice::PlayAudio(shYukikaze, 0, 0.1f);
						}

						break;
					}

					// 溶かす
					Map::Rotten();


					/*   マップ   */

					Map::Update();


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

					for (int i = 0; i < kEnemyNum; i++)
					{
						enemy[i]->isRide_ = false;
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

						// マップの値を初期化する
						switch (Scene::areaNo_)
						{
						case AREA_1:

							switch (Scene::stageNo_)
							{
							case STAGE_1:

								Map::LoadFile("./TextFiles/Stage/area1/stage1.csv");

								break;

							case STAGE_2:

								Map::LoadFile("./TextFiles/Stage/area1/stage2.csv");

								break;

							case STAGE_3:

								Map::LoadFile("./TextFiles/Stage/area1/stage3.csv");

								break;

							case STAGE_4:

								Map::LoadFile("./TextFiles/Stage/area1/stage4.csv");

								break;

							case STAGE_5:

								Map::LoadFile("./TextFiles/Stage/area1/stage5.csv");

								break;

							case STAGE_6:

								Map::LoadFile("./TextFiles/Stage/area1/stage6.csv");

								break;

							case STAGE_7:

								Map::LoadFile("./TextFiles/Stage/area1/stage7.csv");

								break;

							case STAGE_8:

								Map::LoadFile("./TextFiles/Stage/area1/stage8.csv");

								break;

							case STAGE_9:

								Map::LoadFile("./TextFiles/Stage/area1/stage9.csv");

								break;

							case STAGE_10:

								Map::LoadFile("./TextFiles/Stage/area1/stage10.csv");

								break;

							case STAGE_11:

								Map::LoadFile("./TextFiles/Stage/area1/stage11.csv");

								break;

							case STAGE_12:

								Map::LoadFile("./TextFiles/Stage/area1/stage12.csv");

								break;
							}

							break;

						case AREA_2:

							switch (Scene::stageNo_)
							{
							case STAGE_1:

								Map::LoadFile("./TextFiles/Stage/area2/stage1.csv");

								break;

							case STAGE_2:

								Map::LoadFile("./TextFiles/Stage/area2/stage2.csv");

								break;

							case STAGE_3:

								Map::LoadFile("./TextFiles/Stage/area2/stage3.csv");

								break;

							case STAGE_4:

								Map::LoadFile("./TextFiles/Stage/area2/stage4.csv");

								break;

							case STAGE_5:

								Map::LoadFile("./TextFiles/Stage/area2/stage5.csv");

								break;

							case STAGE_6:

								Map::LoadFile("./TextFiles/Stage/area2/stage6.csv");

								break;

							case STAGE_7:

								Map::LoadFile("./TextFiles/Stage/area2/stage7.csv");

								break;

							case STAGE_8:

								Map::LoadFile("./TextFiles/Stage/area2/stage8.csv");

								break;

							case STAGE_9:

								Map::LoadFile("./TextFiles/Stage/area2/stage9.csv");

								break;

							case STAGE_10:

								Map::LoadFile("./TextFiles/Stage/area2/stage10.csv");

								break;

							case STAGE_11:

								Map::LoadFile("./TextFiles/Stage/area2/stage11.csv");

								break;

							case STAGE_12:

								Map::LoadFile("./TextFiles/Stage/area2/stage12.csv");

								break;
							}

							break;

						case AREA_3:

							switch (Scene::stageNo_)
							{
							case STAGE_1:

								Map::LoadFile("./TextFiles/Stage/area3/stage1.csv");

								break;

							case STAGE_2:

								Map::LoadFile("./TextFiles/Stage/area3/stage2.csv");

								break;

							case STAGE_3:

								Map::LoadFile("./TextFiles/Stage/area3/stage3.csv");

								break;

							case STAGE_4:

								Map::LoadFile("./TextFiles/Stage/area3/stage4.csv");

								break;

							case STAGE_5:

								Map::LoadFile("./TextFiles/Stage/area3/stage5.csv");

								break;

							case STAGE_6:

								Map::LoadFile("./TextFiles/Stage/area3/stage6.csv");

								break;

							case STAGE_7:

								Map::LoadFile("./TextFiles/Stage/area3/stage7.csv");

								break;

							case STAGE_8:

								Map::LoadFile("./TextFiles/Stage/area3/stage8.csv");

								break;

							case STAGE_9:

								Map::LoadFile("./TextFiles/Stage/area3/stage9.csv");

								break;

							case STAGE_10:

								Map::LoadFile("./TextFiles/Stage/area3/stage10.csv");

								break;

							case STAGE_11:

								Map::LoadFile("./TextFiles/Stage/area3/stage11.csv");

								break;

							case STAGE_12:

								Map::LoadFile("./TextFiles/Stage/area3/stage12.csv");

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


					// クリア、またはゲームオーバーになると、ロードが開始される（ロードフラグがtrueになる）
					if (Scene::isClear_ || Scene::isGameOver_)
					{
						if (isLoad == false)
						{
							isLoad = true;
						}
					}
				}
			}
			else
			{
				// Bgmを止める
				Novice::StopAudio(phYukisora);
				Novice::StopAudio(phYukinokioku);
				Novice::StopAudio(phYukikaze);
				Novice::StopAudio(phYukinosouretu);

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


				// 600フレームで、ゲーム後の操作ができる
				if (gameFrame == 600)
				{
					// スペースキーで、ロードする（ロードフラグがtrueになる）
					if (!preKeys[DIK_SPACE] && keys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
					{
						//ロードフラグをtrueにする
						if (isLoad == false)
						{
							isLoad = true;
						}

						//ステージ選択に戻る為のフラグをtrueにする
						if (isStageStop == false)
						{
							isStageStop = true;
						}
					}

					//Rキーで、リスタートする(ロードフラグがtrueになる）
					if (!preKeys[DIK_R] && keys[DIK_R])
					{
						//ロードフラグをtrueにする
						if (isLoad == false)
						{
							isLoad = true;
						}

						//ステージをリスタートする為のフラグをtrueにする
						if (isRestart == false)
						{
							isRestart = true;
						}

					}
				}
			}

			// 660フレームで、初期化され、ステージセレクトに移る
			if (gameFrame == 660)
			{
				if (isLoad)
				{
					Scene::sceneNo_ = SCENE_STAGE;
					gameFrame = 360;
          
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

			// 背景
			Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x000022FF, kFillModeSolid);
			Novice::DrawSprite(0,-100, ghTitleBg, 0.87f, 0.87f, 0.0f, 0xFFFFFF55);

			// タイトル
			texture[0]->Draw(gameFrame);

			texture[4]->Draw(gameFrame);

			break;


		case SCENE_STAGE:
			// ステージセレクト画面

			// 背景
			Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x000044FF, kFillModeSolid);

			switch (Scene::areaNo_)
			{
			case AREA_1:

				Novice::DrawBox(128 + Scene::stageNo_ % 6 * 200, 128 + (Scene::stageNo_ / 6) * 200, 128, 128, 0.0f, 0xFFFFFFFF, kFillModeWireFrame);

				break;

			case AREA_2:

				Novice::DrawBox(128 + Scene::stageNo_ % 6 * 200, 128 + (Scene::stageNo_ / 6) * 200, 128, 128, 0.0f, 0xFFFFFFFF, kFillModeWireFrame);

				break;

			case AREA_3:

				Novice::DrawBox(128 + Scene::stageNo_ % 6 * 200, 128 + (Scene::stageNo_ / 6) * 200, 128, 128, 0.0f, 0xFFFFFFFF, kFillModeWireFrame);

				break;
			}


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

			// エリアごとに背景
			switch (Scene::areaNo_)
			{
			case AREA_1:
				// エリア1

				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x000044FF, kFillModeSolid);

				break;

			case AREA_2:
				// エリア2

				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x000044FF, kFillModeSolid);

				break;

			case AREA_3:
				// エリア3

				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x000044FF, kFillModeSolid);

				break;

			case AREA_4:
				// エリア4

				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x000044FF, kFillModeSolid);

				break;
			}

			// 看板
			texture[1]->Draw(gameFrame);
			texture[2]->Draw(gameFrame);
			texture[3]->Draw(gameFrame);

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
				Novice::ScreenPrintf(580, 390, "Spase or R");
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

		texture[kTexutreNum - 1]->Draw(gameFrame);


		/*-------------------------
			パーティクルを描画する
		-------------------------*/

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


	// 画像
	for (int i = 0; i < kTexutreNum; i++)
	{
		delete texture[i];
	}

	delete switching;


	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
