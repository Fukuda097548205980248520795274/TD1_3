#include "Scene.h"

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

		if (keys[DIK_A] && !preKeys[DIK_A])
		{
			if (selectStage_ > 0)
			{
				selectStage_--;
			}
		}

		if (keys[DIK_D] && !preKeys[DIK_D])
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

		break;

	case SCENE_GAME:
		// ゲーム画面


		break;
	}
}