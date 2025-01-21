#include "Scene.h"


SCENE Scene::sceneNo_ = SCENE_START;
int Scene::gameFrame_ = 0;
int Scene::isOperation_ = false;
int Scene::selectStage_ = 0;
int Scene::isPutPreparation_ = false;
int Scene::active_ = false;
int Scene::alpha_ = 0;

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

		//
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
		{
			active_ = true;
			break;
		}

		//フェードイン
		if (active_)
		{
			alpha_ += 3;
			if (alpha_ >= 255)
			{
				alpha_ = 255;
				Scene::sceneNo_ = SCENE_STAGE;
			}
		}
	
		break;

	case SCENE_STAGE:
		// ステージセレクト画面


		//フェードアウト
		if (active_)
		{
			alpha_ -= 3;

			if (alpha_ <= 0)
			{
				alpha_ = 255;
				active_ = false;
			}

		}

		//ステージ切り替え
		if (keys[DIK_UP] && !preKeys[DIK_UP])
		{
			if (selectStage_ > 0)
			{
				selectStage_--;
			}
		}

		if (keys[DIK_DOWN] && !preKeys[DIK_DOWN])
		{
			if (selectStage_ < 20)
			{
				selectStage_++;
			}

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