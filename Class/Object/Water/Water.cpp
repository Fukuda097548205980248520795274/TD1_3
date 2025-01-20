#include "Water.h"

/// <summary>
/// コンストラクタ
/// </summary>
Water::Water()
{
	// 放出しているかどうか（放出フラグ）
	isEmission_ = false;

	// 図形
	shape_.scale = { 4.0f , 4.0f };
	shape_.theta = 0.0f;
	shape_.translate = { 0.0f , 0.0f };

	// 位置
	pos_.local = { {-1.0f , 1.0f} , {1.0f , 1.0f} , {-1.0f , -1.0f} , {1.0f , -1.0f} };
	LocalToScreen();

	// 加速度
	acceleration_ = { 0.0f , 0.0f };

	// 移動速度
	vel_ = { 0.0f , 0.0f };

	// 質量
	mass_ = 3.0f;
}

/// <summary>
/// 放出する
/// </summary>
/// <param name="column">列</param>
/// <param name="row">行</param>
void Water::Emission(int column, int row)
{
	// すでに放出している（放出フラグがtrueである）水は、放出しない
	if (isEmission_)
	{
		return;
	}


	// 放出する（放出フラグがtrueになる）
	isEmission_ = true;

	// 図形
	shape_.scale = { 4.0f , 4.0f };
	shape_.theta = 0.0f;
	shape_.translate = { static_cast<float>(column * kTileSize + kTileSize / 2) ,
		static_cast<float>((kMapRow - 1 - row) * kTileSize) - shape_.scale.y };
	LocalToScreen();

	// 加速度
	acceleration_ = { 0.0f , 0.0f };

	// 移動速度
	vel_ = { 0.0f , 0.0f };
}

/// <summary>
/// 動かす
/// </summary>
void Water::Move()
{
	// 放出していない（放出フラグがfalseである）水は、動かさない
	if (isEmission_ == false)
	{
		return;
	}


	// 空気抵抗
	float airResistanceY = kAir * -vel_.y;

	// 空気抵抗の加速度
	float airResistanceAccelerationY = airResistanceY / mass_;

	// 加速度
	acceleration_.y = kGravity + airResistanceAccelerationY;

	// 加速度を加算する
	vel_.y += acceleration_.y / 60.0f;

	// 動かす
	shape_.translate.y += vel_.y;
	LocalToScreen();

	// 地面についたら、消える（放出フラグがfalseになる）
	if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_GROUND ||
		Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_GROUND ||
		Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_BOTTOM ||
		Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_TOP ||
		Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM ||
		Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_TOP ||
		Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_BLOCK ||
		Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_BLOCK ||
		Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_ROTTED ||
		Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_ROTTED ||
		Map::map_[map_.leftBottom.row][map_.leftBottom.column] < 0 || Map::map_[map_.rightBottom.row][map_.rightBottom.column] < 0)
	{
		if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_BOTTOM)
		{
			if (shape_.translate.y + vel_.y - shape_.scale.y <
				static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) +
				((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2))
			{
				isEmission_ = false;
			}
		}
		else if (Map::map_[map_.leftBottom.row][map_.leftBottom.column] == TILE_SLOPE_RIGHT_TOP)
		{
			if (shape_.translate.y + vel_.y - shape_.scale.y <
				static_cast<float>(kScreenHeight - (map_.rightBottom.row + 1) * kTileSize) +
				static_cast<float>(kTileSize / 2) +
				((shape_.translate.x + shape_.scale.x - static_cast<float>(map_.rightBottom.column * kTileSize)) / kTileSize) * (kTileSize / 2))
			{
				isEmission_ = false;
			}
		}
		else if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_BOTTOM)
		{
			if (shape_.translate.y + vel_.y - shape_.scale.y <
				static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) +
				(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2))
			{
				isEmission_ = false;
			}
		}
		else if (Map::map_[map_.rightBottom.row][map_.rightBottom.column] == TILE_SLOPE_LEFT_TOP)
		{
			if (shape_.translate.y + vel_.y - shape_.scale.y <
				static_cast<float>(kScreenHeight - (map_.leftBottom.row + 1) * kTileSize) +
				static_cast<float>(kTileSize / 2) +
				(1.0f - ((shape_.translate.x - shape_.scale.x - static_cast<float>(map_.leftBottom.column * kTileSize)) / kTileSize)) * (kTileSize / 2))
			{
				isEmission_ = false;
			}
		}
		else
		{
			isEmission_ = false;
		}
	}
}

/// <summary>
/// 描画する
/// </summary>
void Water::Draw()
{
	// 放出していない（放出フラグがfalseである）ときは、描画しない
	if (isEmission_ == false)
	{
		return;
	}

	int ghWhite = Novice::LoadTexture("./NoviceResources/white1x1.png");

	Novice::DrawQuad
	(
		static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
		static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
		static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
		static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
		0, 0, 48, 32, ghWhite, 0x0000FFFF
	);
}