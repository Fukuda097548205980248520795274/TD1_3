#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
{
	// 図形
	shape_.scale = { 18.0f , 22.0f };
	shape_.theta = 0.0f;
	shape_.translate = { 0.0f , 0.0f };

	// 位置
	pos_.local = { {-1.0f , 1.0f} , {1.0f , 1.0f} , {-1.0f , -1.0f} , {1.0f , -1.0f} };
	LocalToScreen();

	// 移動速度
	vel_ = { 0.0f , 0.0f };

	// ジャンプ
	jump_.isJump = false;
	jump_.fallingVel = 0.0f;

	// 乗っかっているかどうか（乗っかりフラグ）
	isRide_ = false;
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{

}

/// <summary>
/// 配置する
/// </summary>
/// <param name="column">列</param>
/// <param name="row">行</param>
void Player::Puttting(int column, int row)
{
	// 位置
	shape_.translate = { static_cast<float>(column * kTileSize) + shape_.scale.x ,
		static_cast<float>((kMapRow - 1 - row) * kTileSize) + shape_.scale.y };
	LocalToScreen();

	// ジャンプ
	jump_.isJump = false;
	jump_.fallingVel = 0.0f;

	// 移動速度
	vel_ = { 0.0f , 0.0f };
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

			// 一時的にマップを更新する
			MapUpdate(vel_.x, 0.0f);

			if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_GROUND ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GROUND ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_BLOCK ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_BLOCK ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_ROTTED ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_ROTTED ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] < 0 || Map::map_[map_.leftBottom.row][map_.leftBottom.column] < 0)
			{
				vel_.x = 0.0f;

				shape_.translate.x = static_cast<float>(map_.leftTop.column * kTileSize + kTileSize) + shape_.scale.x;
				LocalToScreen();
			}
		}
	}
	
	// Dキーで、右に移動する
	if(keys[DIK_D])
	{
		if (shape_.translate.x + shape_.scale.x < static_cast<float>(kScreenWidth))
		{
			vel_.x = 4.0f;

			// 一時的にマップを更新する
			MapUpdate(vel_.x, 0.0f);

			if (Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_GROUND ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GROUND ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_BLOCK ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_BLOCK ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_ROTTED ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_ROTTED ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] < 0 || Map::map_[map_.rightBottom.row][map_.rightBottom.column] < 0)
			{
				vel_.x = 0.0f;

				shape_.translate.x = static_cast<float>(map_.rightTop.column * kTileSize) - shape_.scale.x;
				LocalToScreen();
			}
		}
	}

	// 動かす
	shape_.translate.x += vel_.x;
	LocalToScreen();


	/*   坂   */

	// ジャンプしていない（ジャンプフラグがfalseである）ときは、坂を上ることができる
	if (jump_.isJump == false)
	{
		if (isRide_ == false)
		{
			// 右上がり下
			if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM)
			{
				shape_.translate.y = (static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) + shape_.scale.y) +
					((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2);
			}

			// 右上がり上
			if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
			{
				shape_.translate.y = (static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) + shape_.scale.y) +
					static_cast<float>(kTileSize / 2) +
					((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2);
			}

			// 左上がり下
			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM)
			{
				shape_.translate.y = (static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) + shape_.scale.y) +
					(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2);
			}

			// 左下がり上
			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP)
			{
				shape_.translate.y = (static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) + shape_.scale.y) +
					static_cast<float>(kTileSize / 2) +
					(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2);
			}

			LocalToScreen();
		}
	}

	// 乗っかりフラグをfalseにする
	isRide_ = false;



	/*   梯子   */

	// Wキーで、梯子を上る
	if (keys[DIK_W])
	{
		if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_LADDER ||
			Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_LADDER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_LADDER ||
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER)
		{
			vel_.y = 4.0f;

			// マップを一時的に更新する
			MapUpdate(0.0f, vel_.y);

			// 飛んだ先に天井があったら、天井にぶつかる
			if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_GROUND ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_GROUND ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_SLOPE_LEFT_BOTTOM ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_SLOPE_LEFT_BOTTOM ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_SLOPE_LEFT_TOP ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_SLOPE_LEFT_TOP ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_SLOPE_RIGHT_BOTTOM ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_SLOPE_RIGHT_BOTTOM ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_SLOPE_RIGHT_TOP ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_SLOPE_RIGHT_TOP ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_BLOCK ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_BLOCK ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_ROTTED ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_ROTTED ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] < 0 || Map::map_[map_.rightTop.row][map_.rightTop.column] < 0)
			{
				// 押し込み処理
				shape_.translate.y = static_cast<float>(kScreenHeight - map_.leftTop.row * kTileSize - kTileSize) - shape_.scale.y;
				LocalToScreen();
			}

			shape_.translate.y += vel_.y;
			LocalToScreen();

			// 移動速度を消す
			vel_.y = 0.0f;
		}
	}
	else
	{
		if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_LADDER ||
			Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_LADDER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_LADDER ||
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER)
		{
			shape_.translate.y -= 2.0f;
			LocalToScreen();

			// 降りた先に地面があったら、着地する
			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GROUND ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GROUND ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_BLOCK ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_BLOCK ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_ROTTED ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_ROTTED ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] < 0 || Map::map_[map_.rightBottom.row][map_.rightBottom.column] < 0)
			{
				// 押し込み処理
				shape_.translate.y = static_cast<float>(kScreenHeight - map_.leftBottom.row * kTileSize) + shape_.scale.y;
				LocalToScreen();
			}
		}
	}


	/*   ジャンプ   */

	// Jキーで、ジャンプする（ジャンプフラグがtrueになる）
	if (!preKeys[DIK_J] && keys[DIK_J])
	{
		if (jump_.isJump == false)
		{
			jump_.isJump = true;
			jump_.fallingVel = -0.5f;

			// 上方向に上昇する
			vel_.y = 12.0f;
		}
	}

	// 空中にいると、自動でジャンプ状態になる（ジャンプフラグがtrueになる）
	if (jump_.isJump == false)
	{
		// 一時的にマップを更新する
		MapUpdate(0.0f, -1.0f);

		if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_NOTHING &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_NOTHING ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_LADDER &&
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
			Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP ||
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
			Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
		{
			// 坂にいる場合
			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
			{
				// 右上がり上で落下する（ジャンプフラグがtrueになる）
				if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM)
				{
					if (shape_.translate.y + vel_.y - shape_.scale.y >
						static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) +
						((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2))
					{
						jump_.isJump = true;
						jump_.fallingVel = -0.5f;

						// 速度に変化なし
						vel_.y = 0.0f;
					}
				}
				else if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
				{
					// 右上がり上で落下（ジャンプフラグがtrueになる）

					if (shape_.translate.y + vel_.y - shape_.scale.y >
						static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) +
						static_cast<float>(kTileSize / 2) +
						((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2))
					{
						jump_.isJump = true;
						jump_.fallingVel = -0.5f;

						// 速度に変化なし
						vel_.y = 0.0f;
					}
				}
				else if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM)
				{
					// 左上がり下で落下する（ジャンプフラグがtrueになる）

					if (shape_.translate.y + vel_.y - shape_.scale.y >
						static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) +
						(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2))
					{
						jump_.isJump = true;
						jump_.fallingVel = -0.5f;

						// 速度に変化なし
						vel_.y = 0.0f;
					}
				}
				else if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP)
				{
					// 左上がり上で落下する（ジャンプフラグがtrueになる）

					if (shape_.translate.y + vel_.y - shape_.scale.y >
						static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) +
						static_cast<float>(kTileSize / 2) +
						(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2))
					{
						jump_.isJump = true;
						jump_.fallingVel = -0.5f;

						// 速度に変化なし
						vel_.y = 0.0f;
					}
				}
			}
			else
			{
				jump_.isJump = true;
				jump_.fallingVel = -0.5f;

				// 速度に変化なし
				vel_.y = 0.0f;
			}
		}

		// マップを戻す
		MapUpdate(0.0f, 0.0f);
	}

	// 梯子の場所では、ジャンプができない（ジャンプフラグがfalseになる）
	if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_LADDER ||
		Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_LADDER ||
		Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_LADDER ||
		Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_LADDER )
	{
		jump_.isJump = false;
		jump_.fallingVel = 0.0f;

		vel_.y = 0.0f;
	}

	// ジャンプしている（ジャンプフラグがtrueである）ときの動き
	if (jump_.isJump)
	{
		// ブロックのサイズまで、落下する
		if (vel_.y < static_cast<float>(kTileSize) && vel_.y > -static_cast<float>(kTileSize))
		{
			vel_.y += jump_.fallingVel;
		}

		// マップを一時的に更新する
		MapUpdate(0.0f, vel_.y);

		// 上に飛んでいるとき
		if (vel_.y > 0.0f)
		{
			// 飛んだ先に天井があったら、天井にぶつかる
			if (Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_GROUND ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_GROUND ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_SLOPE_LEFT_BOTTOM ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_SLOPE_LEFT_BOTTOM ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_SLOPE_LEFT_TOP ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_SLOPE_LEFT_TOP ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_SLOPE_RIGHT_BOTTOM ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_SLOPE_RIGHT_BOTTOM ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_SLOPE_RIGHT_TOP ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_SLOPE_RIGHT_TOP ||
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_BLOCK ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_BLOCK || 
				Map::map_[map_.leftTop.row][map_.leftTop.column] == TILE_ROTTED ||
				Map::map_[map_.rightTop.row][map_.rightTop.column] == TILE_ROTTED || 
				Map::map_[map_.leftTop.row][map_.leftTop.column] < 0 || Map::map_[map_.rightTop.row][map_.rightTop.column] < 0)
			{
				// 押し込み処理
				shape_.translate.y = static_cast<float>(kScreenHeight - map_.leftTop.row * kTileSize - kTileSize) - shape_.scale.y;
				LocalToScreen();

				vel_.y = 0.0f;
			}
		}
		else
		{
			// 下に落下しているとき

			// 落ちた先に地面があったら、着地する（ジャンプフラグがfalseになる）
			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GROUND ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GROUND ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_BLOCK ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_BLOCK ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_ROTTED ||
				Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_ROTTED ||
				Map::map_[map_.leftBottom.row][map_.leftBottom.column] < 0 || Map::map_[map_.rightBottom.row][map_.rightBottom.column] < 0)
			{
				// 押し込み処理
				shape_.translate.y = static_cast<float>(kScreenHeight - map_.leftBottom.row * kTileSize) + shape_.scale.y;
				LocalToScreen();

				jump_.isJump = false;
				jump_.fallingVel = 0.0f;

				vel_.y = 0.0f;
			}


			/*   坂に着地する   */

			// 右上がり下で着地する（ジャンプフラグがfalseになる）
			if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_BOTTOM)
			{
				if (shape_.translate.y + vel_.y - shape_.scale.y <
					static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) +
					((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2))
				{
					// 押し込み処理
					shape_.translate.y = 
						(static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) + shape_.scale.y) +
						((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2);
					LocalToScreen();

					jump_.isJump = false;
					jump_.fallingVel = 0.0f;

					vel_.y = 0.0f;
				}
			}

			// 右上がり上で着地する（ジャンプフラグがfalseになる）
			if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_RIGHT_TOP)
			{
				if (shape_.translate.y + vel_.y - shape_.scale.y <
					static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) +
					static_cast<float>(kTileSize / 2) +
					((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2))
				{
					// 押し込み処理
					shape_.translate.y = 
						(static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) + shape_.scale.y) +
						static_cast<float>(kTileSize / 2) +
						((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2);
					LocalToScreen();

					jump_.isJump = false;
					jump_.fallingVel = 0.0f;

					vel_.y = 0.0f;
				}
			}

			// 左上がり下で着地する（ジャンプフラグがfalseになる）
			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_BOTTOM)
			{
				if (shape_.translate.y + vel_.y - shape_.scale.y < 
					static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) +
					(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2))
				{
					// 押し込み処理
					shape_.translate.y = 
						(static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) + shape_.scale.y) +
						(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2);
					LocalToScreen();

					jump_.isJump = false;
					jump_.fallingVel = 0.0f;

					vel_.y = 0.0f;
				}
			}

			// 左上がり上で着地する（ジャンプフラグがfalseになる）
			if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_LEFT_TOP)
			{
				if (shape_.translate.y + vel_.y - shape_.scale.y < 
					static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) +
					static_cast<float>(kTileSize / 2) +
					(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2))
				{
					// 押し込み処理
					shape_.translate.y = 
						(static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) + shape_.scale.y) +
						static_cast<float>(kTileSize / 2) +
						(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2);
					LocalToScreen();

					jump_.isJump = false;
					jump_.fallingVel = 0.0f;

					vel_.y = 0.0f;
				}
			}
		}
	}

	// 動かす
	shape_.translate.y += vel_.y;
	LocalToScreen();
}

/// <summary>
/// ブロックの上に着地する
/// </summary>
/// <param name="block">ブロック</param>
void Player::BlockLanding(CarryBlock* block)
{
	// null を探す
	if (block == nullptr)
	{
		return;
	}
	

	if (block->isPut_)
	{
		// ジャンプしている（ジャンプフラグがtrueである）ときに、ブロックの上に着地できる
		if (jump_.isJump)
		{
			if (vel_.y > 0.0f)
			{
				// 先に進んでいる場合
				if (shape_.translate.y + vel_.y + shape_.scale.y > block->shape_.translate.y - block->shape_.scale.y &&
					shape_.translate.y + vel_.y + shape_.scale.y < block->shape_.translate.y + block->shape_.scale.y)
				{
					if (shape_.translate.x + shape_.scale.x > block->shape_.translate.x - block->shape_.scale.x &&
						shape_.translate.x - shape_.scale.x < block->shape_.translate.x + block->shape_.scale.x)
					{
						// 進む直前
						if (shape_.translate.y - 1.0f + shape_.scale.y > block->shape_.translate.y - block->shape_.scale.y ||
							shape_.translate.y - 1.0f + shape_.scale.y < block->shape_.translate.y + block->shape_.scale.y)
						{
							// 押し込み処理
							shape_.translate.y = block->shape_.translate.y - block->shape_.scale.y - shape_.scale.y;
							LocalToScreen();

							vel_.y = 0.0f;

							// 乗っかりフラグがtrueになる
							isRide_ = true;
						}
					}
				}
			}
			else if (vel_.y < 0.0f)
			{
				// 先に進んでいる場合
				if (shape_.translate.y + vel_.y - shape_.scale.y < block->shape_.translate.y + block->shape_.scale.y &&
					shape_.translate.y + vel_.y - shape_.scale.y > block->shape_.translate.y - block->shape_.scale.y)
				{
					if (shape_.translate.x + shape_.scale.x > block->shape_.translate.x - block->shape_.scale.x &&
						shape_.translate.x - shape_.scale.x < block->shape_.translate.x + block->shape_.scale.x)
					{
						// 進む直前
						if (shape_.translate.y + 1.0f - shape_.scale.y > block->shape_.translate.y + block->shape_.scale.y ||
							shape_.translate.y + 1.0f - shape_.scale.y < block->shape_.translate.y - block->shape_.scale.y)
						{
							// 押し込み処理
							shape_.translate.y = block->shape_.translate.y + block->shape_.scale.y + shape_.scale.y;
							LocalToScreen();

							jump_.isJump = false;
							jump_.fallingVel = 0.0f;

							vel_.y = 0.0f;

							// 乗っかりフラグがtrueになる
							isRide_ = true;
						}
					}
				}
			}
		}
	}
}

/// <summary>
/// ブロックを運ぶ
/// </summary>
/// <param name="carryBlock">運べるブロック</param>
void Player::Carry(CarryBlock* block1 , CarryBlock* block2)
{
	// null を探す
	if (block1 == nullptr || block2 == nullptr)
	{
		return;
	}


	/*   運ぶ   */

	// 配置されている（配置フラグがtrueである）ブロックを押すことができる
	if (block1->isPut_)
	{
		// ジャンプしていない（ジャンプフラグがfalseである）とき
		if (jump_.isJump == false && block1->jump_.isJump == false)
		{
			// 運びたいブロックに、別のブロックが乗っかていない（のかかりフラグがfalseである）ときは、運べる
			if (block1->isUnderRide_ == false)
			{
				// 右から押すとき
				if (vel_.x > 0.0f)
				{
					if (shape_.translate.x + shape_.scale.x > block1->shape_.translate.x - block1->shape_.scale.x &&
						shape_.translate.x + shape_.scale.x < block1->shape_.translate.x + block1->shape_.scale.x)
					{
						if (shape_.translate.y + shape_.scale.y / 2 > block1->shape_.translate.y - block1->shape_.scale.y &&
							shape_.translate.y - shape_.scale.y / 2 < block1->shape_.translate.y + block1->shape_.scale.y)
						{
							block1->vel_.x = 2.0f;

							// 一時的にマップを更新する
							block1->MapUpdate(block1->vel_.x, 0.0f);

							// 進んだ先で壁にぶつかるとき
							if (Map::map_[block1->map_.rightTop.row][block1->map_.rightTop.column] == TILE_GROUND ||
								Map::map_[block1->map_.rightBottom.row][block1->map_.rightBottom.column] == TILE_GROUND ||
								Map::map_[block1->map_.rightTop.row][block1->map_.rightTop.column] == TILE_BLOCK ||
								Map::map_[block1->map_.rightBottom.row][block1->map_.rightBottom.column] == TILE_BLOCK ||
								Map::map_[block1->map_.rightTop.row][block1->map_.rightTop.column] == TILE_ROTTED ||
								Map::map_[block1->map_.rightBottom.row][block1->map_.rightBottom.column] == TILE_ROTTED ||
								Map::map_[block1->map_.rightTop.row][block1->map_.rightTop.column] < 0 || Map::map_[block1->map_.rightBottom.row][block1->map_.rightBottom.column] < 0)
							{
								block1->vel_.x = 0.0f;

								// 押し込み処理
								block1->shape_.translate.x = static_cast<float>(block1->map_.rightTop.column * kTileSize) - block1->shape_.scale.x;
							}

							// 動かす
							block1->shape_.translate.x += block1->vel_.x;
							block1->LocalToScreen();

							vel_.x = 0.0f;
							shape_.translate.x = block1->shape_.translate.x - block1->shape_.scale.x - shape_.scale.x;
							LocalToScreen();
						}
					}
				} 
				else if (vel_.x < 0.0f)
				{
					// 左から押すとき

					if (shape_.translate.x - shape_.scale.x < block1->shape_.translate.x + block1->shape_.scale.x &&
						shape_.translate.x - shape_.scale.x > block1->shape_.translate.x - block1->shape_.scale.x)
					{
						if (shape_.translate.y + shape_.scale.y > block1->shape_.translate.y - block1->shape_.scale.y &&
							shape_.translate.y - shape_.scale.y < block1->shape_.translate.y + block1->shape_.scale.y)
						{
							block1->vel_.x = -2.0f;

							// 一時的にマップを更新する
							block1->MapUpdate(block1->vel_.x, 0.0f);

							// 進んだ先で壁にぶつかるとき
							if (Map::map_[block1->map_.leftTop.row][block1->map_.leftTop.column] == TILE_GROUND ||
								Map::map_[block1->map_.leftBottom.row][block1->map_.leftBottom.column] == TILE_GROUND ||
								Map::map_[block1->map_.leftTop.row][block1->map_.leftTop.column] == TILE_BLOCK ||
								Map::map_[block1->map_.leftBottom.row][block1->map_.leftBottom.column] == TILE_BLOCK ||
								Map::map_[block1->map_.leftTop.row][block1->map_.leftTop.column] == TILE_ROTTED ||
								Map::map_[block1->map_.leftBottom.row][block1->map_.leftBottom.column] == TILE_ROTTED ||
								Map::map_[block1->map_.leftTop.row][block1->map_.leftTop.column] < 0 || Map::map_[block1->map_.leftBottom.row][block1->map_.leftBottom.column] < 0)
							{
								block1->vel_.x = 0.0f;

								// 押し込み処理
								block1->shape_.translate.x = static_cast<float>(block1->map_.leftTop.column * kTileSize + kTileSize) + block1->shape_.scale.x;
							}

							// ブロックを動かす
							block1->shape_.translate.x += block1->vel_.x;
							block1->LocalToScreen();

							// プレイヤーの押し込み処理
							vel_.x = 0.0f;
							shape_.translate.x = block1->shape_.translate.x + block1->shape_.scale.x + shape_.scale.x;
							LocalToScreen();
						}
					}
				}

				// もう一つのブロックが配置されていた（配置フラグがtrueである）とき
				if (block2->isPut_)
				{
					// 進んだ先で別のブロックにぶつかるとき
					if (block1->shape_.translate.x + block1->shape_.scale.x > block2->shape_.translate.x - block2->shape_.scale.x &&
						block1->shape_.translate.x + block1->shape_.scale.x < block2->shape_.translate.x + block2->shape_.scale.x)
					{
						if (block1->shape_.translate.y + block1->shape_.scale.y > block2->shape_.translate.y - block2->shape_.scale.y &&
							block1->shape_.translate.y - block1->shape_.scale.y < block2->shape_.translate.y + block2->shape_.scale.y)
						{
							block1->vel_.x = 0.0f;

							// 押し込み処理
							block1->shape_.translate.x = block2->shape_.translate.x - block2->shape_.scale.x - block1->shape_.scale.x;
							block1->LocalToScreen();

							// プレイヤーの押し込み処理
							vel_.x = 0.0f;
							shape_.translate.x = block1->shape_.translate.x - block1->shape_.scale.x - shape_.scale.x;
							LocalToScreen();
						}
					}

					// 進んだ先で別のブロックにぶつかるとき
					if (block1->shape_.translate.x - block1->shape_.scale.x < block2->shape_.translate.x + block2->shape_.scale.x &&
						block1->shape_.translate.x - block1->shape_.scale.x > block2->shape_.translate.x - block2->shape_.scale.x)
					{
						if (block1->shape_.translate.y + block1->shape_.scale.y > block2->shape_.translate.y - block2->shape_.scale.y &&
							block1->shape_.translate.y - block1->shape_.scale.y < block2->shape_.translate.y + block2->shape_.scale.y)
						{
							block1->vel_.x = 0.0f;

							// 押し込み処理
							block1->shape_.translate.x = block2->shape_.translate.x + block2->shape_.scale.x + block1->shape_.scale.x;
							block1->LocalToScreen();

							// プレイヤーの押し込み処理
							vel_.x = 0.0f;
							shape_.translate.x = block1->shape_.translate.x + block1->shape_.scale.x + shape_.scale.x;
							LocalToScreen();
						}
					}
				}
			}
			else
			{
				// 運びたいブロックに、別のブロックが乗っかているときは、運べない

				// 右から押すとき
				if (vel_.x > 0.0f)
				{
					if (shape_.translate.x + shape_.scale.x > block1->shape_.translate.x - block1->shape_.scale.x &&
						shape_.translate.x + shape_.scale.x < block1->shape_.translate.x + block1->shape_.scale.x)
					{
						if (shape_.translate.y + shape_.scale.y > block1->shape_.translate.y - block1->shape_.scale.y &&
							shape_.translate.y - shape_.scale.y < block1->shape_.translate.y + block1->shape_.scale.y)
						{
							vel_.x = 0.0f;
							shape_.translate.x = block1->shape_.translate.x - block1->shape_.scale.x - shape_.scale.x;
							LocalToScreen();
						}
					}
				}
				else if (vel_.x < 0.0f)
				{
					// 左から押すとき

					if (shape_.translate.x - shape_.scale.x < block1->shape_.translate.x + block1->shape_.scale.x &&
						shape_.translate.x - shape_.scale.x > block1->shape_.translate.x - block1->shape_.scale.x)
					{
						if (shape_.translate.y + shape_.scale.y > block1->shape_.translate.y - block1->shape_.scale.y &&
							shape_.translate.y - shape_.scale.y < block1->shape_.translate.y + block1->shape_.scale.y)
						{
							vel_.x = 0.0f;
							shape_.translate.x = block1->shape_.translate.x + block1->shape_.scale.x + shape_.scale.x;
							LocalToScreen();
						}
					}
				}
			}
		}
		else if (jump_.isJump || block1->jump_.isJump)
		{
			// ジャンプしている（ジャンプフラグがtrueである）ときは、押すことができない

			// 右から押すとき
			if (vel_.x > 0.0f)
			{
				if (shape_.translate.x + shape_.scale.x > block1->shape_.translate.x - block1->shape_.scale.x &&
					shape_.translate.x + shape_.scale.x < block1->shape_.translate.x + block1->shape_.scale.x)
				{
					if (shape_.translate.y + shape_.scale.y > block1->shape_.translate.y - block1->shape_.scale.y &&
						shape_.translate.y - shape_.scale.y < block1->shape_.translate.y + block1->shape_.scale.y)
					{
						vel_.x = 0.0f;
						shape_.translate.x = block1->shape_.translate.x - block1->shape_.scale.x - shape_.scale.x;
						LocalToScreen();
					}
				}
			}
			else if (vel_.x < 0.0f)
			{
				// 左から押すとき

				if (shape_.translate.x - shape_.scale.x < block1->shape_.translate.x + block1->shape_.scale.x &&
					shape_.translate.x - shape_.scale.x > block1->shape_.translate.x - block1->shape_.scale.x)
				{
					if (shape_.translate.y + shape_.scale.y > block1->shape_.translate.y - block1->shape_.scale.y &&
						shape_.translate.y - shape_.scale.y < block1->shape_.translate.y + block1->shape_.scale.y)
					{
						vel_.x = 0.0f;
						shape_.translate.x = block1->shape_.translate.x + block1->shape_.scale.x + shape_.scale.x;
						LocalToScreen();
					}
				}
			}
		}
	}
}

/// <summary>
/// 描画する
/// </summary>
void Player::Draw()
{
	int ghWhite = Novice::LoadTexture("./NoviceResources/white1x1.png");

	Novice::DrawQuad
	(
		static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
		static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
		static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
		static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
		0, 0, 1, 1, ghWhite, 0xFFFFFFFF
	);
}