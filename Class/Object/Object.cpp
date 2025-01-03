#include "Object.h"

/// <summary>
/// マップを更新する
/// </summary>
/// <param name="velX">X軸の移動速度</param>
/// <param name="velY">Y軸の移動速度</param>
void Object::MapUpdate(float velX , float velY)
{
	map_.leftTop = { static_cast<int>(pos_.screen.leftTop.x + velX) / kTileSize ,static_cast<int>(pos_.screen.leftTop.y - velY) / kTileSize };
	map_.rightTop = { static_cast<int>(pos_.screen.rightTop.x + velX - 1.0f) / kTileSize ,static_cast<int>(pos_.screen.rightTop.y - velY) / kTileSize };
	map_.leftBottom = { static_cast<int>(pos_.screen.leftBottom.x + velX) / kTileSize ,static_cast<int>(pos_.screen.leftBottom.y - velY - 1.0f) / kTileSize };
	map_.rightBottom = { static_cast<int>(pos_.screen.rightBottom.x + velX - 1.0f) / kTileSize ,static_cast<int>(pos_.screen.rightBottom.y - velY - 1.0f) / kTileSize };
}

/// <summary>
/// ローカル座標をスクリーン座標に変換する
/// </summary>
void Object::LocalToScreen()
{
	// 全てを結合した行列
	struct Matrix3x3 wvpVpMatrix;

	wvpVpMatrix = Multiply(MakeWorldMatrix(shape_.scale, shape_.theta, shape_.translate),
		MakeInverseMatrix(MakeWorldMatrix({ 1.0f , 1.0f }, 0.0f, { 0.0f , 0.0f })));

	wvpVpMatrix = Multiply(wvpVpMatrix, MakeOrthoMatrix(0.0f, static_cast<float>(kScreenHeight), static_cast<float>(kScreenWidth), 0.0f));

	wvpVpMatrix = Multiply(wvpVpMatrix, MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kScreenWidth), static_cast<float>(kScreenHeight)));

	pos_.screen.leftTop = Transform(pos_.local.leftTop, wvpVpMatrix);
	pos_.screen.rightTop = Transform(pos_.local.rightTop, wvpVpMatrix);
	pos_.screen.leftBottom = Transform(pos_.local.leftBottom, wvpVpMatrix);
	pos_.screen.rightBottom = Transform(pos_.local.rightBottom, wvpVpMatrix);

	// 四捨五入を行う
	pos_.screen.leftTop.x = roundf(pos_.screen.leftTop.x);
	pos_.screen.leftTop.y = roundf(pos_.screen.leftTop.y);
	pos_.screen.rightTop.x = roundf(pos_.screen.rightTop.x);
	pos_.screen.rightTop.y = roundf(pos_.screen.rightTop.y);
	pos_.screen.leftBottom.x = roundf(pos_.screen.leftBottom.x);
	pos_.screen.leftBottom.y = roundf(pos_.screen.leftBottom.y);
	pos_.screen.rightBottom.x = roundf(pos_.screen.rightBottom.x);
	pos_.screen.rightBottom.y = roundf(pos_.screen.rightBottom.y);

	// マップを更新する
	MapUpdate(0.0f, 0.0f);
}