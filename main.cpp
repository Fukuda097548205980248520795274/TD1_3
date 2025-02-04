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
#include "./Class/Object/Particle/SnowSwitching/SnowSwitching.h"

#include "Switching.h"
#include "./Function/FullScreen/FullScreen.h"
#include "./Class/Texture/Texture.h"
#include "./Class/Texture/Title/Title.h"
#include "./Class/Texture/Transition/Transition.h"
#include "./Class/Texture/SignboardWaremono/SignboardWaremono.h"
#include "./Class/Texture/SignboardKonayuki/SignboardKonayuki.h"
#include "./Class/Texture/SignboardScaffold/SignboardScaffold.h"
#include "./class/Texture/SignboardFlag/SignboardFlag.h"
#include "./Class/Texture/SignboardCarry/SignboardCarry.h"
#include "./Class/Texture/SpaceOrA/SpaceOrA.h"
#include "./Class/Texture/TextNextStage/TextNextStage.h"
#include "./Class/Texture/TextReset/TextReset.h"
#include "./Class/Texture/TextReturn/TextReturn.h"
#include "./Class/Texture/TextGameReturn/TextGameReturn.h"
#include "./Class/Texture/TextGameGiveup/TextGameGiveup.h"
#include "./Class/Texture/TextGameClear/TextGameClear.h"
#include "./Class/Texture/TextGameOver/TextGameOver.h"


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

	// リセットしているかどうか（リセットフラグ）
	int isReset = false;

	// ポーズしているかどうか（ポーズフラグ）
	int isPose = false;

	// ゲームのスティック
	int stickX = 0;
	int stickY = 0;
	
	int preStickX = 0;
	int preStickY = 0;


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

	// 切り替え用の雪
	SnowSwitching* snowSwitching[kParticleSnowSwitching];
	for (int i = 0; i < kParticleSnowSwitching; i++)
	{
		snowSwitching[i] = new SnowSwitching();
	}


	/*   画像   */

	Texture* texture[kTexutreNum];
	texture[0] = new Title();
	texture[1] = new SignboardWaremono();
	texture[2] = new SignboardKonayuki();
	texture[3] = new SignboardScaffold();
	texture[4] = new SpaceOrA();
	texture[5] = new SignboardFlag();
	texture[6] = new TextNextStage();
	texture[7] = new TextReset();
	texture[8] = new TextReturn();
	texture[9] = new SignboardCarry();
	texture[10] = new TextGameReturn();
	texture[11] = new TextGameGiveup();
	texture[12] = new TextGameClear();
	texture[13] = new TextGameOver();
	texture[14] = new Transition();

	// 宝箱
	int ghTreasure = Novice::LoadTexture("./Resources/Images/Map/treasureBox.png");



	/*  　デバック表示　  */
	int isActive = true;
	/*--------
	   背景
	--------*/
	int ghArea1 = Novice::LoadTexture("./Resources/Images/BackGround/area1_bg.png");
	int ghArea2 = Novice::LoadTexture("./Resources/Images/BackGround/area2_bg.png");
	int ghArea3 = Novice::LoadTexture("./Resources/Images/BackGround/area3_bg.png");

	/*----------
		画像
	----------*/

	// タイトルの背景
	int ghTitleBg = Novice::LoadTexture("./Resources/Images/Scene/title_bg.png");

	/*ステージ画面*/
	//エリアテキスト
	int ghEreaText = Novice::LoadTexture("./Resources/Images/Scene/StageSelect/erea.png");

	//エリア名前テキスト
	int ghEreaName[3];
	ghEreaName[0] = Novice::LoadTexture("./Resources/Images/Scene/StageSelect/erea1Name.png");
	ghEreaName[1] = Novice::LoadTexture("./Resources/Images/Scene/StageSelect/erea1Name.png");
	ghEreaName[2] = Novice::LoadTexture("./Resources/Images/Scene/StageSelect/erea1Name.png");

	//ステージ選択テキスト
	int ghStageSelectText = Novice::LoadTexture("./Resources/Images/Scene/StageSelect/stageSelect.png");

	/*ゲーム画面*/
	//残りの宝箱
	int ghNumber[5];
	ghNumber[0] = Novice::LoadTexture("./Resources/Images/Scene/Number/0.png");
	ghNumber[1] = Novice::LoadTexture("./Resources/Images/Scene/Number/1.png");
	ghNumber[2] = Novice::LoadTexture("./Resources/Images/Scene/Number/2.png");
	ghNumber[3] = Novice::LoadTexture("./Resources/Images/Scene/Number/3.png");
	ghNumber[4] = Novice::LoadTexture("./Resources/Images/Scene/Number/4.png");

	//残りの宝箱のテキストUI
	int ghTreasureText = Novice::LoadTexture("./Resources/Images/Scene/Game/treasureText.png");

	// 氷
	int ghIce[4];
	ghIce[0] = Novice::LoadTexture("./Resources/Images/Map/IceBlock/iceBlock4.png");
	ghIce[1] = Novice::LoadTexture("./Resources/Images/Map/IceBlock/iceBlock3.png");
	ghIce[2] = Novice::LoadTexture("./Resources/Images/Map/IceBlock/iceBlock2.png");
	ghIce[3] = Novice::LoadTexture("./Resources/Images/Map/IceBlock/iceBlock1.png");

	// 木
	int ghTree = Novice::LoadTexture("./Resources/Images/BackGround/snow_tree.png");

	/*ポーズメニュー*/
	int ghPauseText = Novice::LoadTexture("./Resources/Images/Scene/Game/pause.png");
	//操作説明
	int ghOperationInstructions = Novice::LoadTexture("./Resources/Images/Scene/Game/operationInstructions.png");

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


	// フルスクリーンにする
	//SetFullScreen(GetActiveWindow());

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


		// 左スティックの方向を更新する
		Novice::GetAnalogInputLeft(0, &stickX, &stickY);

		/*------------------------
			パーティクルを動かす
		------------------------*/

		if (Scene::isClear_ == false && Scene::isGameOver_ == false)
		{
			if (isPose == false)
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
										Water::coolTime = 60;

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
		}

		// 切り替え用の雪
		for (int i = 0; i < kParticleSnowSwitching; i++)
		{
			snowSwitching[i]->Move();
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

			// 300フレームで、切り替え用の雪を放出する
			if (gameFrame == 300)
			{
				// 切り替え用の雪を放出する
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < kParticleSnowSwitching; j++)
					{
						if (snowSwitching[j]->isEmission_ == false)
						{
							snowSwitching[j]->Emission
							({ static_cast<float>(kScreenWidth + 300 + (rand() % (kScreenWidth / 2))) ,static_cast<float>(rand() % kScreenHeight) });

							break;
						}
					}
				}
			}

			// 360フレームで、ステージセレクト画面に移る
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

				if (!preKeys[DIK_D] && keys[DIK_D] || preStickX < 16000 && stickX > 16000)
				{
					int currentStage = Scene::stageNo_;

					if (Scene::stageNo_ == STAGE_6)
					{
						Scene::areaNo_ += 1;
						Scene::stageNo_ = STAGE_1;
					}
					else if(Scene::stageNo_ == STAGE_12)
					{
						Scene::areaNo_ += 1;
						Scene::stageNo_ = STAGE_7;
					}
					else
					{
						Scene::stageNo_ += 1;
					}

					if (Scene::areaNo_ > AREA_3)
					{
						Scene::stageNo_ = currentStage;
						Scene::areaNo_ = AREA_3;
					}
					else
					{
						// 効果音
						Novice::PlayAudio(shPick, 0, 0.3f);
					}
				}
				
				if (!preKeys[DIK_A] && keys[DIK_A] || preStickX > -16000 && stickX < -16000)
				{
					int currentStage = Scene::stageNo_;

					if (Scene::stageNo_ == STAGE_1)
					{
						Scene::areaNo_ -= 1;
						Scene::stageNo_ = STAGE_6;
					} else if (Scene::stageNo_ == STAGE_7)
					{
						Scene::areaNo_ -= 1;
						Scene::stageNo_ = STAGE_12;
					} else
					{
						Scene::stageNo_ -= 1;
					}

					if (Scene::areaNo_ < AREA_1)
					{
						Scene::stageNo_ = currentStage;
						Scene::areaNo_ = AREA_1;
					} else
					{
						// 効果音
						Novice::PlayAudio(shPick, 0, 0.3f);
					}
				}

				if (!preKeys[DIK_W] && keys[DIK_W] || preStickY > -16000 && stickY < -16000)
				{
					if (Scene::stageNo_ > STAGE_6)
					{
						Scene::stageNo_ -= 6;

						// 効果音
						Novice::PlayAudio(shPick, 0, 0.3f);
					}
				}

				if (!preKeys[DIK_S] && keys[DIK_S] || preStickY < 16000 && stickY > 16000)
				{
					if (Scene::stageNo_ <= STAGE_6)
					{
						Scene::stageNo_ += 6;

						// 効果音
						Novice::PlayAudio(shPick, 0, 0.3f);
					}
				}


				// スペースキーで、配置準備する（配置準備フラグがtrueになる）
				if (!preKeys[DIK_SPACE] && keys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
				{
					// 凍結の音
					Novice::PlayAudio(shFrozen, 0, 0.3f);

					// 切り替え用の雪を放出する
					for (int i = 0; i < 8; i++)
					{
						for (int j = 0; j < kParticleSnowSwitching; j++)
						{
							if (snowSwitching[j]->isEmission_ == false)
							{
								snowSwitching[j]->Emission
								({ static_cast<float>(kScreenWidth + 300 + (rand() % (kScreenWidth / 2))) ,static_cast<float>(rand() % kScreenHeight) });

								break;
							}
						}
					}

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

			break;

#pragma endregion
		case SCENE_GAME:
#pragma region シーン:ゲーム
			// ゲーム画面

			// マップの描画を更新する
			if (gameFrame == 481)
			{
				// マップの値を初期化する
				switch (Scene::areaNo_)
				{
				case AREA_1:

					switch (Scene::stageNo_)
					{
					case STAGE_1:

						DrawMap::LoadFile("./TextFiles/Design/area1/stage1.csv");

						break;

					case STAGE_2:

						DrawMap::LoadFile("./TextFiles/Design/area1/stage2.csv");

						break;

					case STAGE_3:

						DrawMap::LoadFile("./TextFiles/Design/area1/stage3.csv");

						break;

					case STAGE_4:

						DrawMap::LoadFile("./TextFiles/Design/area1/stage4.csv");

						break;

					case STAGE_5:

						DrawMap::LoadFile("./TextFiles/Design/area1/stage5.csv");

						break;

					case STAGE_6:

						DrawMap::LoadFile("./TextFiles/Design/area1/stage6.csv");

						break;

					case STAGE_7:

						DrawMap::LoadFile("./TextFiles/Design/area1/stage7.csv");

						break;

					case STAGE_8:

						DrawMap::LoadFile("./TextFiles/Design/area1/stage8.csv");

						break;

					case STAGE_9:

						DrawMap::LoadFile("./TextFiles/Design/area1/stage9.csv");

						break;

					case STAGE_10:

						DrawMap::LoadFile("./TextFiles/Design/area1/stage10.csv");

						break;

					case STAGE_11:

						DrawMap::LoadFile("./TextFiles/Design/area1/stage11.csv");

						break;

					case STAGE_12:

						DrawMap::LoadFile("./TextFiles/Design/area1/stage12.csv");

						break;
					}

					break;

				case AREA_2:

					switch (Scene::stageNo_)
					{
					case STAGE_1:

						DrawMap::LoadFile("./TextFiles/Design/area2/stage1.csv");

						break;

					case STAGE_2:

						DrawMap::LoadFile("./TextFiles/Design/area2/stage2.csv");

						break;

					case STAGE_3:

						DrawMap::LoadFile("./TextFiles/Design/area2/stage3.csv");

						break;

					case STAGE_4:

						DrawMap::LoadFile("./TextFiles/Design/area2/stage4.csv");

						break;

					case STAGE_5:

						DrawMap::LoadFile("./TextFiles/Design/area2/stage5.csv");

						break;

					case STAGE_6:

						DrawMap::LoadFile("./TextFiles/Design/area2/stage6.csv");

						break;

					case STAGE_7:

						DrawMap::LoadFile("./TextFiles/Design/area2/stage7.csv");

						break;

					case STAGE_8:

						DrawMap::LoadFile("./TextFiles/Design/area2/stage8.csv");

						break;

					case STAGE_9:

						DrawMap::LoadFile("./TextFiles/Design/area2/stage9.csv");

						break;

					case STAGE_10:

						DrawMap::LoadFile("./TextFiles/Design/area2/stage10.csv");

						break;

					case STAGE_11:

						DrawMap::LoadFile("./TextFiles/Design/area2/stage11.csv");

						break;

					case STAGE_12:

						DrawMap::LoadFile("./TextFiles/Design/area2/stage12.csv");

						break;
					}

					break;

				case AREA_3:

					switch (Scene::stageNo_)
					{
					case STAGE_1:

						DrawMap::LoadFile("./TextFiles/Design/area3/stage1.csv");

						break;

					case STAGE_2:

						DrawMap::LoadFile("./TextFiles/Design/area3/stage2.csv");

						break;

					case STAGE_3:

						DrawMap::LoadFile("./TextFiles/Design/area3/stage3.csv");

						break;

					case STAGE_4:

						DrawMap::LoadFile("./TextFiles/Design/area3/stage4.csv");

						break;

					case STAGE_5:

						DrawMap::LoadFile("./TextFiles/Design/area3/stage5.csv");

						break;

					case STAGE_6:

						DrawMap::LoadFile("./TextFiles/Design/area3/stage6.csv");

						break;

					case STAGE_7:

						DrawMap::LoadFile("./TextFiles/Design/area3/stage7.csv");

						break;

					case STAGE_8:

						DrawMap::LoadFile("./TextFiles/Design/area3/stage8.csv");

						break;

					case STAGE_9:

						DrawMap::LoadFile("./TextFiles/Design/area3/stage9.csv");

						break;

					case STAGE_10:

						DrawMap::LoadFile("./TextFiles/Design/area3/stage10.csv");

						break;

					case STAGE_11:

						DrawMap::LoadFile("./TextFiles/Design/area3/stage11.csv");

						break;

					case STAGE_12:

						DrawMap::LoadFile("./TextFiles/Design/area3/stage12.csv");

						break;
					}

					break;
				}
			}

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

					// ポーズ中の操作
					if (isPose)
					{
						// ポーズメニュー切り替え
						switch (Scene::poseNo_)
						{
						case POSE_GAME_RETURN:

							if (!preKeys[DIK_S] && keys[DIK_S] || stickY > 16000)
							{
								Scene::poseNo_ = POSE_GAME_GIVEUP;

								// 効果音
								Novice::PlayAudio(shPick, 0, 0.3f);
							}

							break;

						case POSE_GAME_GIVEUP:

							if (!preKeys[DIK_W] && keys[DIK_W] || stickY < -16000)
							{
								Scene::poseNo_ = POSE_GAME_RETURN;

								// 効果音
								Novice::PlayAudio(shPick, 0, 0.3f);
							}

							break;
						}

						// スペースキー
						if (!preKeys[DIK_SPACE] && keys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
						{
							// ポーズを中断する（ポーズフラグをfalseにする）
							if (Scene::poseNo_ == POSE_GAME_RETURN)
							{
								isPose = false;
							}
							else if (Scene::poseNo_ == POSE_GAME_GIVEUP)
							{
								// ゲームをやめる
								if (isLoad == false)
								{
									isLoad = true;

									gameFrame = 600;

									// 切り替え用の雪を放出する
									for (int i = 0; i < 8; i++)
									{
										for (int j = 0; j < kParticleSnowSwitching; j++)
										{
											if (snowSwitching[j]->isEmission_ == false)
											{
												snowSwitching[j]->Emission
												({ static_cast<float>(kScreenWidth + 300 + (rand() % (kScreenWidth / 2))) ,static_cast<float>(rand() % kScreenHeight) });

												break;
											}
										}
									}
								}
							}
						}
					}

					// tabキーで、ポーズする
					if (!preKeys[DIK_TAB] && keys[DIK_TAB] || Novice::IsTriggerButton(0, kPadButton4))
					{
						if (isPose == false)
						{
							isPose = true;
						}
						else
						{
							isPose = false;
						}
					}

					if (isPose == false)
					{
						// 溶かす
						Map::Rotten();


						/*   マップ   */

						Map::Update();


						/*   プレイヤー   */

						// 操作する
						player->Operation(keys, preKeys, stickX, stickY);

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
							} else
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
						if (!preKeys[DIK_R] && keys[DIK_R] || Novice::IsTriggerButton(0, kPadButton5))
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


						/*   クリア条件   */

						// 残りの宝がなくなったらクリア（クリアフラグがtrueになる）
						if (Map::treasureNum <= 0)
						{
							Scene::isClear_ = true;
						}


						/*   敗北条件   */

						// プレイヤーがやられたら（復活フラグがfalseになったら）ゲームオーバー
						if (Scene::isClear_ == false)
						{
							if (player->respawn_.isRespawn == false)
							{
								Scene::isGameOver_ = true;
							}
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
					// クリアしているとき（クリアフラグがtrueであるとき）
					if (Scene::isClear_)
					{
						switch (Scene::clearNo_)
						{
						case CLEAR_NEXT_GAME:
							// 次のステージに進む

							if (!preKeys[DIK_S] && keys[DIK_S] || stickY > 16000)
							{
								Scene::clearNo_ = CLEAR_END_GAME;

								// 効果音
								Novice::PlayAudio(shPick, 0, 0.3f);
							}

							isReset = true;

							break;

						case CLEAR_END_GAME:
							// ゲームを終了する

							if (!preKeys[DIK_W] && keys[DIK_W] || stickY < -16000)
							{
								Scene::clearNo_ = CLEAR_NEXT_GAME;

								// 効果音
								Novice::PlayAudio(shPick, 0, 0.3f);
							}

							isReset = false;

							break;
						}
					}
					else if (Scene::isGameOver_)
					{
						// ゲームオーバーのとき（ゲームオーバーフラグがtrueである）

						switch (Scene::gameoverNo_)
						{
						case GAMEOVER_RESET_GAME:
							// 再挑戦する

							if (!preKeys[DIK_S] && keys[DIK_S] || stickY > 16000)
							{
								Scene::gameoverNo_ = GAMEOVER_END_GAME;

								// 効果音
								Novice::PlayAudio(shPick, 0, 0.3f);
							}

							isReset = true;

							break;

						case GAMEOVER_END_GAME:
							// ゲームを終了する

							if (!preKeys[DIK_W] && keys[DIK_W] || stickY < -16000)
							{
								Scene::gameoverNo_ = GAMEOVER_RESET_GAME;

								// 効果音
								Novice::PlayAudio(shPick, 0, 0.3f);
							}

							isReset = false;

							break;
						}
					}


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

						// 再度プレイする場合は、凍結音を流す
						if (isReset)
						{
							Novice::PlayAudio(shFrozen, 0, 0.3f);
						}
						else
						{
							// 鈴の音
							Novice::PlayAudio(shBell2, 0, 0.3f);
						}

						// 切り替え用の雪を放出する
						for (int i = 0; i < 8; i++)
						{
							for (int j = 0; j < kParticleSnowSwitching; j++)
							{
								if (snowSwitching[j]->isEmission_ == false)
								{
									snowSwitching[j]->Emission
									({ static_cast<float>(kScreenWidth + 300 + (rand() % (kScreenWidth / 2))) ,static_cast<float>(rand() % kScreenHeight) });

									break;
								}
							}
						}
					}
				}
			}

			// 600フレーム以降は、曲を流さない
			if (gameFrame > 600)
			{
				// Bgmを止める
				Novice::StopAudio(phYukisora);
				Novice::StopAudio(phYukinokioku);
				Novice::StopAudio(phYukikaze);
				Novice::StopAudio(phYukinosouretu);
			}

			// 660フレームで、初期化され、ステージセレクトに移る
			if (gameFrame == 660)
			{
				if (isLoad)
				{
					// リセットしているとき（リセットフラグがtrueである）
					if (isReset)
					{
						// フレームを戻す
						gameFrame = 480;

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

						// 宝の数を初期化する
						Map::treasureNum = 0;

						// クリアしていたら（クリアフラグがtrueだったら）次のステージにする
						if (Scene::isClear_)
						{
							if (Scene::stageNo_ < STAGE_12)
							{
								Scene::stageNo_ += 1;
							}
							else
							{
								if (Scene::areaNo_ < AREA_3)
								{
									Scene::stageNo_ = STAGE_1;
									Scene::areaNo_ += 1;
								}
							}
						}

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

						// 鈴を鳴らす
						Novice::PlayAudio(shBell, 0, 0.5f);

						// 切り替えを初期化する
						Scene::clearNo_ = CLEAR_NEXT_GAME;
						Scene::gameoverNo_ = GAMEOVER_RESET_GAME;
						Scene::poseNo_ = POSE_GAME_RETURN;

						// クリアを初期化する（クリアフラグをfalseにする）
						Scene::isClear_ = false;
						Scene::isGameOver_ = false;

						//	リセット終了（リセットフラグをfalseにする）
						isReset = false;

						isPose = false;
					}
					else
					{
						// リセットしないとき（リセットフラグがfalseであるとき）

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

						// 切り替えを初期化する
						Scene::clearNo_ = CLEAR_NEXT_GAME;
						Scene::gameoverNo_ = GAMEOVER_RESET_GAME;
						Scene::poseNo_ = POSE_GAME_RETURN;

						// クリア、ゲームオーバーを初期化する
						Scene::isClear_ = false;
						Scene::isGameOver_ = false;

						// 宝の数を初期化する
						Map::treasureNum = 0;

						isPose = false;
					}
				}
			}


			break;
#pragma endregion
		}

		preStickX = stickX;
		preStickY = stickY;

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
			Novice::DrawSprite(0, -100, ghTitleBg, 0.87f, 0.87f, 0.0f, 0xFFFFFF55);

			// タイトル
			texture[0]->Draw(gameFrame);

			texture[4]->Draw(gameFrame);

			// 雪
			for (int i = 0; i < kSnowNum; i++)
			{
				snow[i]->Draw();
			}

			break;


		case SCENE_STAGE:
			// ステージセレクト画面

			// エリアごとに背景
			switch (Scene::areaNo_)
			{
			case AREA_1:
				// エリア1

				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x000022FF, kFillModeSolid);
				Novice::DrawSprite(0, 0, ghArea1, 1.0f, 1.0f, 0.0f, 0xFFFFFFAA);

				for (int i = 0; i < 3; i++)
				{
					Novice::DrawSprite(0 + 500 * i, 320, ghTree, 0.5f, 0.5f, 0.0f, 0xFFFFFFFF);
				}

				for (int i = 0; i < 2; i++)
				{
					Novice::DrawSprite(300 + 500 * i, 420, ghTree, 0.4f, 0.4f, 0.0f, 0xFFFFFFFF);
				}

				break;

			case AREA_2:
				// エリア2

				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x000022FF, kFillModeSolid);
				Novice::DrawSprite(0, -70, ghArea2, 0.95f, 0.95f, 0.0f, 0xFFFFFF44);

				break;

			case AREA_3:
				// エリア3

				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x000022FF, kFillModeSolid);
				Novice::DrawSprite(0, -150, ghArea3, 1.0f, 1.0f, 0.0f, 0xFFFFFFAA);

				break;

			case AREA_4:
				// エリア4

				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x000044FF, kFillModeSolid);

				break;
			}


			// 雪
			for (int i = 0; i < kSnowNum; i++)
			{
				snow[i]->Draw();
			}

			// 背景
			Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x0038b0A, kFillModeSolid);


			for (int i = 0; i <= STAGE_12; i++)
			{
				Novice::DrawSprite(114 + i % 6 * 200, 320 + (i / 6) * 200, ghTreasure, 2.65f, 2.65f, 0.0f, 0xFFFFFF55);
			}

			if (gameFrame >= 420)
			{
				Novice::DrawSprite(114 + Scene::stageNo_ % 6 * 200, 320 + (Scene::stageNo_ / 6) * 200, ghTreasure, 2.65f, 2.65f, 0.0f, 0xFFFFFFFF);
			}

			// セレクトの画像
			if (gameFrame > 420 && gameFrame < 440)
			{
				Novice::DrawSprite(114 + Scene::stageNo_ % 6 * 200, 320 + (Scene::stageNo_ / 6) * 200,
					ghIce[(gameFrame - 420) / 5], 2.65f, 2.65f, 0.0f, 0xFFFFFFFF);
			}
			else if (gameFrame >= 440)
			{
				Novice::DrawSprite(114 + Scene::stageNo_ % 6 * 200, 320 + (Scene::stageNo_ / 6) * 200,
					ghIce[3], 2.65f, 2.65f, 0.0f, 0xFFFFFFFF);
			}

			//ステージ選択テキスト
			Novice::DrawSprite(32, 32, ghStageSelectText, 0.5f, 0.5f, 0.0f, 0xFFFFFFFF);

			//エリアテキスト
			Novice::DrawBox(412,170,482,82,0.0f,0x00000050,kFillModeSolid);

			Novice::DrawSprite(430, 182, ghEreaText, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);

			if (Scene::areaNo_ == AREA_1)
			{
				Novice::DrawSprite(635, 180, ghEreaName[0], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
			}


			break;

		case SCENE_GAME:
			// ゲーム画面

			// エリアごとに背景
			switch (Scene::areaNo_)
			{
			case AREA_1:
				// エリア1

				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x000022FF, kFillModeSolid);
				Novice::DrawSprite(0, 0, ghArea1, 1.0f, 1.0f, 0.0f, 0xFFFFFFAA);

				for (int i = 0; i < 3; i++)
				{
					Novice::DrawSprite(0 + 500 * i, 320, ghTree, 0.5f, 0.5f, 0.0f, 0xFFFFFFFF);
				}

				for (int i = 0; i < 2; i++)
				{
					Novice::DrawSprite(300 + 500 * i, 420, ghTree, 0.4f, 0.4f, 0.0f, 0xFFFFFFFF);
				}

				break;

			case AREA_2:
				// エリア2

				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x000022FF, kFillModeSolid);
				Novice::DrawSprite(0, -500, ghArea2, 1.0f, 1.0f, 0.0f, 0xFFFFFF44);

				break;

			case AREA_3:
				// エリア3

				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x000022FF, kFillModeSolid);
				Novice::DrawSprite(0, -150, ghArea3, 1.0f, 1.0f, 0.0f, 0xFFFFFFAA);

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
			texture[5]->Draw(gameFrame);
			texture[9]->Draw(gameFrame);

			// マップ
			Map::Draw();
			DrawMap::Draw();



			/*   ブロック   */

			// ブロック
			for (int i = 0; i < kBlockNum; i++)
			{
				block[i]->Draw();
			}

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

			// 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				enemy[i]->Draw();
			}

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


			/*   プレイヤー   */

			// プレイヤー
			player->Draw();

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


			/*   宝の個数   */

			// ゲーム中に表示する
			if (Scene::isClear_ == false && Scene::isGameOver_ == false)
			{
				Novice::DrawSprite(32, 32, ghTreasureText, 0.5f, 0.5f, 0.0f, 0xFFFFFFFF);

				if (Map::treasureNum == 0)
				{
					Novice::DrawSprite(183, 33, ghNumber[0], 0.5f, 0.5f, 0.0f, 0xFFFFFFFF);
				} else if (Map::treasureNum == 1)
				{
					Novice::DrawSprite(183, 33, ghNumber[1], 0.5f, 0.5f, 0.0f, 0xFFFFFFFF);
				} else if (Map::treasureNum == 2)
				{
					Novice::DrawSprite(183, 33, ghNumber[2], 0.5f, 0.5f, 0.0f, 0xFFFFFFFF);
				} else if (Map::treasureNum == 3)
				{
					Novice::DrawSprite(183, 33, ghNumber[3], 0.5f, 0.5f, 0.0f, 0xFFFFFFFF);
				} else if (Map::treasureNum == 4)
				{
					Novice::DrawSprite(183, 33, ghNumber[4], 0.5f, 0.5f, 0.0f, 0xFFFFFFFF);
				}
			}
			
			if (isPose || Scene::isClear_ || Scene::isGameOver_)
			{
				Novice::DrawBox(0, 0, kScreenWidth, kScreenHeight, 0.0f, 0x000000EE, kFillModeSolid);
			}

			// ポーズ中
			if (isPose)
			{
				texture[10]->Draw(gameFrame);
				texture[11]->Draw(gameFrame);
				Novice::DrawSprite(572, 52, ghPauseText, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(42, 92, ghOperationInstructions, 0.7f, 0.7f, 0.0f, 0xFFFFFFFF);
			}

			// メニュー
			texture[6]->Draw(gameFrame);
			texture[7]->Draw(gameFrame);
			texture[8]->Draw(gameFrame);

			texture[12]->Draw(gameFrame);
			texture[13]->Draw(gameFrame);

			break;

		}

		texture[kTexutreNum - 1]->Draw(gameFrame);


		/*-------------------------
			パーティクルを描画する
		-------------------------*/

		// 切り替え用の雪
		for (int i = 0; i < kParticleSnowSwitching; i++)
		{
			snowSwitching[i]->Draw();
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

	// 切り替え用の雪
	for (int i = 0; i < kParticleSnowSwitching; i++)
	{
		delete snowSwitching[i];
	}


	// 画像
	for (int i = 0; i < kTexutreNum; i++)
	{
		delete texture[i];
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
