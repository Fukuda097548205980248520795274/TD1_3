#include "Title.h"

/// <summary>
/// コンストラクタ
/// </summary>
Title::Title()
{
	// 画像
	gh_ = Novice::LoadTexture("./Resources/Images/Scene/title.png");

	// 図形
	shape_.scale = { 480.0f , 192.0f };
	shape_.theta = 0.0f;
	shape_.translate = { static_cast<float>(kScreenWidth / 2) , static_cast<float>(kScreenHeight / 2) + 100.0f };

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
Title::~Title()
{

}

/// <summary>
/// 動かす
/// </summary>
/// <param name="gameFrame">ゲームフレーム</param>
void Title::Move(int gameFrame)
{
	// 90 ~ 240フレームで、不透明度を上げる
	if (gameFrame > 90 && gameFrame <= 240)
	{
		float frameRate = static_cast<float>(gameFrame - 90) / 150.0f;

		alpha_ = static_cast<int>((1.0f - frameRate) * 0.0f + frameRate * 255.0f);
	}

	// 240 ~ 300フレームで、不透明度を下げる
	if (gameFrame > 240 && gameFrame <= 300)
	{
		float frameRate = static_cast<float>(gameFrame - 240) / 60.0f;

		alpha_ = static_cast<int>((1.0f - frameRate) * 255.0f + frameRate * 0.0f);
	}
}

/// <summary>
/// 描画する
/// </summary>
/// <param name="gameFrame">ゲームフレーム</param>
void Title::Draw(int gameFrame)
{
	// 90 ~ 300フレームで描画する
	if (gameFrame > 90 && gameFrame <= 300)
	{
		Novice::DrawQuad
		(
			static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
			static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
			static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
			static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
			0, 0, 960, 382, gh_, 0xFFFFFF00 + alpha_
		);
	}
}