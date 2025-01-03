#pragma once

// ベクトル
struct Vec2
{
	float x;
	float y;
};

// 行列
struct Matrix3x3
{
	float m[3][3];
};

// 図形
struct Shape
{
	// 大きさ
	struct Vec2 scale;

	// 角度
	float theta;

	// 移動
	struct Vec2 translate;
};

// 頂点
struct Vertex
{
	// 左上
	struct Vec2 leftTop;

	// 右上
	struct Vec2 rightTop;

	// 左下
	struct Vec2 leftBottom;

	// 右下
	struct Vec2 rightBottom;
};

// マップ
struct Map2
{
	int column;
	int row;
};

// 頂点のマップ
struct VertexMap
{
	// 左上
	struct Map2 leftTop;

	// 右上
	struct Map2 rightTop;

	// 左下
	struct Map2 leftBottom;

	// 右下
	struct Map2 rightBottom;
};

// 位置
struct Pos2
{
	// ローカル座標
	struct Vertex local;

	// スクリーン座標
	struct Vertex screen;
};

// ジャンプ
struct Jump
{
	// ジャンプしているかどうか（ジャンプフラグ）
	int isJump;

	// 落下速度
	float fallingVel;
};