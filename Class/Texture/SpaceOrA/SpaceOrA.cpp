#include "SpaceOrA.h"

/// <summary>
/// コンストラクタ
/// </summary>
SpaceOrA::SpaceOrA()
{
	// 画像
	gh_ = Novice::LoadTexture("./Resources/Images/Scene/SpaceOrA.png");

	// 図形
	shape_.scale = { 192.0f , 64.0f };
	shape_.theta = 0.0f;
	shape_.translate = { static_cast<float>(kScreenWidth / 2) , static_cast<float>(kScreenHeight / 2) - 200.0f };

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
SpaceOrA::~SpaceOrA()
{

}

/// <summary>
/// 動かす
/// </summary>
/// <param name="gameFrame">ゲームフレーム</param>
void SpaceOrA::Move(int gameFrame)
{
	// 180 ~ 240フレームで、不透明度を上げる
	if (gameFrame > 180 && gameFrame < 240)
	{
		float rate = static_cast<float>(gameFrame - 180) / 60.0f;

		alpha_ = static_cast<int>((1.0f - rate) * 0.0f + rate * 224.0f);
	}

	// 240フレームで、点滅させる
	if (gameFrame == 240)
	{
		frame_.current++;

		// 0 ~ 30フレームで不透明度を下げる
		if (frame_.current > 0 && frame_.current <= 30)
		{
			frame_.rate = static_cast<float>(frame_.current) / 30.0f;

			alpha_ = static_cast<int>((1.0f - frame_.rate) * 224.0f + frame_.rate * 64.0f);
		}

		// 30 ~ 60フレームで不透明度を上げる
		if (frame_.current > 30 && frame_.current <= 60)
		{
			frame_.rate = static_cast<float>(frame_.current - 30) / 30.0f;

			alpha_ = static_cast<int>((1.0f - frame_.rate) * 64.0f + frame_.rate * 224.0f);
		}

		// 120フレームで、フレームを初期化する
		if (frame_.current >= 60)
		{
			frame_.current = 0;
		}
	}

	// 241 ~ 256フレームで、不透明度を上げる
	if (gameFrame > 241 && gameFrame <= 256)
	{
		float rate = static_cast<float>(gameFrame - 241) / 15.0f;

		alpha_ = static_cast<int>((1.0f - rate) * 224.0f + rate * 255.0f);
	}

	// 257 ~ 287フレームで、不透明度を下げる
	if (gameFrame > 257 && gameFrame <= 287)
	{
		float rate = static_cast<float>(gameFrame - 257) / 30.0f;

		alpha_ = static_cast<int>((1.0f - rate) * 255.0f + rate * 0.0f);
	}
}

/// <summary>
/// 描画する
/// </summary>
/// <param name="gameFrame">ゲームフレーム</param>
void SpaceOrA::Draw(int gameFrame)
{
	// 90 ~ 300フレームで描画する
	if (gameFrame > 0 && gameFrame <= 300)
	{
		Novice::DrawQuad
		(
			static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
			static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
			static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
			static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
			0, 0, 384, 128, gh_, 0xFFFFFF00 + alpha_
		);
	}
}