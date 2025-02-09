#include "Transition.h"

/// <summary>
/// コンストラクタ
/// </summary>
Transition::Transition()
{
	// 画像
	gh_ = Novice::LoadTexture("./NoviceResources/white1x1.png");

	// 図形
	shape_.scale = { static_cast<float>(kScreenWidth / 2) , static_cast<float>(kScreenHeight / 2) };
	shape_.theta = 0.0f;
	shape_.translate = { static_cast<float>(kScreenWidth / 2) , static_cast<float>(kScreenHeight / 2) };

	// 位置
	pos_.local = { {-1.0f , 1.0f} , {1.0f , 1.0f} , {-1.0f , -1.0f} , {1.0f , -1.0f} };
	LocalToScreen();

	// 移動速度
	vel_ = { 0.0f , 0.0f };

	// 透明度
	alpha_ = 255;

	// フレーム
	frame_ = { 0 , 0 , 0.0f };
}

/// <summary>
/// デストラクタ
/// </summary>
Transition::~Transition()
{

}

/// <summary>
/// 動かす
/// </summary>
/// <param name="gameFrame">ゲームフレーム</param>
void Transition::Move(int gameFrame)
{
	// 0 ~ 60フレームで、不透明度を下げる
	if (gameFrame > 0 && gameFrame <= 60)
	{
		float frameRate = static_cast<float>(gameFrame) / 60.0f;

		alpha_ = static_cast<int>((1.0f - frameRate) * 255.0f + frameRate * 0.0f);
	}

	// 300 ~ 360フレームで、不透明度を上げる
	if (gameFrame > 300 && gameFrame <= 360)
	{
		float frameRate = static_cast<float>(gameFrame - 300) / 60.0f;

		alpha_ = static_cast<int>((1.0f - frameRate) * 0.0f + frameRate * 255.0f);
	}

	// 360 ~ 420フレームで、不透明度を下げる
	if (gameFrame > 360 && gameFrame <= 420)
	{
		float frameRate = static_cast<float>(gameFrame - 360) / 60.0f;

		alpha_ = static_cast<int>((1.0f - frameRate) * 255.0f + frameRate * 0.0f);
	}

	// 420 ~ 480フレームで、不透明度を上げる
	if (gameFrame > 420 && gameFrame <= 480)
	{
		float frameRate = static_cast<float>(gameFrame - 420) / 60.0f;

		alpha_ = static_cast<int>((1.0f - frameRate) * 0.0f + frameRate * 255.0f);
	}

	// 480 ~ 540フレームで、不透明度を下げる
	if (gameFrame > 480 && gameFrame <= 540)
	{
		float frameRate = static_cast<float>(gameFrame - 480) / 60.0f;

		alpha_ = static_cast<int>((1.0f - frameRate) * 255.0f + frameRate * 0.0f);
	}

	// 600 ~ 660フレームで、不透明度を上げる
	if (gameFrame > 600 && gameFrame <= 660)
	{
		float frameRate = static_cast<float>(gameFrame - 600) / 60.0f;

		alpha_ = static_cast<int>((1.0f - frameRate) * 0.0f + frameRate * 255.0f);
	}
}

/// <summary>
/// 描画する
/// </summary>
/// <param name="gameFrame">ゲームフレーム</param>
void Transition::Draw(int gameFrame)
{
	// 0 ~ 60フレームで描画する
	if (gameFrame > 0 && gameFrame <= 60)
	{
		Novice::DrawQuad
		(
			static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
			static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
			static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
			static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
			0, 0, 1,1, gh_, 0xFFFFFF00 + alpha_
		);
	}


	// 300 ~ 420フレームで描画する
	if (gameFrame > 300 && gameFrame <= 420)
	{
		Novice::DrawQuad
		(
			static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
			static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
			static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
			static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
			0, 0, 1, 1, gh_, 0xFFFFFF00 + alpha_
		);
	}

	// 420 ~ 540フレームで描画する
	if (gameFrame > 420 && gameFrame <= 540)
	{
		Novice::DrawQuad
		(
			static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
			static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
			static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
			static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
			0, 0, 1, 1, gh_, 0xFFFFFF00 + alpha_
		);
	}

	// 760 ~ 880フレームで描画する
	if (gameFrame > 760 && gameFrame <= 880)
	{
		Novice::DrawQuad
		(
			static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
			static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
			static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
			static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
			0, 0, 1, 1, gh_, 0xFFFFFF00 + alpha_
		);
	}

	// 600 ~ 660フレームで描画する
	if (gameFrame > 600 && gameFrame <= 660)
	{
		Novice::DrawQuad
		(
			static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
			static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
			static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
			static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
			0, 0, 1, 1, gh_, 0xFFFFFF00 + alpha_
		);
	}
}