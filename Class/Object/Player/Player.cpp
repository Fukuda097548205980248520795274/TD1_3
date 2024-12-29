#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
{
	// 図形
	shape_.scale = { 24.0f , 24.0f };
	shape_.theta = 0.0f;
	shape_.translate = { 36.0f , 36.0f };

	// 位置
	pos_.local = { {-1.0f , 1.0f} , {1.0f , 1.0f} , {-1.0f , -1.0f} , {1.0f , -1.0f} };
	LocalToScreen();

	// 移動速度
	vel_ = { 0.0f , 0.0f };
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{

}

/// <summary>
/// 操作する
/// </summary>
/// <param name="keys">キー</param>
/// <param name="preKeys">前のキー</param>
void Player::Operation(const char* keys, const char* preKeys)
{
	// null を探す
	if (keys == nullptr || preKeys == nullptr)
	{
		return;
	}


	/*   横移動   */

	// 横移動の速度を初期化する
	vel_.x = 0.0f;

	// Aキーで、左に移動する
	if (keys[DIK_A])
	{
		if (shape_.translate.x - shape_.scale.x > 0.0f)
		{
			vel_.x = -4.0f;
		}
	}

	// Dキーで、右に移動する
	if (keys[DIK_D])
	{
		if (shape_.translate.x + shape_.scale.x < static_cast<float>(kScreenWidth))
		{
			vel_.x = 4.0f;
		}
	}

	// 動かす
	shape_.translate.x += vel_.x;
	LocalToScreen();
}

/// <summary>
/// 描画する
/// </summary>
/// <param name="gh">画像</param>
void Player::Draw(int gh)
{
	Novice::DrawQuad
	(
		static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
		static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
		static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
		static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
		0, 0, 1, 1, gh, 0xFFFFFFFF
	);
}