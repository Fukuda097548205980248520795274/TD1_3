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

	// 640 ~ 700フレームで、不透明度を上げる
	if (gameFrame > 640 && gameFrame <= 700)
	{
		float frameRate = static_cast<float>(gameFrame - 640) / 60.0f;

		alpha_ = static_cast<int>((1.0f - frameRate) * 0.0f + frameRate * 255.0f);
	}

	// 700 ~ 760フレームで、不透明度を下げる
	if (gameFrame > 700 && gameFrame <= 760)
	{
		float frameRate = static_cast<float>(gameFrame - 700) / 60.0f;

		alpha_ = static_cast<int>((1.0f - frameRate) * 255.0f + frameRate * 0.0f);
	}

	// 760 ~ 820フレームで、不透明度を上げる
	if (gameFrame > 760 && gameFrame <= 820)
	{
		float frameRate = static_cast<float>(gameFrame - 760) / 60.0f;

		alpha_ = static_cast<int>((1.0f - frameRate) * 0.0f + frameRate * 255.0f);
	}

	// 820 ~ 880フレームで、不透明度を下げる
	if (gameFrame > 820 && gameFrame <= 880)
	{
		float frameRate = static_cast<float>(gameFrame - 820) / 60.0f;

		alpha_ = static_cast<int>((1.0f - frameRate) * 255.0f + frameRate * 0.0f);
	}

	// 940 ~ 1000フレームで、不透明度を上げる
	if (gameFrame > 940 && gameFrame <= 1000)
	{
		float frameRate = static_cast<float>(gameFrame - 940) / 60.0f;

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

	// 640 ~ 760フレームで描画する
	if (gameFrame > 640 && gameFrame <= 760)
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

	// 940 ~ 1000フレームで描画する
	if (gameFrame > 940 && gameFrame <= 1000)
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