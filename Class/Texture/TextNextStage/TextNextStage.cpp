#include "TextNextStage.h"

/// <summary>
/// コンストラクタ
/// </summary>
TextNextStage::TextNextStage()
{
	// 画像
	gh_ = Novice::LoadTexture("./Resources/Images/Scene/text_next_stage.png");

	// 図形
	shape_.scale = { 192.0f , 48.0f };
	shape_.theta = 0.0f;
	shape_.translate = { static_cast<float>(kScreenWidth / 2) , static_cast<float>(kScreenHeight / 2) - 100.0f };

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
TextNextStage::~TextNextStage()
{

}

/// <summary>
/// 動かす
/// </summary>
/// <param name="gameFrame">ゲームフレーム</param>
void TextNextStage::Move(int gameFrame)
{
	if (Scene::isClear_)
	{
		// 570 ~ 600フレームで、不透明度を上げる
		if (gameFrame > 570 && gameFrame < 600)
		{
			float rate = static_cast<float>(gameFrame - 570) / 30.0f;

			alpha_ = static_cast<int>((1.0f - rate) * 0.0f + rate * 32.0f);
		}

		if (gameFrame == 600)
		{
			if (Scene::clearNo_ == CLEAR_NEXT_GAME)
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
			else
			{
				alpha_ = 32;
			}
		}

		if (Scene::clearNo_ == CLEAR_NEXT_GAME)
		{
			if (gameFrame > 600 && gameFrame <= 640)
			{
				float rate = static_cast<float>(gameFrame - 600) / 40.0f;

				alpha_ = static_cast<int>((1.0f - rate) * 255.0f + rate * 0.0f);
			}
		}
		else
		{
			if (gameFrame > 600 && gameFrame <= 610)
			{
				float rate = static_cast<float>(gameFrame - 600) / 10.0f;

				alpha_ = static_cast<int>((1.0f - rate) * 32.0f + rate * 0.0f);
			}
		}
	}
}

/// <summary>
/// 描画する
/// </summary>
/// <param name="gameFrame">ゲームフレーム</param>
void TextNextStage::Draw(int gameFrame)
{
	// クリア時に、表示する
	if (Scene::isClear_)
	{
		// 540 ~ 660フレームで描画する
		if (gameFrame > 540 && gameFrame <= 660)
		{
			Novice::DrawQuad
			(
				static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
				static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
				static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
				static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
				0, 0, 384, 96, gh_, 0xFFFFFF00 + alpha_
			);
		}
	}
}