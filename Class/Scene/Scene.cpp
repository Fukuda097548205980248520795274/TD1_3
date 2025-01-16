#include "Scene.h"


SCENE Scene::sceneNo_ = SCENE_START;
int Scene::gameFrame_ = 0;
int Scene::isOperation_ = false;
int Scene::selectStage_ = 0;
int Scene::isPutPreparation_ = false;

/// <summary>
/// 画面を切り替える
/// </summary>
/// <param name="keys"></param>
/// <param name="preKeys"></param>
void Scene::Switch(const char* keys, const char* preKeys)
{
	// null を探す
	if (keys == nullptr || preKeys == nullptr)
	{
		return;
	}


	switch (sceneNo_)
	{
	case SCENE_START:
		// スタート画面


		break;

	case SCENE_STAGE:
		// ステージセレクト画面

		/*　↓更新処理↓　*/
		if (keys[DIK_W] && !preKeys[DIK_W])
		{
			if (selectStage_ > 0)
			{
				selectStage_--;
			}
		}

		if (keys[DIK_S] && !preKeys[DIK_S])
		{
			if (selectStage_ < 20)
			{
				selectStage_++;
			}
		}

		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
		{
			if (selectStage_ == 0)
			{
				Map::LoadFile("./TextFiles/Stage/stage1.csv");
			}
			else if (selectStage_ == 1)
			{
				Map::LoadFile("./TextFiles/Stage/stage2.csv");
			}
			else if (selectStage_ == 2)
			{
				Map::LoadFile("./TextFiles/Stage/stage3.csv");
			}
			else if (selectStage_ == 3)
			{
				Map::LoadFile("./TextFiles/Stage/stage4.csv");
			}
			else if (selectStage_ == 4)
			{
				Map::LoadFile("./TextFiles/Stage/stage5.csv");
			}
			else if (selectStage_ == 5)
			{
				Map::LoadFile("./TextFiles/Stage/stage6.csv");
			}
			else if (selectStage_ == 6)
			{
				Map::LoadFile("./TextFiles/Stage/stage7.csv");
			}
			else if (selectStage_ == 7)
			{
				Map::LoadFile("./TextFiles/Stage/stage8.csv");
			}
			else if (selectStage_ == 8)
			{
				Map::LoadFile("./TextFiles/Stage/stage9.csv");
			}
			else if (selectStage_ == 9)
			{
				Map::LoadFile("./TextFiles/Stage/stage10.csv");
			}
			else if (selectStage_ == 10)
			{
				Map::LoadFile("./TextFiles/Stage/stage3.csv");
			}

			// 配置準備を行う（配置準備フラグがtrueになる）
			isPutPreparation_ = true;
		}

		if (keys[DIK_TAB] && !preKeys[DIK_TAB])
		{
			Scene::sceneNo_ = SCENE_START;
		}

		/*　↓描画処理↓　*/
		//背景

		//プレイヤー

		//エレベーター
		Novice::DrawBox(120, 150, 600, 500, 0.0f, 0xFFFFFFFF, kFillModeSolid);
		Novice::DrawBox(0, 650, 1400, 700, 0.0f, 0xFFF00FFF, kFillModeSolid);
		//ステージ番号
		Novice::DrawBox(840, 300, 400, 200, 0.0f, 0x000000FF, kFillModeSolid);
		break;

	case SCENE_GAME:
		// ゲーム画面


		break;
	}
}

void Scene::FadeOut(int alpha_, int active_)
{
	if (active_)
	{
		alpha_ += 3;

		if (alpha_ >= 255)
		{
			alpha_ = 255;
			Scene::sceneNo_ = SCENE_STAGE;
		}
	}

}

void Scene::FadeIn(int alpha_, int active_)
{
	if (active_)
	{
		alpha_ -= 3;

		if (alpha_ <= 0)
		{
			alpha_ = 255;
			active_ = false;
		}
	}

}
