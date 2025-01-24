#include "Scene.h"


SCENE Scene::sceneNo_ = SCENE_START;
AREA Scene::areaNo_ = AREA_STAR_LAND;

int Scene::gameFrame_ = 0;
int Scene::isOperation_ = false;

int Scene::isClear_ = false;
int Scene::isGameOver_ = false;

int Scene::selectStage_ = 0;
int Scene::isPutPreparation_ = false;

int Scene::isTitleIn = false;

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


	// ゲームを操作できない（操作フラグがfalseである）ときに、フレームを進める
	if (isOperation_ == false)
	{
		gameFrame_++;
	}


	// シーン切り替え
	switch (sceneNo_)
	{
	case SCENE_START:
		// スタート画面

		// スペースキーを押すと、タイトル出現をスキップできる
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
		{
			if (isOperation_ == false)
			{
				if (gameFrame_ < 180)
				{
					// フレームを飛ばす
					gameFrame_ = 180;

					// タイトルが入る（タイトルインフラグがtrueになる）
					isTitleIn = true;

					isOperation_ = true;
				}
			}
		}

		// タイトルが入るまで操作ができない
		if (isTitleIn == false)
		{
			if (gameFrame_ >= 180)
			{
				// タイトルが入る（タイトルインフラグがtrueになる）
				isTitleIn = true;

				isOperation_ = true;
			}
		}

		// スペースキーを押すと、エリアセレクトに移る
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
		{
			if (isOperation_)
			{
				if (gameFrame_ == 180)
				{
					isOperation_ = false;
				}
			}
		}

		if (isTitleIn)
		{
			if (gameFrame_ >= 210)
			{
				isOperation_ = true;
				sceneNo_ = SCENE_AREA;
			}
		}

		break;

	case SCENE_AREA:
		// エリアセレクト画面

		// エリア切り替え
		switch (areaNo_)
		{
		case AREA_STAR_LAND:
			// スター島

			// Dキーで、レイ島を選ぶ
			if (!preKeys[DIK_D] && keys[DIK_D])
			{
				areaNo_ = AREA_REI_LAND;
			}

			break;

		case AREA_REI_LAND:
			// レイ島

			// Aキーで、スター島を選ぶ
			if (!preKeys[DIK_A] && keys[DIK_A])
			{
				areaNo_ = AREA_STAR_LAND;
			}

			// Dキーで、チクタク島を選ぶ
			if (!preKeys[DIK_D] && keys[DIK_D])
			{
				areaNo_ = AREA_TIKUTAKU_LAND;
			}

			break;

		case AREA_TIKUTAKU_LAND:
			// チクタク島

			// Aキーで、レイ島を選ぶ
			if (!preKeys[DIK_A] && keys[DIK_A])
			{
				areaNo_ = AREA_REI_LAND;
			}

			// Dキーで、ホッ島を選ぶ
			if (!preKeys[DIK_D] && keys[DIK_D])
			{
				areaNo_ = AREA_HOXTU_LAND;
			}

			break;

		case AREA_HOXTU_LAND:
			// ホッ島

			// Aキーで、チクタク島を選ぶ
			if (!preKeys[DIK_A] && keys[DIK_A])
			{
				areaNo_ = AREA_TIKUTAKU_LAND;
			}

			// Dキーで、ラピッ島を選ぶ
			if (!preKeys[DIK_D] && keys[DIK_D])
			{
				areaNo_ = AREA_RAPIXTU_LAND;
			}

			break;

		case AREA_RAPIXTU_LAND:
			// ラピッ島

			// Aキーで、ラピッ島を選ぶ
			if (!preKeys[DIK_A] && keys[DIK_A])
			{
				areaNo_ = AREA_HOXTU_LAND;
			}

			break;
		}

		// スペースキーを押すと、ステージセレクトに移る
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
		{
			if (isOperation_)
			{
				if (gameFrame_ == 210)
				{
					isOperation_ = false;
				}
			}
		}

		if (isTitleIn)
		{
			if (gameFrame_ >= 240)
			{
				isOperation_ = true;
				sceneNo_ = SCENE_STAGE;
			}
		}

		break;

	case SCENE_STAGE:
		// ステージセレクト画面

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

/// <summary>
/// 描画する
/// </summary>
void Scene::Draw()
{
	// シーン切り替え
	switch (sceneNo_)
	{
	case SCENE_START:
		// スタート画面





		break;

	case SCENE_STAGE:
		// ステージセレクト画面



		break;

	case SCENE_GAME:
		// ゲーム画面


		break;
	}
}