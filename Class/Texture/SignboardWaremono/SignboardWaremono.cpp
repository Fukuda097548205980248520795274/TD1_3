#include "SignboardWaremono.h"

/// <summary>
/// コンストラクタ
/// </summary>
SignboardWaremono::SignboardWaremono()
{
	// 画像
	gh_ = Novice::LoadTexture("./Resources/Images/Signboard/Signboard_waremono.png");

	// 図形
	shape_.scale = { 64.0f , 32.0f };
	shape_.theta = 0.0f;
	shape_.translate = { static_cast<float>(kScreenWidth / 2) + 50.0f , static_cast<float>(kScreenHeight / 2) + 80.0f };

	// 位置
	pos_.local = { {-1.0f , 1.0f} , {1.0f , 1.0f} , {-1.0f , -1.0f} , {1.0f , -1.0f} };
	LocalToScreen();

	// 移動速度
	vel_ = { 0.0f , 0.0f };

	// フレーム
	frame_ = { 0 , 0 , 0.0f };

	// 透明度
	alpha_ = 0;
}

/// <summary>
/// デストラクタ
/// </summary>
SignboardWaremono::~SignboardWaremono()
{

}

/// <summary>
/// 動かす
/// </summary>
/// <param name="gameFrame">ゲームフレーム</param>
void SignboardWaremono::Move(int gameFrame)
{
	gameFrame;
}

/// <summary>
/// 描画する
/// </summary>
/// <param name="gameFrame">ゲームフレーム</param>
void SignboardWaremono::Draw(int gameFrame)
{
	// エリア1 ステージ3 で描画する
	if (Scene::areaNo_ == AREA_1)
	{
		if (Scene::stageNo_ == STAGE_3)
		{
			// 480 ~ 660フレームで描画する
			if (gameFrame > 480 && gameFrame <= 660)
			{
				Novice::DrawQuad
				(
					static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
					static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
					static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
					static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
					0, 0, 128, 64, gh_, 0xFFFFFFFF
				);
			}
		}
	}
}