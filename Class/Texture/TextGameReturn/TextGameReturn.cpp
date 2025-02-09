#include "TextGameReturn.h"

/// <summary>
/// コンストラクタ
/// </summary>
TextGameReturn::TextGameReturn()
{
	// 画像
	gh_ = Novice::LoadTexture("./Resources/Images/Scene/text_game_return.png");

	// 図形
	shape_.scale = { 144.0f , 48.0f };
	shape_.theta = 0.0f;
	shape_.translate = { static_cast<float>(kScreenWidth / 2) + 400.0f , static_cast<float>(kScreenHeight / 2) - 100.0f };

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
TextGameReturn::~TextGameReturn()
{

}

/// <summary>
/// 動かす
/// </summary>
/// <param name="gameFrame">ゲームフレーム</param>
void TextGameReturn::Move(int gameFrame)
{
	gameFrame = 0;

	if (Scene::poseNo_ == POSE_GAME_RETURN)
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
	} else
	{
		alpha_ = 32;
	}
}

/// <summary>
/// 描画する
/// </summary>
/// <param name="gameFrame">ゲームフレーム</param>
void TextGameReturn::Draw(int gameFrame)
{
	// 540フレームで描画する
	if (gameFrame == 540)
	{
		Novice::DrawQuad
		(
			static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
			static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
			static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
			static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
			0, 0, 288, 96, gh_, 0xFFFFFF00 + alpha_
		);
	}
}