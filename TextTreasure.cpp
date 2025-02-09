#include "TextTreasure.h"

/// <summary>
/// コンストラクタ
/// </summary>
TextTreasure::TextTreasure()
{
	// 画像
	gh_ = Novice::LoadTexture("./Resources/Images/Scene/treasureText.png");

	int  ghnum_[5];

	ghnum_[0] = Novice::LoadTexture("./Resources/Images/Scene/Number/0.png");
	ghnum_[1] = Novice::LoadTexture("./Resources/Images/Scene/Number/1.png");
	ghnum_[2] = Novice::LoadTexture("./Resources/Images/Scene/Number/2.png");
	ghnum_[3] = Novice::LoadTexture("./Resources/Images/Scene/Number/3.png");
	ghnum_[4] = Novice::LoadTexture("./Resources/Images/Scene/Number/4.png");
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
TextTreasure::~TextTreasure()
{
}

/// <summary>
/// 描画する
/// </summary>
/// <param name="gameFrame">ゲームフレーム</param>
void TextTreasure::Draw(int gameFrame)
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
